#include "Rectangle.h"
#include "ConfigUtilities.h"
#include "basicrender.h"

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
    {
        return;
    }

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
        shader.SetMat4("transform", posture().getMatrix());
        shader.SetFloat("recWidth", posture().getScaleX());
        shader.SetFloat("recHeight", posture().getScaleY());
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
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
}

Rectangle::Rectangle(const glm::vec3 &pos, const Color &color, const Color &roundColor,
          float radius, float width, float height,
          const GLchar *vertexPath, const GLchar *fragmentPath)
          :radius(radius)
{
    posture = Posture(pos.x, pos.y, pos.z, width, height, 1);

    this->color = color;

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

    shader.SetFloat("recWidth", posture().getScaleX());
    shader.SetFloat("recHeight", posture().getScaleX());

    shader.SetMat4("transform", posture().getMatrix());

    VAO = getSquareVAO();

    shader.Stop();
}
