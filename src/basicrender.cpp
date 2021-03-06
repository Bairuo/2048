#include "basicrender.h"

static const GLuint squareIndices[6] =
{
    0, 1, 2,
    1, 2, 3
};

static const GLfloat squareVertices[12]
{
    -0.5f, 0.5f, 0,
    0.5f, 0.5f, 0,
    -0.5f, -0.5f, 0,
    0.5f, -0.5f, 0
};

GLuint getSquareVAO()
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    // Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    // Set the vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLAT, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    init = true;

    return VAO;
}
