#include "Text.h"
#include "Animation.h"

Text::~Text()
{

}

Text::Text(const std::string &text, const glm::vec3 &pos, const Color &color, int size)
{
    textRenderer = TextRenderer::GetInstance();

    this->pos = glm::vec4(pos, 1);
    this->color = color;
    this->size = size;

    this->text = text;
}

void Text::Update()
{
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

    textRenderer->DrawText(text, pos().x - shift.x, pos().y - shift.y, color, size, true);
}
