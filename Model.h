#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

//#include <json/json.hpp>
#include"Mesh.h"
#include "globals.h"

using json = nlohmann::json;


class Model
{
public:
	//�������� ������ �� �����, ���������� � 'data', 'JSON', � 'file'
	Model(const char* file);
	//����������� ������
	Model(const Model& original);

	void setMaterial(Material material);

	void Draw(Shader& shader, Camera& camera, Transform moving, LightsInfo lights);

private:
	const char* file;
	Material material;

	std::vector<unsigned char> data;
	json JSON;

	// ��� ����� � �������������
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// �������������� ���� �������� �������
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	// �������� ����� ����� �� �������
	void loadMesh(unsigned int indMesh);

	// ����������� ����� �����(����� ���� �����)
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// ��������� ��������� �� �����
	std::vector<unsigned char> getData();
	// �������� � floats, indices, textures
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	// ��������� float � �������
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// ����������� float ��� ������ � ����
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif