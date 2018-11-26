#include "Rectangle.h"
#include "ConfigUtilities.h"

const GLuint Rectangle::indices[6] =
{
    0, 1, 2,
    1, 2, 3
};

#if defined(_WIN32)
const GLchar Rectangle::standardVsPath[] = ".\\res\\standard.vs";
const GLchar Rectangle::standardFragPath[] = ".\\res\\rounded.frag";
#else
const GLchar Rectangle::standardVsPath[] = "./res/standard.vs";
const GLchar Rectangle::standardFragPath[] = "./res/rounded.frag";
#endif

void Rectangle::Update()
{
    if(!active)
        return;

    shader.Use();

    if(animation.get() != nullptr)
    {
        color = animation->getColor();
        posture = animation->getPosture();

        if(animation->getActive() == false)
        {
            animation.reset();
        }
        else
        {
            animation->Update();
        }
    }

    if(colorChange)
    {
        shader.Set4f("setcolor", color().r, color().g, color().b, color().a);   // seems can not do like c#
        colorChange = false;
    }

    if(postureChange)
    {
        shader.SetMat4("transform", posture().GetMatrix());
        shader.SetFloat("recWidth", width);
        shader.SetFloat("recHeight", height);
        postureChange = false;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader.Stop();
}

Rectangle::Rectangle() = default;

Rectangle::~Rectangle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

Rectangle::Rectangle(const glm::vec3 &pos, const Color &color, const Color &roundColor,
          float radius, float width, float height,
          const GLchar *vertexPath, const GLchar *fragmentPath)
{
    active = true;

    this->pos = glm::vec3(0, 0, 0);
    posture = Posture(pos.x, pos.y, pos.z, 1);

    this->color = color;
    this->radius = radius;
    this->width = width;
    this->height = height;

    vertices[0] = - width / 2;
    vertices[1] = height / 2;
    vertices[2] = 0;

    vertices[3] = width / 2;
    vertices[4] = height / 2;
    vertices[5] = 0;

    vertices[6] = - width / 2;
    vertices[7] = - height / 2;
    vertices[8] = 0;

    vertices[9] = width / 2;
    vertices[10] = - height / 2;
    vertices[11] = 0;

    Init(vertexPath, fragmentPath, roundColor);
}

void Rectangle::Init(const GLchar *vertexPath, const GLchar *fragmentPath, const Color &roundColor)
{
    shader = Shader(vertexPath, fragmentPath);
    shader.Use();

    shader.SetInt("windowWidth", WindowWidth);
    shader.SetInt("windowHeight", WindowHeight);
    shader.Set4f("roundColor", roundColor.r, roundColor.g, roundColor.b, roundColor.a);
    shader.Set4f("setcolor", color().r, color().g, color().b, color().a);

    shader.SetFloat("radius", radius);

    shader.SetFloat("recWidth", width);
    shader.SetFloat("recHeight", height);

    shader.SetMat4("transform", posture().GetMatrix());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind Vertex Array Object
    glBindVertexArray(VAO);

    // copy to vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set the vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLAT, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.Stop();
}
