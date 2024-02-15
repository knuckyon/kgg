#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// векторы камеры
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// старая реализация предотвращения первого клика
	bool firstClick = true;

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 100.0f;

	//контсруктор
	Camera(int width, int height, glm::vec3 position);

	// обновление матрицы камеры в верш шейд
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// загр матрицы камеры в шейд
	void Matrix(Shader& shader, const char* uniform);
	// управление камерой периферией
	void Inputs(GLFWwindow* window);
};
#endif