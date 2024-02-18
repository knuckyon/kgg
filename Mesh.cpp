#include "Mesh.h"

using namespace std;

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	// ген VBO и линковка к верш
	VBO VBO(vertices);
	// ген EBO и линковка к верш
	EBO EBO(indices);
	// VBO данные в VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	//отбиндить
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw
(
	Shader& shader,
	Camera& camera,
	Transform moving,
	LightsInfo lights,
	Material material,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	//активация в объекта класса шейдеров, в кот будут передаваться unifowm переменные
	shader.Activate();
	VAO.Bind();

	// учет текстур
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{

			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	// матрциа камеры учитывать
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	//Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);//!!!!!!

	// Transform the matrices to their correct form
	/*trans = glm::translate(trans, moving.transVec);
	rot = glm::mat4_cast(moving.rotQuat);
	sca = glm::scale(sca, moving.scaVec);*/

	moving.transMat = glm::translate(trans, moving.transVec);
	moving.rotMat = glm::mat4_cast(moving.rotQuat);
	moving.scaMat = glm::scale(sca, moving.scaVec);

	//матрицы в верш шейдер
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(moving.transMat));//!moving.transMat
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(moving.rotMat));//@moving.rotMat
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(moving.scaMat));//!moving.scaMat
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));


	//Сначала фоновый
	GLuint la = glGetUniformLocation(shader.ID, "lights.dirLight.ambient");
	glUniform3fv(la, 1, &lights.dirLight.ambient[0]);
	GLuint ld = glGetUniformLocation(shader.ID, "lights.dirLight.diffuse");
	glUniform3fv(ld, 1, &lights.dirLight.diffuse[0]);
	GLuint ls = glGetUniformLocation(shader.ID, "lights.dirLight.specular");
	glUniform3fv(ls, 1, &lights.dirLight.specular[0]);
	GLuint ldir = glGetUniformLocation(shader.ID, "lights.dirLight.direction");
	glUniform3fv(ldir, 1, &lights.dirLight.direction[0]);

	//Теперь точечные
	GLuint lnp = glGetUniformLocation(shader.ID, "lights.numPLights");
	glUniform1i(lnp, lights.numPLights);
	for (int i = 0; i < lights.numPLights; i++)
	{
		GLuint lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].ambient").c_str());
		glUniform3fv(lp, 1, &lights.pointLights[i].ambient[0]);
		lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].diffuse").c_str());
		glUniform3fv(lp, 1, &lights.pointLights[i].diffuse[0]);
		lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].specular").c_str());
		glUniform3fv(lp, 1, &lights.pointLights[i].specular[0]);
		lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].position").c_str());
		glUniform3fv(lp, 1, &lights.pointLights[i].position[0]);

		lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].constant").c_str());
		glUniform1f(lp, lights.pointLights[i].constant);
		lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].linear").c_str());
		glUniform1f(lp, lights.pointLights[i].linear);
		lp = glGetUniformLocation(shader.ID, ("lights.pointLights[" + to_string(i) + "].quadratic").c_str());
		glUniform1f(lp, lights.pointLights[i].quadratic);
	}

	//И направленные
	GLuint lns = glGetUniformLocation(shader.ID, "lights.numSLights");
	glUniform1i(lns, lights.numSLights);

	for (int i = 0; i < lights.numSLights; i++)
	{
		GLuint lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].ambient").c_str());
		glUniform3fv(lsp, 1, &lights.spotLights[i].ambient[0]);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].diffuse").c_str());
		glUniform3fv(lsp, 1, &lights.spotLights[i].diffuse[0]);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].specular").c_str());
		glUniform3fv(lsp, 1, &lights.spotLights[i].specular[0]);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].position").c_str());
		glUniform3fv(lsp, 1, &lights.spotLights[i].position[0]);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].direction").c_str());
		glUniform3fv(lsp, 1, &lights.spotLights[i].direction[0]);


		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].cutOff").c_str());
		glUniform1f(lsp, lights.spotLights[i].cutOff);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].outerCutOff").c_str());
		glUniform1f(lsp, lights.spotLights[i].outerCutOff);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].constant").c_str());
		glUniform1f(lsp, lights.spotLights[i].constant);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].linear").c_str());
		glUniform1f(lsp, lights.spotLights[i].linear);
		lsp = glGetUniformLocation(shader.ID, ("lights.spotLights[" + to_string(i) + "].quadratic").c_str());
		glUniform1f(lsp, lights.spotLights[i].quadratic);
	}

	 //Загрузка ссылки на текстуру в шейдер
	GLuint mtx = glGetUniformLocation(shader.ID, "material.texture");
	glUniform1i(mtx, 0);

	GLuint ms = glGetUniformLocation(shader.ID, "material.specularmap");
	glUniform1i(ms, 1);

	//Активация текстур
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	// И от материала всё равно нужен уровень зеркальности
	GLuint msh = glGetUniformLocation(shader.ID, "material.shininess");
	glUniform1f(msh, material.shininess);
	//отрисовать текущей сетки
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}