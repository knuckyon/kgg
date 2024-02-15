#include "Func.h"


bool loadOBJ(const char* path, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals)
{
    //��������� ������� ��� �������� �������� � �������� �� �����
    std::vector< unsigned int > vertexIndices, vtIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;

    //������ ��� �������� ������� �� �����
    std::string buf;
    //�����, �������� ���������� �����
    std::stringstream sstr;
    //���������� ��� ���������� ������ � ����������� ������ ������ � �����
    bool cont = false;

    //���������� �����
    std::ifstream file(path, std::ios::in);
    if (file.is_open()) {
        sstr << file.rdbuf();
        file.close();
    }
    else
        return false;

    //������� �������� ��������
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //������� ������� ����� �� �����
    while (cont || sstr >> buf) {
        cont = false;

        //��������� ���������
        if (buf == "v") {
            glm::vec3 vertex;
            sstr >> vertex.x;
            sstr >> vertex.y;
            sstr >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        //��������� ���������� ���������
        else if (buf == "vt") {
            glm::vec2 texture;
            sstr >> texture.x;
            sstr >> texture.y;
            temp_textures.push_back(texture);
        }
        //��������� ��������
        else if (buf == "vn") {
            glm::vec3 normal;
            sstr >> normal.x;
            sstr >> normal.y;
            sstr >> normal.z;
            temp_normals.push_back(normal);
        }
        //��������� ������
        else if (buf == "f") {
            //��������� ������� ��� �������� ���������� � �����
            std::vector<std::string> vertexes;
            std::vector<int> vertindexes;
            while (!cont)
            {
                //����������� �����
                sstr >> buf;
                //���� ����� �������� ������ � ������� - ��� ������������ � ������
                if (buf.find('/') != std::string::npos)
                    vertexes.push_back(buf);
                //���� ����� - ������ ��������� ������, �� ����������� � ���������� ��������� �����
                else
                {
                    //��� ������ �� ���������� ������ ������������ ������� ������
                    for (std::string vert : vertexes)
                    {
                        replace(vert.begin(), vert.end(), '/', ' ');
                        std::stringstream tmpstream(vert);
                        int v, vt, n;
                        tmpstream >> v;
                        tmpstream >> vt;
                        tmpstream >> n;
                        //������� ��������� � ��������� ������
                        vertindexes.push_back(v);
                        vertindexes.push_back(vt);
                        vertindexes.push_back(n);
                    }
                    //������ ��� ������� ��������� � ������� ��������
                    for (int i = 0; i < 3; i++)
                    {
                        vertexIndices.push_back(vertindexes[i * 3 + 0]);
                        vtIndices.push_back(vertindexes[i * 3 + 1]);
                        normalIndices.push_back(vertindexes[i * 3 + 2]);
                    }
                    //������ ������� - ���� ��� �������� �������, ���� � �� �������
                    //�� ���� ������������� �� ����� ������������� � ����� �������������
                    size_t tmpsize = vertexes.size();
                    if (tmpsize > 3)
                    {
                        //��� ������ �� ������ ����������� ��� �������, ���������� �����������
                        for (int i = 3; i < tmpsize; i++)
                        {
                            vertexIndices.push_back(vertindexes[0]);
                            vtIndices.push_back(vertindexes[1]);
                            normalIndices.push_back(vertindexes[2]);
                            vertexIndices.push_back(vertindexes[(i - 1) * 3 + 0]);
                            vtIndices.push_back(vertindexes[(i - 1) * 3 + 1]);
                            normalIndices.push_back(vertindexes[(i - 1) * 3 + 2]);
                            vertexIndices.push_back(vertindexes[i * 3 + 0]);
                            vtIndices.push_back(vertindexes[i * 3 + 1]);
                            normalIndices.push_back(vertindexes[i * 3 + 2]);
                        }
                    }
                    //����� �� ��������� ��������� �����, �������� ���� � ��� ��� �� �������
                    cont = true;
                }
            }
        }
    }
    // ������ ��������� �������� �������� � �������� �������� ��������
    // ��� ������ ������� � ������� ��������, �������������� �������
    // ��� ��������� �������� ��������� � �������� ������

    size_t tmpsize = vertexIndices.size();
    for (unsigned int i = 0; i < tmpsize; i++)
    {
        //���� ������ ��� ������� � ����� ���������� � 1!
        glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];
        outVertices.push_back(vertex);
        glm::vec3 normal = temp_normals[normalIndices[i] - 1];
        outNormals.push_back(normal);
        glm::vec2 vt = temp_textures[vtIndices[i] - 1];
        outTextures.push_back(vt);
    }
    return true;
};