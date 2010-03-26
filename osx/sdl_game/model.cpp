#include "model.h"

bool loadModel(char *filename, myVertex* &vertices, int &num_verts)
{
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	if(!fp) return false;
	
	int _nverts = -1;
	int _nfaces = -1;
	
	fscanf(fp, "%i", &_nverts);
	if(_nverts < 1)
	{
		fclose(fp);
		return false;
	}
	
	myVertex *verts = new myVertex[_nverts];
	
	for (int i = 0; i<_nverts; i++) {
		fscanf(fp, "%f %f %f %f %f %f", &verts[i].x, &verts[i].y, &verts[i].z, &verts[i].nx, &verts[i].ny, &verts[i].nz);
	}
	
	fscanf(fp, "%i", &_nfaces);
	if(_nfaces < 1)
	{
		delete [] verts;
		fclose(fp);
		return false;
	}
	
	vertices = new myVertex[_nfaces * 3];
	num_verts = _nfaces * 3;
	
	for (int i = 0; i < _nfaces; i++) {
		int x,y,z;
		fscanf(fp, "%i %i %i", &x, &y, &z);
		vertices[i*3 + 0] = verts[x];
		vertices[i*3 + 1] = verts[y];
		vertices[i*3 + 2] = verts[z];
	}
	
	fclose(fp);
	return true;
}

Model::~Model()
{
	glDeleteBuffers(NUM_BUFFERS, &buffer_name);
	if(this->num_verts>0)
		delete [] this->vertices;
	this->num_verts = 0;
}

bool Model::loadModel(char *filename)
{
	if (!::loadModel(filename, vertices, num_verts)) 
	{
		std::cout << "Could not load model file" << std::endl;
		return false;
	}
	
	glGenBuffers(NUM_BUFFERS, &buffer_name);
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_name);
	glBufferData(GL_ARRAY_BUFFER, sizeof(myVertex)*num_verts, this->vertices, GL_STATIC_COPY);
	return true;
}

void Model::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_name);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(myVertex), 0);
	
	//glEnableVertexAttribArray(0);
	glVertexPointer(3, GL_FLOAT, sizeof(myVertex), NULL);
	glNormalPointer(GL_FLOAT, sizeof(myVertex), (unsigned char*)(NULL) + sizeof(float)*3);
	
	GLfloat material_Ka[] = {0.1f, 0.1f, 0.0f, 1.0f};
	GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
	GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
	GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
	GLfloat material_Se = 20.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
	
	
	glDrawArrays(GL_TRIANGLES, 0, this->num_verts);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}