#ifndef FUNC_H
#define FUNC_H

#include <glm/glm.hpp>
#include<GLFW/glfw3.h>
#include <vector>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include <algorithm>

extern bool isRotateStatue;
extern char directionStatue;

extern bool isFlying;

extern glm::vec3 owlPos;
extern GLfloat owlShift;




bool loadOBJ(const char* path, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

typedef struct ParabConst
{
    GLfloat x0;
    GLfloat y0;
    GLfloat a;

} ParabConst;

void cycleOX(GLfloat& x, GLfloat delta, GLfloat end);

void cycleOY(GLfloat& y, GLfloat x, ParabConst comp);

ParabConst findParabComponents(glm::vec3 dot1, glm::vec3 dot2, glm::vec3 dot3);
#endif