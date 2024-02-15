//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include "Model.h"
#include <set>

//ASSIMP
#include <assimp/config.h>

// Define these only in *one* .cc file.
#include "tinygltf/tiny_gltf.h"

//4:3
const unsigned int width = 1024;
const unsigned int height = 768;

int main()
{
	// ���� GLFW
	glfwInit();
	//������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Black Wigwam", NULL, NULL);
	// ���� - �������� ��������
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ����
	glfwMakeContextCurrent(window);

	//�������� GLAD
	gladLoadGL();

	glViewport(0, 0, width, height);

	// ������ ������ ��������
	Shader shaderProgram("Resourses/Shaders/vertex.txt", "Resourses/Shaders/fragment.txt");

	// �������� ������
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//
	glm::vec3 lightPos = glm::vec3(0.5f, 6.5f, 0.5f);//!!!
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//�������
	glEnable(GL_DEPTH_TEST);
	
	//������
	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 2.0f));


	//��������� � ������ ����� � ���������������� �� ������� ���������� 
	typedef struct Path_Trans
	{
		std::string path;
		Transform transform;
		//Model* pModel;

		Path_Trans(std::string path, Transform transform)
		{
			Path_Trans::path = path;
			Path_Trans::transform = transform;
			//Path_Trans::pModel = NULL;
		}
	} Path_Trans;

	//������ 
	std::vector<Path_Trans> modelPT;
	//���������� ����� ��� ���������
	const GLfloat curtH = 1.7f;


	//�������� � ������ � ���������� ������
	//curtains
	Transform moving;

	moving.Clear();
	moving.scale(glm::vec3(curtH, 0.5f, 1.1f));
	glm::quat axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY* axisX);
	moving.translate(glm::vec3(2.0f, 0.0f, 0.0f));

	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	//floor
	moving.Clear();
	moving.scale(glm::vec3(0.03f, 0.03f, 0.03f));
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	moving.rotate(axisY);
	moving.translate(glm::vec3(0.0f, 3.0f, 0.0f));

	modelPT.push_back(Path_Trans("Resourses/Models/floor/scene.gltf", moving));


	//sofa
	moving.Clear();
	moving.scale(glm::vec3(0.85f, 0.85f, 0.85f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	axisY = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	moving.rotate(axisX * axisY);
	moving.translate(glm::vec3(-30.0f, 0.0f, -85.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/sofa/scene.gltf", moving));

	////venera
	 moving.Clear();
	moving.scale(glm::vec3(5.5f, 5.5f, 5.5f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	axisY = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	moving.rotate(axisX * axisY);
	moving.translate(glm::vec3(-30.0f, 0.0f, -40.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/venera/scene.gltf", moving));

	////floor_lamp
	 moving.Clear();
	moving.scale(glm::vec3(0.011f, 0.011f, 0.011f));
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	moving.rotate(axisY);
	moving.translate(glm::vec3(0.01f, 0.01f, 1.7f));
	modelPT.push_back(Path_Trans("Resourses/Models/floor_lamp/scene.gltf", moving));

	////owl
	 moving.Clear();
	moving.scale(glm::vec3(1.5f, 1.5f, 1.5f));
	axisY = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisX * axisY);
	moving.translate(glm::vec3(525.0f, -75.0f, -590.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/owl/scene.gltf", moving));

	////rick_owens
	 moving.Clear();
	moving.scale(glm::vec3(0.05f, 0.05f, 0.05f));
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY * axisX);
	moving.translate(glm::vec3(15.0f, -3.5f, -0.55f));
	modelPT.push_back(Path_Trans("Resourses/Models/rick_owens/scene.gltf", moving));

	////curtains r-d
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.5f, 1.1f));
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY * axisX);
	moving.translate(glm::vec3(-1.4f, 0.05f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	////curtains u-r
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.8f, 1.4f));//
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY * axisX);
	moving.translate(glm::vec3(3.7f, -2.3f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	////curtains d
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.8f, 1.4f));//
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY* axisX);
	moving.translate(glm::vec3(-3.3f, -2.0f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	////curtains l-d
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.5f, 1.1f));
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY * axisX);
	moving.translate(glm::vec3(-1.4f, -6.6f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	//// curtains l-u
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.5f, 1.1f));
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY * axisX);
	moving.translate(glm::vec3(2.0f, -6.625f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	////curtains u-l
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.8f, 1.4f));//
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY * axisX);
	moving.translate(glm::vec3(3.7f, -5.3f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	////curtains d-l
	 moving.Clear();
	moving.scale(glm::vec3(curtH, 0.8f, 1.4f));//
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	moving.rotate(axisY* axisX);
	moving.translate(glm::vec3(-3.3f, -5.0f, 0.0f));
	modelPT.push_back(Path_Trans("Resourses/Models/curtains/scene.gltf", moving));

	//������ � ��������� �������
	std::vector<Model*> modelObj;

	//������� � ������ � �������� �� ������
	std::map<std::string, Model*> dublicates;
	for (auto it = modelPT.begin(); it != modelPT.end(); it++)
	{
		//if (dublicates.count(it->path))
		//{
		//	//
		//	modelObj.push_back(new Model(*dublicates[it->path]));
		//}
		//else
		//{
		//	auto tempModel = new Model(it->path.c_str());
		//	modelObj.push_back(tempModel);
		//	dublicates.insert({ it->path, tempModel });
		//}

		modelObj.push_back(new Model(it->path.c_str()));
	}

	// ������� ����
	while (!glfwWindowShouldClose(window))
	{
		// ������ ���
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//
		glClearColor(0.07f, 0.07f, 0.07f, 1.0f);//
		// ������� ��� ����� � �������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ���������� ������� � ���� �� ������
		camera.Inputs(window);
		// ������������
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		//�������� �������� ������� �� �����
		auto i = std::make_pair(modelObj.begin(), modelPT.begin());
		for (; i.first != modelObj.end(); ++i.first, ++i.second)
		{
			(*i.first)->Draw(shaderProgram, camera, (*i.second).transform);
		}
		// ��������� ��������� � ������� ������
		glfwSwapBuffers(window);
		//�������� �������
		glfwPollEvents();
	}

	//�������� ������� ������

	shaderProgram.Delete();
	modelObj.erase(modelObj.begin(), modelObj.end());
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}