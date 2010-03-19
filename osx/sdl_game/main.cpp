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

int gheight, gwidth;
int last_time, new_time;
Model myModel1;
HeightMap hm;
static SDL_Surface *gScreen;

static int mtime(void)
{
	return SDL_GetTicks();
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
	angle = 1.0;
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
	last_time = mtime();
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
	char num_str[128];
	new_time = mtime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glTranslatef(0.0, 0.0f, -10.0f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(angle + (120 * SDL_GetTicks() / 1000.0), 0.0f, 1.0f, 0.0f);
	//hm.render();
	myModel1.render();
	glPopMatrix();
	
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, gwidth, 0, gheight, -10.0, 10.0);
	glRasterPos2f(5.0, 5.0);
	
	
	
	
	sprintf(num_str, "%0.2f Hz, %dx%d", 1 / ((new_time - last_time) / 1000.0), gwidth, gheight);
	DrawStr(num_str);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
	
	last_time = new_time;
	glFlush();
}

static void mainLoop ()
{
    SDL_Event event;
    int done = 0;
    
    while ( !done ) {		
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
    
        drawGL ();
        SDL_GL_SwapBuffers ();
	}
}

int main(int argc, char *argv[])
{
	pname = argv[0];
	if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {
		
        fprintf(stderr, "Couldn't initialize SDL: %s\n",
			SDL_GetError());
		exit(1);
	}

    initAttributes ();
    createSurface (0);
    initGL ();
    mainLoop ();
	SDL_Quit();
	
    return 0;
}
