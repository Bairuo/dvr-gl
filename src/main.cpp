#include <iostream>
#include <boost/shared_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TextRenderer.h"
#include "bairuofunctions.h"
#include "GameTime.h"
#include "KeyUtilities.h"
#include "ConfigUtilities.h"
#include "Cuboid.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Transform.h"
#include "GraphNode.h"
#include "DVRTextureGenerate.h"

#include <list>

static const Color titleColor(1.0f, 1.0f, 1.0f, 1.0f);
static const Color FPSColor(1, 0, 0, 1.0f);

static const Color backGround(0, 0, 0, 1.0f);

#if defined(_WIN32)
static const char ttfFile[] = ".\\res\\Arial.ttf";
#else
static const char ttfFile[] = "./res/Arial.ttf";
#endif

#define OBJNUM 1
static const glm::vec3 objPos[OBJNUM] = {
    glm::vec3(0, 0, 0)
};

static const glm::vec3 objScale[OBJNUM] = {
    glm::vec3(1, 1, 1)
};

static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX = WindowWidth / 2.0f;
static float lastY = WindowHeight / 2.0f;
static bool firstMouse = true;

static GameTime mainTime;

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
static void InitWindow();

int main()
{
    InitWindow();

    /* subsystem start-up */
    TextRenderer* textRenderer = TextRenderer::GetInstance();
    textRenderer->Load(ttfFile);

    /* object generate */
    Camera::MainCamera = &camera;

    for(int i = 0; i < OBJNUM; i++)
    {
        auto obj = generateObject(new Transform(objPos[i], objScale[i], 20.0f, glm::vec3(1.0f, 0.3f, 0.5f)),
                                  new Cuboid());
    }

    /* rendering setting */
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(backGround.r, backGround.g, backGround.b, backGround.a);

    // Time
    mainTime.Init();

    GLuint volumeTexid = buildTexture3d();
    GLuint transferTexid = buildTransferFunction();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, volumeTexid);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, transferTexid);


    /* Game loop */
    while(!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window, camera);

        // Event
        glfwPollEvents();

        // Game update
        SceneGraph.update(glm::mat4(1), false);

        // render
        glEnable(GL_DEPTH_TEST);

        // Normal render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(size_t i = 0; i < Objects.size(); i++)
        {
            Objects[i]->render();
        }

        glDisable(GL_DEPTH_TEST);

        textRenderer->DrawText("DVR Renderer", -680, 358, titleColor, 32, true);
        textRenderer->DrawText("FPS:" + bairuo::int2str(GameTime::GetFPS()), -680, 326, FPSColor, 16, true);
        textRenderer->DrawText("WASD to move ", -680, -334, titleColor, 16, true);


        glfwSwapBuffers(window);

        mainTime.update();

    }

    glfwTerminate();

    return 0;
}


void InitWindow()
{
    glfwInit();

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName, nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
