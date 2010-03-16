#include "SDL.h"
#include "SDL_opengl.h"

#include "model.h"
#include "glsl.h"

float angle = 0.0f;
float ypos = 0.0f;
float ydir = 0.05f;

Model myModel1;

static SDL_Surface *gScreen;

static void initAttributes ()
{
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
}

int resizeWindow( int width, int height )
{
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
    
    gScreen = SDL_SetVideoMode (640, 480, 0, flags);
    resizeWindow( 640, 480 );
	if (gScreen == NULL) 
	{
		fprintf (stderr, "Couldn't set 640x480 OpenGL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
}

static void initGL ()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	GLuint prog_id = createGLSLProgram("simple.vert", "simple.frag");
	glUseProgram(prog_id);
	if(!myModel1.loadModel("teapotn.txt")) {
		std::cout << "Could not load model file" << std::endl;
		return;
	}
}

static void drawGL ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	glScalef(0.09f, 0.09f, 0.09f);
	glTranslatef(0.0, 0.0f, -20.0f);
	glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	
	glRotatef(180.0f, 1.0f, 0.0f, 1.0f);
	
	//glRotatef(angle, 0.0f, 0.0f, 1.0f);
	myModel1.render();
	glPopMatrix();
	glFlush();
}

static void mainLoop ()
{
    SDL_Event event;
    int done = 0;
    int fps = 24;
	int delay = 1000/fps;
    int thenTicks = -1;
    int nowTicks;
    
    while ( !done ) {
		angle += 5.0f;
		if(angle > 360) {
			angle -= 360;
		}
		
		ypos += ydir;
		if(ypos > 3.0f || ypos < -3.0f) ydir *= -1;
		/* Check for events */
		while ( SDL_PollEvent (&event) ) {
			switch (event.type) {

				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					/* Any keypress quits the app... */
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
