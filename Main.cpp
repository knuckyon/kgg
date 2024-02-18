//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------


#include "globals.h"
#include "Model.h"
#include "Func.h"

//4:3
//const unsigned int width = 1024;
//const unsigned int height = 768;

bool isRotateStatue = false;
char directionStatue = 0;

bool isFlying = false;

glm::vec3 owlPos;
GLfloat owlShift = 13.0f;

LightsInfo lights;
// фоновый источник света
DirLight dirLight{};
// точечный источник света
PointLight pointLight{};

int main()
{
	// иниц GLFW
	glfwInit();
	//версия
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Black Wigwam", NULL, NULL);
	// окно - проверка создания
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ОКНО
	glfwSetKeyCallback(window, key_callback); // привязываем функция обработчика нажатий
	glfwMakeContextCurrent(window);

	//загрузка GLAD
	gladLoadGL();

	glViewport(0, 0, width, height);

	// ОБЪЕКТ КЛАССА ШЕЙДЕРОВ
	//Shader shaderProgram("Resourses/Shaders/vertex.txt", "Resourses/Shaders/fragment.txt");
	Shader shaderProgram("Resourses/Shaders/vertex.txt", "Resourses/Shaders/fragment.txt");

	// СВЕТОВАЯ МОДЕЛЬ
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);//!!!glm::vec3 lightPos = glm::vec3(0.0f, 0.2f, 0.0f)
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	lights.numPLights = 0;
	lights.numSLights = 0;

	// фоновый источник света
	dirLight.ambient = glm::vec3(0.9f);
	dirLight.diffuse = glm::vec3(0.15f);
	dirLight.specular = glm::vec3(0.1f);
	dirLight.direction = glm::vec3(0.5f, 0.5f, 0.5f);

	lights.dirLight = dirLight;

	shaderProgram.Activate();
	//glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//ГЛУБИНА
	glEnable(GL_DEPTH_TEST);
	
	//КАМЕРА
	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 2.0f));


	//структура с именем файла и преобразованиями на мировые координаты 


	//вектор 
	std::vector<PathTransMat> modelPT;
	//магическое число для занавесок
	const GLfloat curtH = 1.7f;


	//ЗАГРУЗКА В ВЕКТОР С НАЗВАНИЯМИ ФАЙЛОВ
	Transform moving;
	Material material;

	glm::quat axisY;
	glm::quat axisX;

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelDir = "Resourses/Models/";

	//sky
	axisX = glm::angleAxis(glm::radians(110.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	axisY = glm::angleAxis(glm::radians(-75.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	moving.transVec = glm::vec3(-5.0f, 8.0f, -10.0f);
	moving.rotQuat = axisX * axisY;
	moving.scaVec = glm::vec3(1.0f, 1.0f, 1.0f);

	modelPT.push_back(PathTransMat("moon/scene.gltf", moving, material));

	//curtains
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(2.0f, 0.0f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.5f, 1.1f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	////floor-pearl
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	moving.transVec = glm::vec3(0.0f, 3.0f, 0.0f);
	moving.rotQuat = axisY;
	moving.scaVec = glm::vec3(0.03f, 0.03f, 0.03f);

	material.Pearl();

	modelPT.push_back(PathTransMat("floor/scene.gltf", moving, material));

	material.Clear();
	////sofa
	moving.Clear();
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));//180.0f
	axisY = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//-90.0f
	
	moving.transVec = glm::vec3(-30.0f, 0.0f, -85.0f);
	moving.rotQuat = axisX * axisY;
	moving.scaVec = glm::vec3(0.85f, 0.85f, 0.85f);

	modelPT.push_back(PathTransMat("sofa/scene.gltf", moving, material));

	//////venera
	moving.Clear();
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	axisY = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	moving.transVec = glm::vec3(-30.0f, 0.0f, -40.0f);
	moving.rotQuat = axisX * axisY;
	moving.scaVec = glm::vec3(5.5f, 5.5f, 5.5f);

	//
	modelPT.push_back(PathTransMat("venera/scene.gltf", moving, material));
	//auto it_statue = modelPT.back();

	//////floor_lamp
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	moving.transVec = glm::vec3(0.01f, 0.01f, 1.7f);
	moving.rotQuat = axisY;
	moving.scaVec = glm::vec3(0.011f, 0.011f, 0.011f);

	modelPT.push_back(PathTransMat("floor_lamp/scene.gltf", moving, material));

	//////owl
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	owlPos = glm::vec3(525.0f, -75.0f, -590.0f);

	moving.transVec = owlPos;
	moving.rotQuat = axisX * axisY;
	moving.scaVec = glm::vec3(1.5f, 1.5f, 1.5f);

	modelPT.push_back(PathTransMat("owl/scene.gltf", moving, material));

	//////rick_owens
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(15.0f, -3.5f, -0.55f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(0.05f, 0.05f, 0.05f);

	modelPT.push_back(PathTransMat("rick_owens/scene.gltf", moving, material));

	//////curtains r-d
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(-1.4f, 0.05f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.5f, 1.1f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	////////curtains u-r
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(3.7f, -2.3f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.8f, 1.4f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	////////curtains d
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(-3.3f, -2.0f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.8f, 1.4f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	////////curtains l-d
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(-1.4f, -6.6f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.5f, 1.1f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	//////// curtains l-u
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(2.0f, -6.625f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.5f, 1.1f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving,material));

	////////curtains u-l
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(3.7f, -5.3f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.8f, 1.4f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	////////curtains d-l
	moving.Clear();
	axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	axisX = glm::angleAxis(glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	moving.transVec = glm::vec3(-3.3f, -5.0f, 0.0f);
	moving.rotQuat = axisY * axisX;
	moving.scaVec = glm::vec3(curtH, 0.8f, 1.4f);

	modelPT.push_back(PathTransMat("curtains/scene.gltf", moving, material));

	//ВЕКТОР С ОБЪЕКТАМИ МОДЕЛЕЙ
	std::vector<Model*> modelObj;

	//СЛОВАРЬ С ПУТЯМИ И ССЫЛКАМИ НА МОДЕЛЬ
	//std::map<std::string, Model*> dublicates;
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
		Model* tempModel = new Model((modelDir + it->path).c_str());
		tempModel->setMaterial(it->material);
		modelObj.push_back(tempModel);
	}

	// ИГРОВОЙ ЦИКЛ

	double startTime = glfwGetTime();

	modelRotations statue;

	ParabConst owlParab = findParabComponents(owlPos, glm::vec3(-125.0f, -850.0f, owlPos.z), glm::vec3(-500.0f, -700.0f, owlPos.z));
	//owlParab.a = -owlParab.a;

	GLfloat owlX = owlPos.x;
	std::thread thx;
	while (!glfwWindowShouldClose(window))
	{
		//angle = (GLfloat)glfwGetTime() * 0.2f;
		// задний фон
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);//
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);//glClearColor(0.07f, 0.07f, 0.07f, 1.0f)
		// очитска буф цвета и глубины
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// назвачение выводов с окна на камеру
		camera.Inputs(window);
		// перепесктива
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		//ОТРИСОВКА ОБЪЕКТОВ МОДЕЛЕЙ НА СЦЕНЕ
		auto i = std::make_pair(modelObj.begin(), modelPT.begin());

		for (; i.first != modelObj.end(); ++i.first, ++i.second)
		{
			//обработка анимаций выбранных объектов
			if ((*i.second).path == "venera/scene.gltf")
			{
				if (isRotateStatue && !directionStatue) //против часовой - 0
				{
					(*i.second).transform.rotQuat *= statue.CW;
					//(*i.second).transform = it_statue.transform;
				}

				else if (isRotateStatue && directionStatue) //по часовой - 1
				{
					(*i.second).transform.rotQuat *= statue.CC;
					//(*i.second).transform = it_statue.transform;
				}
			}

			if ((*i.second).path == "owl/scene.gltf")
			{
				if (isFlying)
				{
					//std::thread xThread(cycleOX, std::ref(owlPos.x), -5.0f, 300.0f);
					//std::thread yThread(cycleOY, std::ref(owlPos.y), std::ref(owlPos.x), owlParab);
					////открыть поток приближения x
					////
					//xThread.detach();
					//yThread.detach();
					thx = std::thread(cycleOX,std::ref(owlPos.x), std::ref(owlPos.y), owlParab, -6.0f, -1500.f); //закрыть поток -400.f
					//std::thread thy(cycleOY, std::ref(owlPos.y), std::ref(owlPos.x), owlParab);
					//cycleOY(owlPos.y, owlPos.x, owlParab);
					
					thx.detach();
					//thy.detach();
				}
				(*i.second).transform.transVec = owlPos;
			}
			//else
			(*i.first)->Draw(shaderProgram, camera, (*i.second).transform,lights);
			//std::cout << (*i.second).path << std::endl;
			//thx.join();
		}
		// подгрузка переднего и зданего буфера
		//thx.~thread();
		glfwSwapBuffers(window);
		//ПРОВЕРКА СОБЫТИЙ
		glfwPollEvents();
	}
	thx.~thread();
	//thx.join();
	//УДАЛЕНИЕ ОЧИСТКА ПАМЯТИ

	shaderProgram.Delete();
	modelObj.erase(modelObj.begin(), modelObj.end());
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}