#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include"shaderClass.h"
#include "globals.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	// присв текст слоту текстуру
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	void Delete();
};
#endif