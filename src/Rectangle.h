// Rectangle.h
// Author: Bairuo

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Posture.h"
#include "Animation.h"
#include "property.h"

class Rectangle
{
public:
    float radius;
    boost::shared_ptr<Animation> animation;

    property_rw<Posture> posture = property_rw<Posture>(
        property_set(Posture)
        {
            postureChange = true;
            _posture = value;
        },
        property_get(Posture)
        {
            return _posture;
        }
    );

    property_rw<glm::vec3> pos = property_rw<glm::vec3>(
        property_set(glm::vec3)
        {
            _pos = value;
        },
        property_get(glm::vec3)
        {
            glm::vec4 t = posture().GetMatrix() * glm::vec4(_pos.x, _pos.y, _pos.z, 1);

            return  glm::vec3(t.x, t.y, t.z);
        }
    );

    property_rw<Color> color = property_rw<Color>(
        property_set(Color)
        {
            colorChange = true;
            _color = value;
        },
        property_get(Color)
        {
            return _color;
        }
    );

    property_rw<float> width = property_rw<float>(
        property_set(float)
        {
            _width = value;
        },
        property_get(float)
        {
            return _width * posture().scale;
        }
    );

    property_rw<float> height = property_rw<float>(
        property_set(float)
        {
            _height = value;
        },
        property_get(float)
        {
            return _height * posture().scale;
        }
    );


    virtual void Update();
    Rectangle();
    virtual ~Rectangle();

    Rectangle(const glm::vec3 &pos, const Color &color, const Color &roundColor,
              float radius, float width, float height,
              const GLchar *vertexPath = standardVsPath, const GLchar *fragmentPath = standardFragPath);

    void Init(const GLchar *vertexPath, const GLchar *fragmentPath, const Color &roundColor);


private:
    GLfloat vertices[12];
    static const GLuint indices[6];

    GLuint VBO, VAO;
    GLuint EBO;

    Shader shader;

    Color _color;
    Posture _posture;
    glm::vec3 _pos;
    float _width;
    float _height;

    bool active = false;
    bool colorChange = false;
    bool postureChange = false;

    static const GLchar standardVsPath[];
    static const GLchar standardFragPath[];
};


#endif // RECTANGLE_H_
