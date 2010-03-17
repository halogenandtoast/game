#include "SDL.h"
#include "SDL_opengl.h"

#include <sys/time.h>
#include <string>
#include "model.h"
#include "glsl.h"
#include "height_map.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif


char *pname;
float angle = 0.0f;
float ypos = 0.0f;
float ydir = 0.05f;

GLboolean Timing = GL_TRUE;
GLenum StrMode = GL_VENDOR;

int gheight, gwidth;

Model myModel1;
HeightMap hm;
static SDL_Surface *gScreen;

static double mtime(void)
{
	struct timeval tk_time;
	struct timezone tz;
	
	gettimeofday(&tk_time, &tz);
	
	return 4294.967296 * tk_time.tv_sec + 0.000001 * tk_time.tv_usec;
}

static double filter(double in, double *save)
{
	static double k1 = 0.9;
	static double k2 = 0.05;
	save[3] = in;
	save[1] = save[0]*k1 + k2*(save[3] + save[2]);
	save[0]=save[1];
	save[2]=save[3];
	return(save[1]);
}

void DrawStr(const char *str)
{
	GLint i = 0;
	
	if(!str) return;
	
	while(str[i])
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
		i++;
	}
}

static void initAttributes ()
{
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
}

int resizeWindow( int width, int height )
{
	gwidth = width;
	gheight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(width)/height, 0.1, 100.0);
	return 0;
}


static void createSurface (int fullscreen)
{
    Uint32 flags = 0;
    
    flags = SDL_OPENGL;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;
    
    gScreen = SDL_SetVideoMode (800, 600, 0, flags);
    resizeWindow( 800, 600 );
	if (gScreen == NULL) 
	{
		fprintf (stderr, "Couldn't set 640x480 OpenGL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
}

static void initGL ()
{
	#ifdef __APPLE__    
		// Let's look in the resources directory
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
		char path[PATH_MAX];
		if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
		{
		}
		CFRelease(resourcesURL);
		chdir(path);
	#endif
	
	hm.load((char *) "hm.gif");
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	GLuint prog_id = createGLSLProgram((char *) "simple.vert", (char *) "simple.frag");
	glUseProgram(prog_id);
	if(!myModel1.loadModel((char *) "teapotn.txt")) {
		std::cout << "Could not load model file" << std::endl;
		return;
	}
}

static void drawGL ()
{
	static double t1 = 0.0, t2 = 0.0, t;
	static double th[4] = {0.0, 0.0, 0.0, 0.0};
	t1 = t2;
	char num_str[128];
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0.0, 0.0f, -10.0f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	//hm.render();
	myModel1.render();
	glPopMatrix();
	
	if(Timing)
	{
		t2 = mtime();
		t = t2 - t1;
		if(t > 0.0001) t = 1.0 / t;
		
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 0.0, 0.0);
		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, gwidth, 0, gheight, -10.0, 10.0);
		glRasterPos2f(5.0, 5.0);
		sprintf(num_str, "%0.2f Hz, %dx%d", filter(t, th), gwidth, gheight);
		DrawStr(num_str);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
	}
	
	glFlush();
}

static void mainLoop ()
{
    SDL_Event event;
    int done = 0;
    int fps = 60;
	int delay = 1000/fps;
    int thenTicks = -1;
    int nowTicks;
    
    while ( !done ) {
		angle += 1.0f;
		if(angle > 360) {
			angle -= 360;
		}
		
		ypos += ydir;
		if(ypos > 3.0f || ypos < -3.0f) ydir *= -1;
		while ( SDL_PollEvent (&event) ) {
			switch (event.type) {

				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.mod & KMOD_META) {
						if (event.key.keysym.sym == SDLK_q) {
							done = 1;
						}
					} else if (event.key.keysym.sym == SDLK_ESCAPE) {
						done = 1;
					}
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
    
        // Draw at 24 hz
        //     This approach is not normally recommended - it is better to
        //     use time-based animation and run as fast as possible
        drawGL ();
        SDL_GL_SwapBuffers ();

        // Time how long each draw-swap-delay cycle takes
        // and adjust delay to get closer to target framerate
        if (thenTicks > 0) {
            nowTicks = SDL_GetTicks ();
            delay += (1000/fps - (nowTicks-thenTicks));
            thenTicks = nowTicks;
            if (delay < 0)
                delay = 1000/fps;
        }
        else {
            thenTicks = SDL_GetTicks ();
        }

        SDL_Delay (delay);
	}
}

int main(int argc, char *argv[])
{
	pname = argv[0];
	// Init SDL video subsystem
	if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {
		
        fprintf(stderr, "Couldn't initialize SDL: %s\n",
			SDL_GetError());
		exit(1);
	}

    // Set GL context attributes
    initAttributes ();
    
    // Create GL context
    createSurface (0);
	
    // Init GL state
    initGL ();
    
    // Draw, get events...
    mainLoop ();
    
    // Cleanup
	SDL_Quit();
	
    return 0;
}
