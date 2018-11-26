#include "bairuofunctions.h"
#include "Card.h"
#include "TextRenderer.h"
#include "GameTime.h"
#include <map>
#include "ConfigUtilities.h"

static const Color penColor1(0.465f, 0.43f, 0.395f, 1.0f);
static const Color penColor2(1.0f, 1.0f, 1.0f, 1.0f);
static const std::map<int, Color> cardColors = {
    {2, Color(0.93f, 0.891f, 0.852f, 1.0f)},
    {4, Color(0.926f, 0.875f, 0.781f, 1.0f)},
    {8, Color(0.945f, 0.691f, 0.423f, 1.0f)},
    {16, Color(0.961f, 0.584f, 0.388f, 1.0f)},
    {32, Color(0.965f, 0.486f, 0.373f, 1.0f)},
    {64, Color(0.965f, 0.369f, 0.231f, 1.0f)},
    {128, Color(0.929f, 0.812f, 0.447f, 1.0f)},
    {256, Color(0.929f, 0.8f, 0.38f, 1.0f)},
    {512, Color(0.929f, 0.784f, 0.314f, 1.0f)},
    {1024, Color(0.929f, 0.773f, 0.247f, 1.0f)},
    {2048, Color(0.929f, 0.773f, 0.247f, 1.0f)}
};

#if defined(_WIN32)
const GLchar Card::standardVsPath[] = ".\\res\\standard.vs";
const GLchar Card::standardFragPath[] = ".\\res\\rounded.frag";
#else
const GLchar Card::standardVsPath[] = "./res/standard.vs";
const GLchar Card::standardFragPath[] = "./res/rounded.frag";
#endif

Card::Card(float score, const glm::vec3 &pos, const Color &roundColor, float width, float height,
           const GLchar *vertexPath, const GLchar *fragmentPath
           ):Rectangle(pos, cardColors.find(score)->second, roundColor, c_cardRadius, width, height, vertexPath, fragmentPath)
{
    textRenderer = TextRenderer::GetInstance();

    this->score = score;

    //scoreSize = proportion1 * (width * WindowWidth / 2);
    if(score < 100)
    {
        scoreSize = 68;
    }
    else if(score < 1000)
    {
        scoreSize = 51;
    }
    else if(score < 10000)
    {
        scoreSize = 38;
    }

    scoreText = bairuo::ToString(score);

    //std::cout << textRenderer->GetTexLength(scoreText, scoreSize, scoreBold) << std::endl;

    // A mysterious problem :
    // It seems that the shift position of "32" will make text vague
    // Under size 68, the text length of "32" is 0.1875, 0.1873, 0.1874, 0.1876, 0.1877 all can use
//    if(score == 32)
//        textShift.x = 0.0937;
//    else
//        textShift.x = textRenderer->GetTexLength(scoreText, scoreSize, scoreBold) / 2;

    // It seems number after a decimal point fourth will cause vague sometimes
    textShift.x = textRenderer->GetTexLength(scoreText, scoreSize, scoreBold) / 2;
    textShift.x = (int)(textShift.x * 10000) * 1.0f / 10000;

    textShift.y = textRenderer->GetTexHeight(scoreText, scoreSize, scoreBold) * 0.74f / 2;

    if(score <= 4)
    {
        scoreColor = penColor1;
    }
    else
    {
        scoreColor = penColor2;
    }
}


void Card::Refresh(float t, float refreshTime, float initialSize)
{
    if(animation.get() != nullptr)
    {
        animation.reset();
    }

    animation.reset(new Animation());
    animation->setStartTime(t);
    animation->addFrame(0, Posture(pos, initialSize), color);
    animation->addFrame(refreshTime, Posture(pos, 1), color);
    animation->Start();
}

void Card::MergeBorn(float t)
{
    if(animation.get() != nullptr)
    {
        animation.reset();
    }

    animation.reset(new Animation());
    animation->setStartTime(t);
    animation->addFrame(0, Posture(pos, 1), color);
    animation->addFrame(c_mergeBornTime * 0.75f, Posture(pos, 1.1f), Color(color().r, color().g, color().b, 0.8f));
    animation->addFrame(c_mergeBornTime, Posture(pos, 1), color);
    animation->Start();
}

void Card::MoveTo(float x, float y, float t)
{
    if(animation.get() != nullptr)
    {
        posture = animation->getEndPosture();
        color = animation->getEndColor();
        animation.reset();
    }

    animation.reset(new Animation());
    animation->addFrame(0, Posture(pos, 1), color);
    animation->addFrame(t, Posture(x, y, pos().z, 1), color);
    animation->Start();
}


void Card::Death(float t)
{
    deathTime = t;
}


void Card::Update()
{
    Rectangle::Update();

    /** death event **/
    if(deathTime > 0)
    {
        deathTimer += GameTime::deltaTime;

        if(deathTimer > deathTime)
        {
            active = false;
            return;
        }
    }

    /** disgraceful, slow **/
//    clock_t cur = clock();
//    fontTimer += 1.0f * (cur - lastTime) / CLOCKS_PER_SEC;
//    lastTime = cur;
//
//    if(fontTimer > 0.03f)
//    {
//        scoreSize = proportion1 * (width * WindowWidth / 2);
//        fontTimer = 0;
//    }

    //std::cout << pos().x - textRenderer->GetTexLength(scoreText, scoreSize, scoreBold) / 2 << ", " << pos().y - textRenderer->GetTexHeight(scoreText, scoreSize, scoreBold) * 0.74f / 2 << std::endl;
    //std::cout << textRenderer->GetTexLength(scoreText, scoreSize, scoreBold) / 2 << ", " << textRenderer->GetTexHeight(scoreText, scoreSize, scoreBold) * 0.74f / 2 << std::endl;

    if(width > 0)
    {
        textRenderer->DrawText(scoreText, pos().x - textShift.x, pos().y - textShift.y, scoreColor, scoreSize, scoreBold);
    }
}
