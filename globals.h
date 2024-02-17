#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <json/json.hpp>
#include<stb/stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


#include <string>

#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>

#include <chrono>
#include <cerrno>

#include <vector>
#include <set>
#include <algorithm>

constexpr auto width = 1024;
constexpr auto height = 768;

extern bool isRotateStatue;
extern char directionStatue;

extern bool isFlying;

extern glm::vec3 owlPos;
extern GLfloat owlShift;


//структура для вершин использ в сетке
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};
typedef struct Transform
{
	glm::mat4 transMat;
	glm::mat4 rotMat;
	glm::mat4 scaMat;

	glm::vec3 transVec;
	glm::quat rotQuat;
	glm::vec3 scaVec;

	Transform()
	{
		transMat = glm::mat4(1.0f);
		rotMat = glm::mat4(1.0f);
		scaMat = glm::mat4(1.0f);

		transVec = glm::vec3(0.0f, 0.0f, 0.0f);
		rotQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		scaVec = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void Clear()
	{
		transMat = glm::mat4(1.0f);
		rotMat = glm::mat4(1.0f);
		scaMat = glm::mat4(1.0f);

		transVec = glm::vec3(0.0f, 0.0f, 0.0f);
		rotQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		scaVec = glm::vec3(1.0f, 1.0f, 1.0f);
	}
} Transform;

typedef struct modelRotations
{
	GLfloat angle = 0.4f;
	glm::quat CW = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat CC = glm::angleAxis(glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
}modelRotations;

struct Material
{
    glm::vec3 ka; // коэф. фонового отражения (цвет фонового освещения)
    glm::vec3 kd; // коэф. диффузного отражения (цвет объекта)
    glm::vec3 ks; // коэф. зеркального блика
    GLfloat shininess; // показатель глянцевости

    // Значения по умолчанию
    Material()
    {
        ka = glm::vec3(0.0215, 0.1745, 0.0215);
        kd = glm::vec3(0.07568, 0.61424, 0.07568);
        ks = glm::vec3(0.633, 0.727811, 0.633);
        shininess = 128 * 0.6;
    };
};

// Структура с описанием параметров фонового освещения
struct DirLight
{
    // Направление
    glm::vec3 direction;

    // Параметры света
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
extern DirLight dirLight;

// Структура с описанием точечного источника света
struct PointLight
{
    // Координаты
    glm::vec3 position;

    // Затухание
    float constant;
    float linear;
    float quadratic;

    // Параметры света
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
extern PointLight pointLight;

// Структура с описанием направленного источника света
struct SpotLight
{
    //Местоположение и направление
    glm::vec3 position;
    glm::vec3 direction;

    //Радиус луча
    float cutOff;
    float outerCutOff;

    // Затухание
    float constant;
    float linear;
    float quadratic;

    // Параметры света
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
extern SpotLight spotLight;

// Структура с описанием источников света на сцене
struct LightsInfo
{
    /// Параметры фонового освещения
    DirLight dirLight;

    // Массив с точечными источниками света
    std::vector<PointLight> pointLights;

    // Массив с направленными источниками света
    std::vector<SpotLight> spotLights;

    //Количество точечных и направленных источников света
    int numPLights;
    int numSLights;
};
extern LightsInfo lights;
