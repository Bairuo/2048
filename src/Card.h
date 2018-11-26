// Card.h
// Author: Bairuo

#ifndef CARD_H_
#define CARD_H_

#include<time.h>
#include "Rectangle.h"

class TextRenderer;

class Card : public Rectangle
{
public:
    int score;
    bool active = true;

    void MoveTo(float x, float y, float t);

    void Refresh(float t, float refreshTime, float initialSize);
    void MergeBorn(float t);
    void Death(float t);

    void Update();

    Card() = default;

    Card(float score, const glm::vec3 &pos, const Color &roundColor, float width, float height,
         const GLchar *vertexPath = standardVsPath, const GLchar *fragmentPath = standardFragPath);

private:
    TextRenderer *textRenderer;

    std::string scoreText;
    int scoreSize = 68;
    const float proportion1 = 0.648f;
    bool scoreBold = true;
    Color scoreColor;
    glm::vec2 textShift;

    float deathTimer = 0;
    float deathTime = 0;

    static const GLchar standardVsPath[];
    static const GLchar standardFragPath[];
};

#endif // CARD_H_
