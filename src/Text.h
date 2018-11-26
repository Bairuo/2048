// Text.h
// Author: Bairuo

#ifndef TEXT_H_
#define TEXT_H_

#include<string>
#include <boost/shared_ptr.hpp>
#include "TextRenderer.h"
#include "Posture.h"
#include "property.h"
#include "Color.h"

class Animation;

class Text
{
public:
    Posture posture;
    Color color;
    int size;

    property_rw<glm::vec4> pos = property_rw<glm::vec4>(
        property_set(glm::vec4)
        {
            posture = Posture(value.x, value.y, value.z, 1);
        },
        property_get(glm::vec4)
        {
            return posture.GetMatrix() * _pos;
        }
    );

    property_rw<std::string> text = property_rw<std::string>(
        property_set(std::string)
        {
            _text = value;
            shift.x = textRenderer->GetTexLength(text, size, true) / 2;
            shift.x = (int)(shift.x * 10000) * 1.0f / 10000;

            shift.y = textRenderer->GetTexHeight(text, size, true) * 0.74f / 2;
        },
        property_get(std::string)
        {
            return _text;
        }
    );

    boost::shared_ptr<Animation> animation;

    void Update();

    ~Text();

    Text(const std::string &text, const glm::vec3 &pos, const Color &color, int size);

protected:
    TextRenderer *textRenderer;
    glm::vec2 shift;

private:
    std::string _text;
    glm::vec4 _pos = glm::vec4(0, 0, 0, 1);

};

#endif // TEXT_H_
