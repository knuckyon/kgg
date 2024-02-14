//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include "Model.h"

//ASSIMP
#include <assimp/config.h>

// Define these only in *one* .cc file.
#include "tinygltf/tiny_gltf.h"


const unsigned int width = 1920;
const unsigned int height = 1080;

int main()
{
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "Red Room", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("Resourses/Shaders/vertex.txt", "Resourses/Shaders/fragment.txt");

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);//
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);//!!!
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	//std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	//std::string modelPath0 = "Resourses/Models/curtains/scene.gltf";//hardcode
	//std::string modelPath1 = "Resourses/Models/floor/scene.gltf";//hardcode
	//std::string modelPath2 = "Resourses/Models/sofa/scene.gltf";//hardcode
	//std::string modelPath3 = "Resourses/Models/venera/scene.gltf";//hardcode
	//std::string modelPath4 = "Resourses/Models/floor_lamp/scene.gltf";//hardcode
	//std::string modelPath5 = "Resourses/Models/owl/scene.gltf";//hardcode
	//std::string modelPath6 = "Resourses/Models/rick_owens/scene.gltf";//hardcode



	/*std::vector<std::string> modelPath;

	modelPath.push_back("Resourses/Models/curtains/scene.gltf");
	modelPath.push_back("Resourses/Models/floor/scene.gltf");

	modelPath.push_back("Resourses/Models/sofa/scene.gltf");
	modelPath.push_back("Resourses/Models/venera/scene.gltf");
	modelPath.push_back("Resourses/Models/floor_lamp/scene.gltf");
	modelPath.push_back("Resourses/Models/owl/scene.gltf");
	modelPath.push_back("Resourses/Models/rick_owens/scene.gltf");
	modelPath.push_back("Resourses/Models/curtains/scene.gltf");
	
	modelPath.push_back("Resourses/Models/curtains/scene.gltf");
	modelPath.push_back("Resourses/Models/curtains/scene.gltf");

	modelPath.push_back("Resourses/Models/curtains/scene.gltf");
	modelPath.push_back("Resourses/Models/curtains/scene.gltf");

	modelPath.push_back("Resourses/Models/curtains/scene.gltf");
	modelPath.push_back("Resourses/Models/curtains/scene.gltf");

	modelPath.push_back("Resourses/Models/armchair/scene.gltf");*/

	typedef struct Path_Trans
	{
		std::string path;
		Transform transform;

		Path_Trans(std::string path, Transform transform)
		{
			Path_Trans::path = path;
			Path_Trans::transform = transform;
		}
	} Path_Trans;

	std::vector<Path_Trans> modelPT;
	const GLfloat curtH = 1.7f;

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

	std::vector<Model*> modelObj;

	for (auto it = modelPT.begin(); it != modelPT.end(); it++)
	{
		//auto temp = new Model(it->path.c_str());
		modelObj.push_back(new Model(it->path.c_str()));
	}



	//const unsigned int N_MODELS = 14;//!!!!
	//std::string modelPath[N_MODELS] = { "0" };

	////parsing from file
	//modelPath[0] = "Resourses/Models/curtains/scene.gltf";
	//modelPath[1] = "Resourses/Models/floor/scene.gltf";
	//modelPath[2] = "Resourses/Models/sofa/scene.gltf";
	// 
	//modelPath[3] = "Resourses/Models/venera/scene.gltf";
	//modelPath[4] = "Resourses/Models/floor_lamp/scene.gltf";
	//modelPath[5] = "Resourses/Models/owl/scene.gltf";
	//modelPath[6] = "Resourses/Models/rick_owens/scene.gltf";
	//modelPath[7] = "Resourses/Models/curtains/scene.gltf";
	//
	//modelPath[8] = "Resourses/Models/curtains/scene.gltf";
	//modelPath[9] = "Resourses/Models/curtains/scene.gltf";

	//modelPath[10] = "Resourses/Models/curtains/scene.gltf";
	//modelPath[11] = "Resourses/Models/curtains/scene.gltf";

	//modelPath[12] = "Resourses/Models/curtains/scene.gltf";
	//modelPath[13] = "Resourses/Models/curtains/scene.gltf";

	//modelPath[14] = "Resourses/Models/armchair/scene.gltf";
	

	/*std::array <Model*, N_MODELS> modelObj;

	for (int i = 0; i < N_MODELS; i++)
	{
		if (modelPath[i] != "0")
		{
			auto temp = new Model(modelPath[i].c_str());
			modelObj[i] = { temp };
		} 
	}*/


	//// Load in a model
	//Model model0((modelPath0).c_str());
	//Model model0_1((modelPath0).c_str());
	////Model model0_2((modelPath0).c_str());
	////Model model0_3((modelPath0).c_str());

	//Model model1((modelPath1).c_str());
	//Model model2((modelPath2).c_str());
	//Model model3((modelPath3).c_str());
	//Model model4((modelPath4).c_str());
	//Model model5((modelPath5).c_str());
	//Model model6((modelPath6).c_str());

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
		

		auto i = std::make_pair(modelObj.begin(), modelPT.begin());
		for (; i.first != modelObj.end(); ++i.first, ++i.second)
		{
			(*i.first)->Draw(shaderProgram, camera, (*i.second).transform);
		}

		//Transform moving[N_MODELS];
		//const GLfloat curtH = 1.7f;
		////std::map <std::string, Transform> moving;

		//// curtains r-u
		//moving[0].scale(glm::vec3(curtH, 0.5f, 1.1f));
		//glm::quat axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::quat axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[0].rotate(axisY * axisX);
		//moving[0].translate(glm::vec3(2.0f, 0.0f, 0.0f));

		////floor 
		//moving[1].scale(glm::vec3(0.03f, 0.03f, 0.03f));
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//moving[1].rotate(axisY);
		//moving[1].translate(glm::vec3(0.0f, 3.0f, 0.0f));

		////sofa-large
		//moving[2].scale(glm::vec3(0.85f, 0.85f, 0.85f));
		//axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//axisY = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//moving[2].rotate(axisX * axisY);
		//moving[2].translate(glm::vec3(-30.0f, 0.0f, -85.0f));

		////venus
		//moving[3].scale(glm::vec3(5.5f, 5.5f, 5.5f));
		//axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//axisY = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//moving[3].rotate(axisX * axisY);
		//moving[3].translate(glm::vec3(-30.0f, 0.0f, -40.0f));

		////lamp
		//moving[4].scale(glm::vec3(0.011f, 0.011f, 0.011f));
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//moving[4].rotate(axisY);
		//moving[4].translate(glm::vec3(0.01f, 0.01f, 1.7f));

		////owl
		//moving[5].scale(glm::vec3(1.5f, 1.5f, 1.5f));
		//axisY = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[5].rotate(axisX * axisY);
		//moving[5].translate(glm::vec3(525.0f, -75.0f, -590.0f));

		////sofa small
		//moving[6].scale(glm::vec3(0.05f, 0.05f, 0.05f));
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[6].rotate(axisY * axisX);
		//moving[6].translate(glm::vec3(15.0f, -3.5f, -0.55f));

		////curtains r-d
		//moving[7].scale(glm::vec3(curtH, 0.5f, 1.1f));
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[7].rotate(axisY * axisX);
		//moving[7].translate(glm::vec3(-1.4f, 0.05f, 0.0f));

		////curtains u-r
		//moving[8].scale(glm::vec3(curtH, 0.8f, 1.4f));//
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[8].rotate(axisY * axisX);
		//moving[8].translate(glm::vec3(3.7f, -2.3f, 0.0f));

		////curtains d
		//moving[9].scale(glm::vec3(curtH, 0.8f, 1.4f));//
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[9].rotate(axisY* axisX);
		//moving[9].translate(glm::vec3(-3.3f, -2.0f, 0.0f));

		////curtains l-d
		//moving[10].scale(glm::vec3(curtH, 0.5f, 1.1f));
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[10].rotate(axisY * axisX);
		//moving[10].translate(glm::vec3(-1.4f, -6.6f, 0.0f));

		//// curtains l-u
		//moving[11].scale(glm::vec3(curtH, 0.5f, 1.1f));
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[11].rotate(axisY * axisX);
		//moving[11].translate(glm::vec3(2.0f, -6.625f, 0.0f));

		////curtains u-l
		//moving[12].scale(glm::vec3(curtH, 0.8f, 1.4f));//
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[12].rotate(axisY * axisX);
		//moving[12].translate(glm::vec3(3.7f, -5.3f, 0.0f));

		////curtains d-l
		//moving[13].scale(glm::vec3(curtH, 0.8f, 1.4f));//
		//axisY = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//axisX = glm::angleAxis(glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//moving[13].rotate(axisY* axisX);
		//moving[13].translate(glm::vec3(-3.3f, -5.0f, 0.0f));

		//table
		


	/*	int i = 0;
		for (auto obj: modelObj)
		{
			obj->Draw(shaderProgram, camera, moving[i++]);
		}*/


		//modelObj[0]->Draw(shaderProgram, camera, moving[0]);//ct

		//modelObj[1]->Draw(shaderProgram, camera, moving[1]);//floor
		////modelObj[2]->Draw(shaderProgram, camera, moving[2]);
		////modelObj[3]->Draw(shaderProgram, camera, moving[3]);
		////modelObj[4]->Draw(shaderProgram, camera, moving[4]);
		////modelObj[5]->Draw(shaderProgram, camera, moving[5]);
		////modelObj[6]->Draw(shaderProgram, camera, moving[6]);
		////modelObj[7]->Draw(shaderProgram, camera, moving[7]);//ct

		////modelObj[8]->Draw(shaderProgram, camera, moving[8]);//ct
		////modelObj[9]->Draw(shaderProgram, camera, moving[9]);//ct

		////modelObj[10]->Draw(shaderProgram, camera, moving[10]);//ct
		////modelObj[11]->Draw(shaderProgram, camera, moving[11]);//ct

		////modelObj[12]->Draw(shaderProgram, camera, moving[12]);//ct
		////modelObj[13]->Draw(shaderProgram, camera, moving[13]);//ct
		//modelObj[14]->Draw(shaderProgram, camera, moving[13]);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}