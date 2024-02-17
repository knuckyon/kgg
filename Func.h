#ifndef FUNC_H
#define FUNC_H

#include "globals.h"

bool loadOBJ(const char* path, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

typedef struct ParabConst
{
    GLfloat x0;
    GLfloat y0;
    GLfloat a;

} ParabConst;

void cycleOY(GLfloat& y, GLfloat& x, ParabConst comp);
void cycleOX(GLfloat& x, GLfloat& y, ParabConst comp, GLfloat delta, GLfloat end);

ParabConst findParabComponents(glm::vec3 dot1, glm::vec3 dot2, glm::vec3 dot3);
#endif