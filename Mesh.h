#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

typedef struct Transform
{
	glm::mat4 transMat;
	glm::mat4 rotMat;
	glm::mat4 scaMat;

	Transform()
	{
		transMat = glm::mat4(1.0f);
		rotMat = glm::mat4(1.0f);
		scaMat = glm::mat4(1.0f);

	}

	void Clear()
	{
		transMat = glm::mat4(1.0f);
		rotMat = glm::mat4(1.0f);
		scaMat = glm::mat4(1.0f);
	}

	void translate(glm::vec3 trans)
	{
		transMat = glm::translate(transMat, trans);
	}

	void rotate(glm::quat rot)
	{
		rotMat = glm::mat4_cast(rot);
	}

	void scale(glm::vec3 sca)
	{
		scaMat = glm::scale(scaMat, sca);
	}
} Transform;

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	VAO VAO;

	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	void Draw
	(
		Shader& shader,
		Camera& camera,
		Transform moving,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};
#endif
