#include "Func.h"


bool loadOBJ(const char* path, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals)
{
    //Временные массивы для хранения индексов и значений из файла
    std::vector< unsigned int > vertexIndices, vtIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;

    //Строка для хранения токенов из файла
    std::string buf;
    //Буфер, хранящий содержимое файла
    std::stringstream sstr;
    //Переменная для корректной работы с неизвестным числом вершин в грани
    bool cont = false;

    //Считывание файла
    std::ifstream file(path, std::ios::in);
    if (file.is_open()) {
        sstr << file.rdbuf();
        file.close();
    }
    else
        return false;

    //Очистка выходных массивов
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //Попытка считать токен из файла
    while (cont || sstr >> buf) {
        cont = false;

        //Обработка координат
        if (buf == "v") {
            glm::vec3 vertex;
            sstr >> vertex.x;
            sstr >> vertex.y;
            sstr >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        //Обработка текстурных координат
        else if (buf == "vt") {
            glm::vec2 texture;
            sstr >> texture.x;
            sstr >> texture.y;
            temp_textures.push_back(texture);
        }
        //Обработка нормалей
        else if (buf == "vn") {
            glm::vec3 normal;
            sstr >> normal.x;
            sstr >> normal.y;
            sstr >> normal.z;
            temp_normals.push_back(normal);
        }
        //Обработка граней
        else if (buf == "f") {
            //Временные массивы для хранения информации о грани
            std::vector<std::string> vertexes;
            std::vector<int> vertindexes;
            while (!cont)
            {
                //Считывается токен
                sstr >> buf;
                //Если токен содержит данные о вершине - она записывается в массив
                if (buf.find('/') != std::string::npos)
                    vertexes.push_back(buf);
                //Если токен - начало следующей строки, он сохраняется и начинается обработка грани
                else
                {
                    //Для каждой из сохранённых вершин производится парсинг данных
                    for (std::string vert : vertexes)
                    {
                        replace(vert.begin(), vert.end(), '/', ' ');
                        std::stringstream tmpstream(vert);
                        int v, vt, n;
                        tmpstream >> v;
                        tmpstream >> vt;
                        tmpstream >> n;
                        //Индексы заносятся в временный массив
                        vertindexes.push_back(v);
                        vertindexes.push_back(vt);
                        vertindexes.push_back(n);
                    }
                    //Первые три вершины заносятся в массивы индексов
                    for (int i = 0; i < 3; i++)
                    {
                        vertexIndices.push_back(vertindexes[i * 3 + 0]);
                        vtIndices.push_back(vertindexes[i * 3 + 1]);
                        normalIndices.push_back(vertindexes[i * 3 + 2]);
                    }
                    //Дальше сложнее - если ещё остались вершины, надо и их занести
                    //Но надо преобразовать из веера треугольников в набор треугольников
                    size_t tmpsize = vertexes.size();
                    if (tmpsize > 3)
                    {
                        //Для каждой из вершин добавляются три вершины, образующие треугольник
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
                    //Чтобы не потерялся считанный токен, ставится флаг о том что он сохранён
                    cont = true;
                }
            }
        }
    }
    // Теперь обработка массивов индексов и создание выходных массивов
    // Для каждой вершины в массиве значения, соответсвующие вершине
    // под указанным индексом заносятся в выходной массив

    size_t tmpsize = vertexIndices.size();
    for (unsigned int i = 0; i < tmpsize; i++)
    {
        //Надо учесть что индексы в файле начинаются с 1!
        glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];
        outVertices.push_back(vertex);
        glm::vec3 normal = temp_normals[normalIndices[i] - 1];
        outNormals.push_back(normal);
        glm::vec2 vt = temp_textures[vtIndices[i] - 1];
        outTextures.push_back(vt);
    }
    return true;
};