#include "ConfigUtilities.h"
#include "Cuboid.h"

GLFWwindow *window;
char WindowName[] = "dvr-gl";
extern const GLuint WindowWidth = 1400;
extern const GLuint WindowHeight = 800;

extern const float c_cardRadius = 0.01f;
extern const float c_refreshTime = 0.3f;
extern const float c_mergeBornTime = 0.2f;

#if defined(_WIN32)
const GLchar Cuboid::standardVsPath[] = ".\\shaders\\3D_Standard\\standard.vs";
const GLchar Cuboid::standardFragPath[] = ".\\shaders\\3D_Standard\\standard.frag";
#else
const GLchar Cuboid::standardVsPath[] = "./shaders/3D_Standard/standard.vs";
const GLchar Cuboid::standardFragPath[] = "./shaders/3D_Standard/standard.frag";
#endif
