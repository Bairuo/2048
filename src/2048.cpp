#include <iostream>
#include <boost/shared_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Player.h"
#include "Board.h"
#include "TextRenderer.h"
#include "AlignedText.h"
#include "Button.h"
#include "PlayerRecord.h"
#include "bairuofunctions.h"
#include "GameTime.h"
#include "GameUtilities.h"
#include "KeyUtilities.h"
#include "ConfigUtilities.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void mouse_callback(GLFWwindow *window, int button, int action, int mode);
static void InitWindow();
static void UpdateCurScore();

static const Color playingPenColor(0, 0.918f, 1.0f, 1.0f);
static const Color AutoPlayPenColor(1, 0, 0, 1.0f);
static const Color titleColor(0.464f, 0.43f, 0.395f, 1.0f);
static const Color backGround(0.98f, 0.972f, 0.937f, 1.0f);
static const Color boardColor(0.733f, 0.678f, 0.627f, 1.0f);
static const Color scoreColor(boardColor);
static const Color bestColor(boardColor);
static const Color newGameColor(0.559f, 0.477f, 0.4f, 1.0f);
static const glm::vec3 boardPos(0, -0.2f, 0);
static const glm::vec3 newGamePos(0.456f, 0.618f, 0);
static const glm::vec3 scorePos(0.178f, 0.81f, 0);
static const glm::vec3 bestPos(0.478f, 0.81f, 0);

#if defined(_WIN32)
static const std::string dataFile = ".\\data\\Playerlist.txt";
static const char ttfFile[] = ".\\res\\Arial.ttf";
#else
static const std::string dataFile = "./data/Playerlist.txt";
static const char ttfFile[] = "./res/Arial.ttf";
#endif

static boost::shared_ptr<AlignedText> score;
static boost::shared_ptr<AlignedText> best;
static boost::shared_ptr<Button> newGame;
static boost::shared_ptr<Board> board;

static boost::shared_ptr<PlayerRecord> playerRecord;
static Player player;
static GameTime mainTime;
static int curScore;

int main()
{
    InitWindow();

    /* subsystem start-up */
	PlayerRecord::init(dataFile);

    TextRenderer* textRenderer = TextRenderer::GetInstance();
    textRenderer->Load(ttfFile);

    /* basic object generate */
    score.reset(new AlignedText("SCORE", 0, 0.015f, scorePos, scoreColor, backGround, 0.28f, 0.14f));
    best.reset(new AlignedText("BEST", 0, 0.015f, bestPos, bestColor, backGround, 0.28f, 0.14f));
    newGame.reset(new Button(newGamePos, newGameColor, backGround, 0.01f, 0.32f, 0.1f));
    board.reset(new Board(boardPos, boardColor, backGround, 1.2f, 1.2f));

    /* subsystem start-up */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gameManager.StartGame(board.get());
    gameManager.StartInput();

    mainTime.Init();
    /* Game loop */
    std::string welcomeText = "";
    while(!glfwWindowShouldClose(window))
    {
        // Event
        glfwPollEvents();

        if(player.state == Moved)
        {
            int freshPoint = board->Fill();
            board->RecordHistory(freshPoint, player.moveOperate);
            player.state = Idle;
            UpdateCurScore();
        }
        else if(player.state == PlayingBack)
        {
            bool isForward = false;
            int pointer;
            player.Playing(board.get(), pointer, isForward);
            UpdateCurScore();

            if(isForward)
            {
                board->ReproduceRefresh(pointer);
            }
        }
        else if(player.state == Auto)
        {
            bool isForward = false;
            bool isOver = player.AutoPlay(board.get(), isForward);
            UpdateCurScore();

            if(isForward)
            {
                int freshPoint = board->Fill();
                board->RecordHistory(freshPoint, player.moveOperate);

                player.UpdateAI(board.get());
            }

            if(isOver)
            {
                player.ShutDownAI();
            }
        }

        // Rendering
        glClearColor(backGround.r, backGround.g, backGround.b, backGround.a);
        glClear(GL_COLOR_BUFFER_BIT);

        board->Update();
        score->Update();
        best->Update();
        newGame->Update();

        textRenderer->DrawText("2048", -245, 300, titleColor, 72, true);
        if(playerRecord.get() == 0)
        {
            textRenderer->DrawText("Welcome to play Bairuo's 2048", -245, 250, titleColor, 18, true);
        }
        else
        {
            textRenderer->DrawText(welcomeText + "Welcome to play, " + playerRecord->getName(), -245, 250, titleColor, 18, true);
        }

        if(player.state == PlayingBack)
        {
            textRenderer->DrawText("History playing, any keys to pause", -245, 210, playingPenColor, 16, true);
        }
        else if(player.state == PlayingPause)
        {
            textRenderer->DrawText("press 'h' to return game, others to continue", -245, 210, playingPenColor, 16, true);
        }
        else if(player.state == Auto)
        {
            textRenderer->DrawText("AI working, any key to stop", -245, 210, AutoPlayPenColor, 16, true);
        }
        else
        {
            textRenderer->DrawText("Undo-z, Redo-y, Ranking-s, History-h, Auto-i", -245, 210, titleColor, 16);
        }


        textRenderer->DrawText("New Game", 133, 239, Color(1.0f, 1.0f, 1.0f, 1.0f), 20, true);

        if(player.state != PlayingBack && !gameManager.isRanking)
        {
            if(!gameManager.isGameWin && gameManager.JudgeWin(board.get()))
            {
                gameManager.WinGame(board.get());
            }
            else if(!gameManager.isGameLose && gameManager.JudgeLose(board.get()))
            {
                gameManager.LoseGame(board.get());
            }
        }

        gameManager.Update();

        glfwSwapBuffers(window);

        mainTime.Update();

    }

    if(playerRecord.get() != nullptr)
    {
        playerRecord->save(dataFile);
    }

    glfwTerminate();

    return 0;
}

void mouse_callback(GLFWwindow *window, int button, int action, int mode)
{
    if(gameManager.isInput || gameManager.isRanking)
        return;

    if(action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        if(newGame->isTrigger(x, y))
        {
            if(player.state == PlayingBack)
            {
                player.PausePlaying();
            }

            if(player.state == PlayingPause)
            {
                player.QuitPlaying(board.get());
            }

            if(player.state == Auto)
            {
                player.ShutDownAI();
            }


            curScore = 0;
            score->SetValue(curScore);
            gameManager.StartGame(board.get());

        }

    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
    {
        if(key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if(!gameManager.isInput)
        {
            /* Game mode */
            /* Not Text Input */

            if(player.state == PlayingBack)
            {
                gameManager.PauseScreenAnimation();
                player.PausePlaying();
            }
            else if(player.state == PlayingPause)
            {
                if(KeyToChar(false, key) != 'h')
                {
                    gameManager.ContinueScreenAnimation();
                    player.ContinueToPlay();
                }
                else
                {
                    gameManager.CleanScreen();
                    player.QuitPlaying(board.get());
                }
            }
            else if(player.state == Auto)
            {
                player.ShutDownAI();
            }
            else
            {
                player.HandleKeyOperate(board.get(), key);
            }
        }
        else
        {
            /* Text Input */
            // gameManager.isInput

            if(key == GLFW_KEY_BACKSPACE)   // delete
            {
                gameManager.DelteInput();
            }
            else if(key == GLFW_KEY_ENTER && gameManager.GetInput() != "")  // confirm
            {
                playerRecord.reset(new PlayerRecord(gameManager.GetInput()));
                best->SetValue(playerRecord->getBestScore());
                gameManager.EndInput();
                gameManager.StartGame(board.get());
            }
            else
            {
                bool capital = isCapsActive();
                char input = KeyToChar(capital, key);
                if(input != 0)
                {
                    gameManager.HandleInput(input);
                }
            }
        }
    }
}

void UpdateCurScore()
{
    if(board->score != curScore)
    {
        curScore = board->score;
        score->SetValue(curScore);

        if(curScore > playerRecord->getBestScore())
        {
            playerRecord->setBestScore(curScore);
            best->SetValue(curScore);
        }
    }
}

void InitWindow()
{
    glfwInit();

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName, nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}
