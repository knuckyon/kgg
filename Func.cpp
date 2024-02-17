#include "Func.h"

using namespace std;


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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_Z: // �������� �������� (z)
        if (action == GLFW_PRESS)
        {
            isRotateStatue = true;
            directionStatue = 1;
        }
        else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
            isRotateStatue = false;
        break;
    case GLFW_KEY_X: // �������� �������� (z)
        if (action == GLFW_PRESS)
        {
            isRotateStatue = true;
            directionStatue = 0;
        }
        else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
            isRotateStatue = false;
        break;
    case GLFW_KEY_F:
        {
        //owlPos = glm::vec3(525.0f, -75.0f, -590.0f);
        isFlying = true;
        }
        break;
    }
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        isFlying = false;
        owlPos = glm::vec3(525.0f, -75.0f, -590.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == (GLFW_PRESS || GLFW_REPEAT))
    {
        owlPos.x -= owlShift;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == (GLFW_PRESS || GLFW_REPEAT))
    {
        owlPos.z -= owlShift;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == (GLFW_PRESS || GLFW_REPEAT))
    {
        owlPos.x += owlShift;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == (GLFW_PRESS || GLFW_REPEAT))
    {
        owlPos.z += owlShift;
    }
    
    if (glfwGetKey(window, GLFW_KEY_MINUS) == (GLFW_PRESS || GLFW_REPEAT))
    {
        owlPos.y += owlShift;
    }
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == (GLFW_PRESS || GLFW_REPEAT))
    {
        owlPos.y -= owlShift;
    }
}

void cycleOY(GLfloat& y, GLfloat& x, ParabConst comp)
{
    cout << "vizov Y:";
    y = comp.a * (x - comp.x0) * (x - comp.x0) + comp.y0;
    cout << "owlPos.y " << y << endl;
    this_thread::sleep_for(chrono::milliseconds(800));
}
void cycleOX(GLfloat& x, GLfloat& y, ParabConst comp, GLfloat delta, GLfloat end)
{
    cout << "vizov X: ";
   
    //this_thread::sleep_for(chrono::milliseconds(500));
    if (x < end)
    {
        isFlying = false;
    }
    while (x > end)
    {
        x += delta;
        y = comp.a * (x - comp.x0) * (x - comp.x0) + comp.y0;
       // cout << x << endl;
        cout << "owlPos.x " << owlPos.x << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
    }
    //isFlying = false;
}
ParabConst findParabComponents(glm::vec3 dot1, glm::vec3 dot2, glm::vec3 dot3)
{
    ParabConst temp;
    GLfloat c = (dot1.y - dot2.y) / (dot2.y - dot3.y);
    temp.x0 = (dot1.x * dot1.x + dot2.x * dot2.x + c * (dot2.x * dot2.x - dot3.x * dot3.x)) / (-dot1.x + dot2.x + c * dot2.x - c * dot3.x);
    temp.a = (dot1.y - dot2.y) / ((dot1.x - temp.x0) * (dot1.x - temp.x0) - (dot2.x - temp.x0) * (dot2.x - temp.x0));
    temp.y0 = dot1.y - temp.a * (dot1.x - temp.x0) * (dot1.x - temp.x0);
    return temp;
}