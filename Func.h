#ifndef FUNC_H
#define FUNC_H

#include <glm/glm.hpp>
#include <vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <algorithm>

bool loadOBJ(const char* path, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals);

#endif