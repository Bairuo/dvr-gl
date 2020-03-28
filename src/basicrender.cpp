#include "basicrender.h"

static const GLfloat cubeVertices[] =
{
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f, 0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f
};

static const GLuint cubeIndices[] =
{
    0, 2, 1,
    0, 3, 2,
    2, 3, 4,
    2, 4, 5,
    1, 2, 5,
    1, 5, 6,
    0, 7, 4,
    0, 4, 3,
    5, 4, 7,
    5, 7, 6,
    0, 6, 7,
    0, 1, 6
};

GLuint getCubeVAO()
{
    static bool init = false;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    if(init)
    {
        return VAO;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind Vertex Array Object
    glBindVertexArray(VAO);

    // copy to vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    init = true;

    return VAO;
}



