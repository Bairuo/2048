#include "InputText.h"
#include "Rectangle.h"

InputText::~InputText()
{

}

InputText::InputText(const std::string &text, const glm::vec3 &pos, const Color &color, int size)
{
    textRenderer = TextRenderer::GetInstance();

    this->pos = glm::vec4(pos, 1);
    this->color = color;
    this->size = size;

    cursorWight = textRenderer->GetCharWidth('a', size, true);
    cursorHeight = cursorWight * 0.31f;

    cursorShift = textRenderer->GetCharAdvy('a', size, true) * 0.74f / 2 * 1.25f;

    cursor.reset(new Rectangle(glm::vec3(0, 0, 0), Color(), Color(), 0, cursorWight, cursorHeight));

    this->text = text;
}

boost::shared_ptr<Animation> InputText::MakeCurosAnimation(const Posture &posture, const Color &color, float time)
{
    boost::shared_ptr<Animation> cursorAnimation(new Animation());
    cursorAnimation->setStartTime(time);
    cursorAnimation->setLoop(true);
    cursorAnimation->addFrame(0, posture, Color(color.r, color.g, color.b, 1));
    cursorAnimation->addFrame(0.6f, posture, Color(color.r, color.g, color.b, 1));
    cursorAnimation->addFrame(0.6f, posture, Color(color.r, color.g, color.b, 0));
    cursorAnimation->addFrame(1.2f, posture, Color(color.r, color.g, color.b, 0));
    cursorAnimation->Start();
    return cursorAnimation;
}

void InputText::Update()
{
    if(cursor.get() != nullptr)
    {
        cursor->Update();
    }

    textRenderer->DrawText(text, pos().x - shift.x, pos().y - shift.y, color, size, true);

}
