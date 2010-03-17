#include "height_map.h"
#include "SDL_image.h"
HeightMap::~HeightMap()
{
	
}

Uint32 HeightMap::GetPixel(int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
    switch(bpp) {
		case 1:
			return *p;
			
		case 2:
			return *(Uint16 *)p;
			
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			
		case 4:
			return *(Uint32 *)p;
			
		default:
			return 0;       /* shouldn't happen, but avoids warnings */
    }
}

//  (w-1)*(h-1)*2 triangles
void HeightMap::load(char *filename)
{
	surface = IMG_Load(filename);
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	map = (GLfloat *)malloc(surface->w * surface->h * sizeof(GLfloat));
	
	Uint8 r1, g1, b1, a1;
	height = surface->h;
	width = surface->w;
	for (int i = 0; i < surface->h; i++) {
		for (int j = 0; j < surface->w; j++) {
			SDL_GetRGBA(GetPixel(j, i), surface->format, &r1, &g1, &b1, &a1);
			map[i * surface->w + j] = float(r1) / 256.0;
		}
	}
	
	SDL_FreeSurface(surface);
}

void HeightMap::render()
{
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, id);
	for(int z = 0; z < height-1; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < width; x++) {
			//glTexCoord2d(GLfloat(x) / width, GLfloat(z) / height);
			glVertex3f(x, map[(z+1) * width + x], z+1);
			glVertex3f(x, map[z * width + x], z);
		}
		glEnd();
	}
}