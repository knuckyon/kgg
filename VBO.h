#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "globals.h"

class VBO
{
public:
	//номер VBO
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
