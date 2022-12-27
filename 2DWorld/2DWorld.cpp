/* by Wildan R. */

#include "2DWorld.h"

const int screenWidth  = 512;
const int screenHeight = 512;

constexpr int8_t maxCarrots      = 12;
constexpr int8_t maxSmallFlowers = 12;

int framesCounter  = 0;
bool endGame       = 0;
bool developerMode = 1;

Timer audioTimer   = {};
Timer teleportText = {};
Audio audio        = {};
Music mainBGM      = {};
Music logoBGM      = {};
Music birdBGM      = {};
Music menuBGM      = {};
Sound clickSound   = {};

MagicFruit* magicFruit = nullptr;

std::unique_ptr<std::vector<std::unique_ptr<Carrot>>> carrots = nullptr;
std::unique_ptr<std::vector<std::unique_ptr<Flower>>> flowers = nullptr;

static std::unique_ptr<StaticObject> staticGameObj = nullptr;

std::unique_ptr<std::vector<int>> dynamicObjPos = nullptr;
std::unique_ptr<Camera2D>         camera        = nullptr;

std::unique_ptr<std::array<Tree, 20>> trees = nullptr;

std::thread loadDataThread;

static std::atomic<bool> dataLoaded = ATOMIC_VAR_INIT(false);

static int dataProgress = 0;

static const char* strCameraMode[2]
{
    "Camera Mode: Smooth",
    "Camera Mode: Normal"
};
bool cameraNormalMode = 0;

static const Rectangle invisibleFences[19]
{
    { 0.0f,     1390.0f,    400.0f,     350.0f  },
    { 472.0f,   380.0f,     10.0f,      10.0f   },
    { 510.0f,   170.0f,     550.0f,     500.0f  },
    { 1790.0f,  929.0f,     970.0f,     750.2f  },
    { 1749.0f,  1982.0f,    530.0f,     450.2f  },
    { 720.0f,   1599.0f,    100.0f,     100.2f  },
    { 94.0f,    590.0f,     340.0f,     20.2f   },
    { 94.0f,    194.0f,     390.0f,     20.2f   },
    { 94.0f,    194.0f,     20.0f,      400.2f  },
    { 489.0f,   194.0f,     20.0f,      210.2f  },
    { 340.0f,   700.0f,     78.0f,      20.2f   },
    { 905.0f,   675.0f,     100.0f,     80.2f   },
    { 1690.0f,  1740.0f,    10.0f,      90.2f   },
    { 145.0f,   3590.0f,    78.0f,      78.2f   },
    
    // bridge 1
    { 410.0f,  1210.0f,  5.0f,   260.0f   },
    { 535.0f,  1210.0f,  5.0f,   260.0f   },

    // bridge 2
    { 1180.0f,  1850.0f,  325.0f,  5.0f  },
    { 1180.0f,  2000.0f,  325.0f,  5.0f  }
};

void SetupWindow();

void InitGame();
void UpdateGame();
void EndGame();

void UpdateCamera(Camera2D& camera, const Player& player);

void DrawVolumeBar(const int x = 10);
void DrawMuteCheckBoxV(const Vector2D pos);

void UpdateVolumeTimer();

void InitAudio();
void UpdateAudio();
void InitBGM();
void ShutdownAudio();

inline static bool OnTouch(const Player& player, float targetPosX);
inline static Vector2D PlantPosition(const Player& player, int px);

static void* LoadDataThread();

enum class ApplicationStates
{
    LOGO = 0,
    TITLE,
    LOADING,
    LOADING_FINISHED,
    GAMEPLAY,
    MENU,
    PAUSE
};
ApplicationStates applicationState;

class LogoScreen : public Screen
{
public:
    void Draw() override
    {
        DrawHelixNebula();
    }
};
LogoScreen logoScreen;

class TitleScreen : public Screen
{
public:
    void Draw() override
    {
        DrawEarth();

        DrawText("Walking", 15, 20, 40, GREEN);

        DrawCenteredText(164, "Welcome to 2DWorld!", 24, GREEN);

        DrawCenteredText(220, "PRESS ENTER to Walking!", 19, GREEN);

        DrawVolumeBar(screenWidth - 220);
    }
};
TitleScreen titleScreen;

class GameplayScreen : public Screen
{
public:
    void Draw() override
    {
        UpdateCamera(*camera, staticGameObj->player);

        BeginMode2D(*camera);

        DrawGamePlayScreen();

        EndMode2D();

        DrawGamePlayHUD(*camera, staticGameObj->player);
    }

    void InitTexture()
    {
        _HUDFillBar05 = LoadTexture("textures/HUD/HUD_FillBar05.png");
    }

    void FreeTexture()
    {
        UnloadTexture(_HUDFillBar05);
    }

private:
    void DrawGamePlayScreen();
    void DrawGamePlayHUD(const Camera2D& camera, const Player& player);

private:
    Texture2D _HUDFillBar05{};
    struct GameplayButton
    {
        bool isMenu = 0;
    };
    GameplayButton _gameplayButton;
};
GameplayScreen gameplayScreen;

class MenuScreen : public Screen
{
public:
    void Draw() override
    {
        DrawDusk();

        float x((screenWidth / 2) - 50);
        
        if (_menuStates == _MenuStates::MAIN_MENU)
        {
            _menuButton.isInventory = GuiButton({ x, 110 + (float)(45 * 0), 120, 35 }, "Inventory");
            _menuButton.isOption    = GuiButton({ x, 110 + (float)(45 * 1), 120, 35 }, "Option");
            _menuButton.isBack      = GuiButton({ x, 110 + (float)(45 * 2), 120, 35 }, "Back");
            _menuButton.isExit      = GuiButton({ x, 110 + (float)(45 * 3), 120, 35 }, "Exit");
        }

        if (_menuStates == _MenuStates::OPTION)
        {
            const Color volumeColors[4]{ RED, DARKGREEN, LIME, GREEN };

            int volume = (int)(audio.masterVolume * 10);

            if (volume > 5) volume = volume - 1;

            int volumeColor = 2;

            if (audio.masterVolume < 0.1f) volumeColor = 0;
            else if (audio.masterVolume > 0.1f && audio.masterVolume < 0.3f) volumeColor = 1;
            else if (audio.masterVolume > 0.3f && audio.masterVolume < 0.4f) volumeColor = 2;
            else if (audio.masterVolume > 0.4f) volumeColor = 3;

            Rectangle volumeBarRec = { 0 };

            DrawText("Volume: ", x - 60, 120 + 20, 16, DARKGRAY);

            for (auto i = 0; i < 5; i++)
            {
                volumeBarRec = { x + 21 * i, 120 + 20, 20, 20 };

                if (CheckCollisionRecs({ GetMousePosition().x, GetMousePosition().y, 5, 5 }, volumeBarRec)
                    && (GetMouseWheelMove() > 0.0f) && audio.masterVolume <= 0.5f && !audio.muted)
                {
                    audio.masterVolume += 0.1f;
                    audio.currentVolume += 0.1f;
                }
                if (CheckCollisionRecs({ GetMousePosition().x, GetMousePosition().y, 5, 5 }, volumeBarRec)
                    && (GetMouseWheelMove() < 0.0f) && audio.masterVolume >= 0.1f && !audio.muted)
                {
                    audio.masterVolume -= 0.1f;
                    audio.currentVolume -= 0.1f;
                }
                if (audio.masterVolume > 0.0f)
                {
                    if (i < volume)
                    {
                        DrawRectangle(volumeBarRec.x, volumeBarRec.y,
                            volumeBarRec.width, volumeBarRec.height, volumeColors[volumeColor]);
                    }
                    DrawRectangleLines(volumeBarRec.x, volumeBarRec.y, volumeBarRec.width, volumeBarRec.height, RED);
                }
                else
                {
                    if (CheckCollisionRecs({ GetMousePosition().x, GetMousePosition().y, 5, 5 }, { x + 15, 120 + 140, 48 * 2, 28 })
                        && (GetMouseWheelMove() > 0.0f) && audio.masterVolume <= 0.1f)
                    {
                        audio.masterVolume += 0.1f;
                        audio.currentVolume += 0.1f;
                    }
                    DrawText("Muted", x + 15, 120 + 20, 28, RED);
                }
            }

            _menuButton.isVolumeUp = GuiButton({ x + 140, 120 + 20, 20, 20 }, "+");
            _menuButton.isVolumeDown = GuiButton({ x + 120, 120 + 20, 20, 20 }, "-");

            if (_menuButton.isVolumeUp && audio.masterVolume <= 0.5f && !audio.muted)
            {
                audio.masterVolume += 0.1f;
                audio.currentVolume += 0.1f;

                PlaySound(clickSound);
            }
            if (_menuButton.isVolumeDown && audio.masterVolume >= 0.1f && !audio.muted)
            {
                audio.masterVolume -= 0.1f;
                audio.currentVolume -= 0.1f;

                PlaySound(clickSound);
            }
            if (_menuButton.isVolumeUp && audio.muted || _menuButton.isVolumeDown && audio.muted)
            {
                audio.muted = !audio.muted;
                audio.masterVolume = audio.currentVolume;
            }

            developerMode = GuiCheckBox({ screenWidth - 165, screenHeight - 205, 15, 15 }, "Developer Mode", developerMode);

            _menuButton.isBackToMainMenu = GuiButton({ screenWidth - 165, screenHeight - 180, 120, 35 }, "Back");

            if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
            {
                _menuStates = _MenuStates::MAIN_MENU;

                _menuButton.isBackToMainMenu = 0;
            }
        }

        if (_menuStates == _MenuStates::INVENTORY)
        {
            DrawText("Flowers: ", x - 100, 120 + 20, 16, DARKGRAY);

            DrawTextureEx(_flower, { x - 40, 105 }, 0.0f, 4.0f, WHITE);

            std::string plantedFlowers{ "Planted flowers : " };
            std::string unplantedFlowers{ "Unplanted flowers : " };

            DrawText(plantedFlowers.append(std::to_string(flowers->size())).c_str(), x + 30, 130, 16, DARKGRAY);

            DrawText(unplantedFlowers.append(std::to_string(maxSmallFlowers - flowers->size())).c_str(), x + 70, 120 + 30, 16, DARKGRAY);

            const int yCarrot = 74;

            DrawText("Carrots: ", x - 100, 120 + 20 + yCarrot, 16, DARKGRAY);

            DrawTextureEx(_carrot, { x - 28, 134 + yCarrot }, 0.0f, 1.8f, WHITE);

            std::string plantedCarrots{ "Planted carrots : " };
            std::string unplantedCarrots{ "Unplanted carrots : " };

            DrawText(plantedCarrots.append(std::to_string(carrots->size())).c_str(), x + 30, 130 + yCarrot, 16, DARKGRAY);

            DrawText(unplantedCarrots.append(std::to_string(maxSmallFlowers - carrots->size())).c_str(), x + 70, 120 + 30 + yCarrot, 16, DARKGRAY);

            _menuButton.isBackToMainMenu = GuiButton({ screenWidth - 165, screenHeight - 180, 120, 35 }, "Back");

            if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
            {
                _menuStates = _MenuStates::MAIN_MENU;

                _menuButton.isBackToMainMenu = 0;
            }
        }

        time_t now = time(0);

        const char* dt = ctime(&now);

        std::string strDateAndTime = "The local date and time is: ";

        DrawText(strDateAndTime.append(dt).c_str(), 50, 120 + 260, 18, RED);

        DrawFPS(screenWidth - 80, 8);

        DrawMuteCheckBoxV({ screenWidth - 165, screenHeight - 80 });

        if (_menuButton.isOption)
        {
            PlaySound(clickSound);

            _menuStates = _MenuStates::OPTION;

            SetWindowTitle("Option");

            _menuButton.isOption = 0;
        }
        if (_menuButton.isInventory)
        {
            PlaySound(clickSound);

            SetWindowTitle("Inventory");

            _menuStates = _MenuStates::INVENTORY;

            _menuButton.isInventory = 0;
        }
        if (_menuButton.isBackToMainMenu)
        {
            PlaySound(clickSound);

            _menuStates = _MenuStates::MAIN_MENU;

            _menuButton.isBackToMainMenu = 0;

            SetWindowTitle("Menu");
        }
        if (_menuButton.isExit)
        {
            PlaySound(clickSound);

            endGame = 1;
        }
        if (_menuButton.isBack)
        {
            SetActiveScreen(&gameplayScreen);

            SetWindowTitle("Walking");

            applicationState = ApplicationStates::GAMEPLAY;

            PauseMusicStream(mainBGM);

            PlaySound(clickSound);
        }
    }

    void InitTexture()
    {
        _carrot = LoadTexture("textures/natural_objects/carrot/big.png");
        _flower = LoadTexture("textures/natural_objects/small_purple_flower.png");
    }

    void FreeTexture()
    {
        UnloadTexture(_carrot);
        UnloadTexture(_flower);
    }

private:
    Texture2D _carrot{};
    Texture2D _flower{};
    enum class _MenuStates
    {
        MAIN_MENU = 0,
        INVENTORY,
        OPTION
    };
    _MenuStates _menuStates = _MenuStates::MAIN_MENU;
    struct MenuButton
    {
        bool isBackToMainMenu = 0;
        bool isBack           = 0;
        bool isInventory      = 0;
        bool isOption         = 0;
        bool isExit           = 0;
        bool isVolumeUp       = 0;
        bool isVolumeDown     = 0;
    };
    MenuButton _menuButton;
};
MenuScreen menuScreen;

class PauseScreen : public Screen
{
public:
    void Draw() override
    {
        DrawGalaxy();

        DrawCenteredText(220, "Paused", 60, BLUE);

        DrawVolumeBar();

        _pauseButton.isMenu = GuiButton({ screenWidth - 165, screenHeight - 160, 120, 35 }, "Menu");
        _pauseButton.isBack = GuiButton({ screenWidth - 165, screenHeight - 120, 120, 35 }, "Back");

        if (_pauseButton.isMenu)
        {
            SetActiveScreen(&menuScreen);

            SetWindowTitle("Menu");

            applicationState = ApplicationStates::MENU;

            PlaySound(clickSound);
        }
        if (_pauseButton.isBack)
        {
            SetActiveScreen(nullptr);

            SetWindowTitle("Walking");

            applicationState = ApplicationStates::GAMEPLAY;

            PauseMusicStream(mainBGM);

            PlaySound(clickSound);
        }
    }

private:
    struct PauseButton
    {
        bool isMenu = 0;
        bool isBack = 0;
    };
    PauseButton _pauseButton;
};
PauseScreen pauseScreen;

void SetupWindow()
{
    InitWindow(screenWidth, screenHeight, "Walking");

    SetExitKey(0);

    SetTargetFPS(60);

    Image icon = LoadImage("icon/earth_400_400.png");

    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    ImageColorReplace(&icon, BLACK, BLANK);

    SetWindowIcon(icon);

    // free the image data
    UnloadImage(icon);
}

void InitGame()
{
    SetupWindow();

    InitAudio();

    InitScreenTexture();

#ifdef _DEBUG
    SetActiveScreen(&gameplayScreen);

    applicationState = ApplicationStates::GAMEPLAY;

    UnloadMusicStream(logoBGM);
#else
    applicationState = ApplicationStates::LOGO;

    SetActiveScreen(&logoScreen);
#endif

    magicFruit = new MagicFruit;

    staticGameObj = std::make_unique<StaticObject>();

    flowers = std::make_unique<std::vector<std::unique_ptr<Flower>>>();

    carrots = std::make_unique<std::vector<std::unique_ptr<Carrot>>>();

    dynamicObjPos = std::make_unique<std::vector<int>>();

    magicFruit->LoadTexture(LoadTexture("textures/magic_fruit/apple.png"));

    clickSound = LoadSound("sounds/menu_selection_click.wav");

    trees = std::make_unique<std::array<Tree, 20>>();

    float treePos[20][2]
    {
        { 14.0f,  1900.0f  },
        { 240.0f, 2340.0f  },
        { 720.0f, 2590.0f  },
        { 380.0f, 2860.0f  },
        { 880.0f, 3110.0f  },

        { 1490.0f + (360.0f * 0.0f), 75.0f },
        { 1490.0f + (360.0f * 1.0f), 75.0f },
        { 1490.0f + (360.0f * 2.0f), 75.0f },
        { 1490.0f + (360.0f * 3.0f), 75.0f },
        { 1490.0f + (360.0f * 4.0f), 75.0f },
        { 1490.0f + (360.0f * 5.0f), 75.0f },


        { 1490.0f, 450.0f },
        { 1945.0f, 640.0f },

        { 2500.0f, 450.0f },
        { 2840.0f, 800.0f },

        { 3400.0f, 90.0f + (360.0f * 1.0f) },
        { 3400.0f, 90.0f + (360.0f * 2.0f) },
        { 3400.0f, 90.0f + (360.0f * 3.0f) },
        { 3400.0f, 90.0f + (360.0f * 4.0f) },

        { 3400.0f, 2120.0f }
    };

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            treePos[i][j] = (treePos[i][j] + treePos[i][j] + (float)GetRandomValue(0, 30)) / 2;
        }
    }

    for (int i = 5; i < 20; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            treePos[i][j] = (treePos[i][j] + treePos[i][j] + (float)GetRandomValue(90, 120)) / 2;
        }
    }

    for (int i = 0; i < 20; i++)
    {
        trees->at(i).LoadTextureFile();

        trees->at(i).SetPosition({ (treePos[i][0]), (treePos[i][1]) });
    }

    /*for (auto i = 0; i < 2; i++)
    {
        trees->at(i).LoadTextureFile();
        trees->at(i).SetPosition({ (float)GetRandomValue(2010, 2830), (float)GetRandomValue(50, 740) });
    }

    for (auto i = 2; i < 4; i++)
    {
        trees->at(i).LoadTextureFile();
        trees->at(i).SetPosition({ (float)GetRandomValue(2830, 3490), (float)GetRandomValue(1000, 2050) });
    }

    for (auto i = 4; i < 6; i++)
    {
        trees->at(i).LoadTextureFile();
        trees->at(i).SetPosition({ (float)GetRandomValue(400, 850), (float)GetRandomValue(2100, 2400) });
    }

    for (auto i = 6; i < 8; i++)
    {
        trees->at(i).LoadTextureFile();
        trees->at(i).SetPosition({ (float)GetRandomValue(0, 400), (float)GetRandomValue(2100, 2400) });
    }

    for (auto i = 8; i < 10; i++)
    {
        trees->at(i).LoadTextureFile();
        trees->at(i).SetPosition({ (float)GetRandomValue(0, 500), (float)GetRandomValue(2400, 3000) });
    }*/

    camera = std::make_unique<Camera2D>();

    camera->target = {};

    camera->offset = {};

    camera->rotation = 0.0f;

    camera->zoom = 1.0f;

    menuScreen.InitTexture();
    
    gameplayScreen.InitTexture();
}

void UpdateCamera(Camera2D& camera, const Player& player)
{
    Vector2D diff{ player.GetPosition().Subtract(camera.target) };

    float speed{ player.GetSpeed() };

    speed = (speed == 2.0f) ? fmaxf(0.8f * diff.Length(), 70.0f) : fmaxf(0.8f * diff.Length(), 240.0f);

    camera.offset = Vector2D{ screenWidth / 2.0f,  screenHeight / 2.0f };

    if (IsKeyPressed(KEY_C)) cameraNormalMode = !cameraNormalMode;

    if (cameraNormalMode)
    {
        if (diff.Length() > 10.0f)
        {
            camera.target = Vector2D{ camera.target }
            .Add(diff.Scale(0.394f * GetFrameTime() / diff.Length()));
        }
        else
        {
            camera.target = player.GetPosition();
        }
    }

    float minX = staticGameObj->map.GetWildanEmpireSize().width * staticGameObj->map.GetMapScale()
        + 760.0f + staticGameObj->map.GetDesertSize().width * staticGameObj->map.GetMapScale(),
        minY = staticGameObj->map.GetWildanEmpireSize().height * staticGameObj->map.GetMapScale(), 
        maxX = 0, maxY = 0;

    Rectangle rect
    { 
        0, 
        0, 
        staticGameObj->map.GetWildanEmpireSize().width * staticGameObj->map.GetMapScale() 
        + 760.0f + staticGameObj->map.GetDesertSize().width * staticGameObj->map.GetMapScale(),
        staticGameObj->map.GetWildanEmpireSize().height * staticGameObj->map.GetMapScale()
    };

    minX = fminf(rect.x, minX);
    maxX = fmaxf(rect.x + rect.width, maxX);
    minY = fminf(rect.y, minY);
    maxY = fmaxf(rect.y + rect.height, maxY);

    Vector2D max = GetWorldToScreen2D({ maxX, maxY }, camera);
    Vector2D min = GetWorldToScreen2D({ minX, minY }, camera);

    if (max.x < screenWidth) camera.offset.x = screenWidth - (max.x - screenWidth / 2);
    if (max.y < screenHeight) camera.offset.y = screenHeight - (max.y - screenHeight / 2);
    if (min.x > 0) camera.offset.x = screenWidth / 2 - min.x;
    if (min.y > 0) camera.offset.y = screenHeight / 2 - min.y;

    // don't need cinematic effect when teleporting
    if (diff.Length() > 1000.0f) camera.target = player.GetPosition();

    if (diff.Length() > 10.0f)
    {
        camera.target = Vector2D{ camera.target }.Add(diff.Scale(speed * GetFrameTime() / diff.Length()));
    }

    if ((GetMouseWheelMove() > 0.0f) && (camera.zoom < 2.0f))
    {
        camera.zoom += 0.1f;
    }
    if ((GetMouseWheelMove() < 0.0f) && (camera.zoom > 1.0f))
    {
        camera.zoom -= 0.1f;
    }
}

inline static bool OnTouch(const Player& player, float targetPosX)
{
    if (player.GetFacing() == 1.0f && player.GetPosition().x < targetPosX) return 1;
    else if (player.GetFacing() == -1.0f && player.GetPosition().x > targetPosX) return 1;

    return 0;
}

inline static Vector2D PlantPosition(const Player& player, int px = 0)
{
    Vector2D plantPosition = (player.GetFacing() == -1.0f)
        ? Vector2D{ player.GetPosition().x - 10.0f - px, player.GetPosition().y + 40.0f }
        : Vector2D{ player.GetPosition().x + 40.0f + px, player.GetPosition().y + 40.0f };

    return plantPosition;
}

inline void Timer::Start(float lifeTime)
{
    _lifeTime = lifeTime;
}

inline void Timer::Update()
{
    if (_lifeTime > 0) _lifeTime -= GetFrameTime();
}

inline bool Timer::Done() const
{
    return _lifeTime <= 0;
}

void GameplayScreen::DrawGamePlayScreen()
{
    staticGameObj->map.Draw();

    if (magicFruit != nullptr)
    {
        magicFruit->Draw();

        if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), magicFruit->GetRectangle()))
        {
            staticGameObj->player.SetStamina(1);

            delete magicFruit;

            magicFruit = nullptr;
        }
    }

    if (magicFruit == nullptr)
    {
        if (IsKeyPressed(KEY_I))
        {
            staticGameObj->player.SetStamina(0);

            if (staticGameObj->player.GetFacing() == -1.0f)
            {
                magicFruit = new MagicFruit{
                    { staticGameObj->player.GetPosition().x - 20.0f, staticGameObj->player.GetPosition().y + 40.0f } };
            }
            else
            {
                magicFruit = new MagicFruit{
                    { staticGameObj->player.GetPosition().x + 50.0f, staticGameObj->player.GetPosition().y + 40.0f } };
            }

            magicFruit->LoadTexture(LoadTexture("textures/magic_fruit/apple.png"));
        }
    }

    Vector2D flowerPos = PlantPosition(staticGameObj->player);

    if (flowers->size() < maxSmallFlowers)
    {
        if (!dynamicObjPos->empty())
        {
            if (IsKeyPressed(KEY_J) && !std::count(dynamicObjPos->begin(), dynamicObjPos->end(), flowerPos.ConcatXY()))
            {
                dynamicObjPos->push_back(flowerPos.ConcatXY());
                flowers->push_back(std::make_unique<Flower>(flowerPos));
            }
        }
        else
        {
            if (IsKeyPressed(KEY_J))
            {
                dynamicObjPos->push_back(flowerPos.ConcatXY());
                flowers->push_back(std::make_unique<Flower>(flowerPos));
            }
        }
    }

    if (!flowers->empty())
    {
        for (auto& flower : *flowers)
        {
            flower->Draw();
        }
    }

    if (staticGameObj->player.GetPosition().x < 0 || staticGameObj->player.GetPosition().y < 0 ||        
        staticGameObj->player.GetPosition().x > staticGameObj->map.GetDesertPos().x + staticGameObj->map.GetDesertSize().width * staticGameObj->map.GetMapScale() ||
        staticGameObj->player.GetPosition().y > staticGameObj->map.GetWildanEmpireSize().width * staticGameObj->map.GetMapScale() - 70.0f)
    {
        staticGameObj->player.Stop();
    }

    if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), staticGameObj->animals.crocodile.GetRectangle()) &&
        staticGameObj->player.IsPunch() && OnTouch(staticGameObj->player, staticGameObj->animals.crocodile.GetPosition().x))
    {
        staticGameObj->animals.crocodile.Hurt();
    }
    else
    {
        staticGameObj->animals.crocodile.Walk();
    }

    for (auto& rhino : staticGameObj->animals.rhinos) if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), rhino.GetRectangle()))
    {
        staticGameObj->player.Stop();
    }

    if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), staticGameObj->map.GetMapLine1()) ||
        CheckCollisionRecs(staticGameObj->player.GetRectangle(), staticGameObj->map.GetMapLine2()))
    {
        staticGameObj->player.Stop();
    }

    for (const auto& invisibleFence : invisibleFences)
    {
        if (CheckCollisionRecs(staticGameObj->player.GetRectangle(),
            Rectangle{ invisibleFence.x, invisibleFence.y, invisibleFence.width, invisibleFence.height }))
        {
            staticGameObj->player.Stop();
        }
    }

    Vector2D carrotPos = PlantPosition(staticGameObj->player);

    if (carrots->size() < maxCarrots)
    {
        if (!dynamicObjPos->empty())
        {
            if (IsKeyPressed(KEY_G) && !std::count(dynamicObjPos->begin(), dynamicObjPos->end(), carrotPos.ConcatXY()))
            {
                dynamicObjPos->push_back(carrotPos.ConcatXY());
                carrots->push_back(std::make_unique<Carrot>(carrotPos));
            }
        }
        else
        {
            if (IsKeyPressed(KEY_G))
            {
                dynamicObjPos->push_back(carrotPos.ConcatXY());
                carrots->push_back(std::make_unique<Carrot>(carrotPos));
            }
        }
    }

    if (!carrots->empty())
    {
        for (auto& carrot : *carrots)
        {
            carrot->Draw();
        }
    }

    if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), staticGameObj->prop.naturalObj.GetBigStone1Coll()))
    {
        staticGameObj->player.SetPosition({ 5300.0f, 300.0f });
    }

    if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), staticGameObj->prop.naturalObj.GetBigStone2Coll()))
    {
        staticGameObj->player.SetPosition({ 1600.0f - 80.0f, 700.0f });
    }

    bool onRiver = 0;

    for (auto& river : staticGameObj->map.GetMapRiverRects())
    {
        if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), river))
        {
            onRiver = 1;
        }
    }

    if (staticGameObj->player.GetPosition().x > (float)staticGameObj->map.GetWildanEmpireSize().width * staticGameObj->map.GetMapScale() &&
        staticGameObj->player.GetPosition().x < staticGameObj->map.GetDesertPos().x || onRiver)
    {
        staticGameObj->player.OnWater();
    }
    else
    {
        staticGameObj->player.OnLand();
    }

    // staticGameObj->animals.Draw(GetFrameTime());

    staticGameObj->player.Draw();

    for (auto& tree : *trees)
    {
        if (CheckCollisionRecs(staticGameObj->player.GetRectangle(), tree.GetRectangle()))
        {
            staticGameObj->player.Stop();
        }

        tree.Draw();
    }

    staticGameObj->prop.Draw();
}

void GameplayScreen::DrawGamePlayHUD(const Camera2D& camera, const Player& player)
{
    if (developerMode)
    {
        std::string strFPS{ "FPS: " };
        std::string strStatus{};

        int cameraMode = (cameraNormalMode) ? 1 : 0;

        time_t now = time(0);

        tm* ltm = localtime(&now);

        std::string time = { "Time: " };

        int hour = ltm->tm_hour;

        if (ltm->tm_hour > 12) hour = hour - 12;

        const char* strAmPm = (ltm->tm_hour < 12) ? "AM" : "PM";

        time.append(std::to_string(hour)).append(":").append(std::to_string(ltm->tm_min))
            .append(":").append(std::to_string(ltm->tm_sec)).append(" ").append(strAmPm);

        Vector2D diff{ player.GetPosition().Subtract(camera.target) };

        if (player.GetDirection().Length() == 0) strStatus = "Status: IDLE";
        else strStatus = (player.GetSpeed() == 4.0f) ? "Status: Walk Fast" : "Status: Walk Slow";

        DrawText(player.GetPosition().ToString().c_str(), 10, screenHeight - 50, 24, BLACK);

        std::string strCameraZoom = { "Camera Zoom: " };

        float cameraZoom = camera.zoom;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << cameraZoom;

        strCameraZoom.append(stream.str());

        DrawText(strCameraZoom.c_str(), 10, screenHeight - 124, 18, BLACK);

        DrawText(strCameraMode[cameraMode], 10, screenHeight - 90, 18, BLACK);

        const char* strStamina[7]{ "Stamina: 0", "Stamina: #", "Stamina: # #", "Stamina: # # #", "Stamina: # # # #", "Stamina: # # # # #", "Stamina: # # # # # #" };

        Color staminaColor = GREEN;

        if ((int)player.GetStamina() == 0) staminaColor = RED;
        else if ((int)player.GetStamina() > 0 && (int)player.GetStamina() < 4) staminaColor = DARKGREEN;
        else if ((int)player.GetStamina() > 4) staminaColor = GREEN;

        const char* stamina = (player.GetStamina() == 9.0f) ? "Stamina: Unlimited" : strStamina[(int)player.GetStamina()];

#ifdef _DEBUG
    stamina = "Stamina: Debug Mode";
#endif

        if (player.GetStamina() == 9.0f)
        {
            DrawText("Press `I` to spit out the magic fruit", 10, screenHeight - 180, 18, RED);
        }

        DrawText(stamina, 10, screenHeight - 150, 18, staminaColor);

        DrawText(strCameraZoom.c_str(), 10, screenHeight - 124, 18, BLACK);

        if (player.IsInvisible()) DrawText("You are invisible", 10, screenHeight - 260, 20, RED);

        if (diff.Length() > 1000.0f) teleportText.Start(4.0f);
        teleportText.Update();

        if (!teleportText.Done()) DrawCenteredText(screenHeight - 270, "You are teleported", 24, RED);

        DrawText(strFPS.append(std::to_string(GetFPS())).c_str(), screenWidth - 140, 10, 18, RED);

        DrawText(time.c_str(), screenWidth - 140, 35, 18, RED);

        DrawText(strStatus.c_str(), screenWidth - 165, screenHeight - 40, 19, RED);

        DrawVolumeBar();

        _gameplayButton.isMenu = GuiButton({ screenWidth - 165, screenHeight - 120, 120, 35 }, "Menu");

        if (_gameplayButton.isMenu)
        {
            SetWindowTitle("Menu");

            applicationState = ApplicationStates::MENU;

            PlaySound(clickSound);
        }

        DrawMuteCheckBoxV({ screenWidth - 165, screenHeight - 80 });
    } 
    else
    {
        int cameraMode = (cameraNormalMode) ? 1 : 0;

        time_t now = time(0);

        tm* ltm = localtime(&now);

        std::string time = { "Time: " };

        int hour = ltm->tm_hour;

        if (ltm->tm_hour > 12) hour = hour - 12;

        const char* strAmPm = (ltm->tm_hour < 12) ? "AM" : "PM";

        time.append(std::to_string(hour)).append(":").append(std::to_string(ltm->tm_min))
            .append(":").append(std::to_string(ltm->tm_sec)).append(" ").append(strAmPm);

        Vector2D diff{ player.GetPosition().Subtract(camera.target) };

        DrawTextureV(_HUDFillBar05, { screenWidth - 210, 8 }, WHITE);

        // DrawText(player.GetPosition().ToString().c_str(), 10, screenHeight - 50, 24, BLACK);

        std::string strCameraZoom = { "Camera Zoom: " };

        float cameraZoom = camera.zoom;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << cameraZoom;

        strCameraZoom.append(stream.str());

        DrawText(strCameraZoom.c_str(), 10, screenHeight - 124, 18, BLACK);

        DrawText(strCameraMode[cameraMode], 10, screenHeight - 90, 18, BLACK);

        const char* strStamina[7]{ "Stamina: 0", "Stamina: #", "Stamina: # #", "Stamina: # # #", "Stamina: # # # #", "Stamina: # # # # #", "Stamina: # # # # # #" };

        Color staminaColor = GREEN;

        if ((int)player.GetStamina() == 0) staminaColor = RED;
        else if ((int)player.GetStamina() > 0 && (int)player.GetStamina() < 4) staminaColor = DARKGREEN;
        else if ((int)player.GetStamina() > 4) staminaColor = GREEN;

        const char* stamina = (player.GetStamina() == 9.0f) ? "Stamina: Unlimited" : strStamina[(int)player.GetStamina()];

#ifdef _DEBUG
        stamina = "Stamina: Debug Mode";
#endif

        if (player.GetStamina() == 9.0f)
        {
            DrawText("Press `I` to spit out the magic fruit", 10, screenHeight - 180, 18, RED);
        }

        DrawText(stamina, 10, screenHeight - 150, 18, staminaColor);

        DrawText(strCameraZoom.c_str(), 10, screenHeight - 124, 18, BLACK);

        if (player.IsInvisible()) DrawText("You are invisible", 10, screenHeight - 260, 20, RED);

        if (diff.Length() > 1000.0f) teleportText.Start(4.0f);
        teleportText.Update();

        if (!teleportText.Done()) DrawCenteredText(screenHeight - 270, "You are teleported", 24, RED);

        DrawText(time.c_str(), screenWidth - 188, 14, 20, WHITE);

        DrawVolumeBar();

        _gameplayButton.isMenu = GuiButton({ screenWidth - 140, screenHeight - 80, 120, 35 }, "Menu");

        if (_gameplayButton.isMenu)
        {
            SetWindowTitle("Menu");

            applicationState = ApplicationStates::MENU;

            PlaySound(clickSound);
        }

        DrawMuteCheckBoxV({ screenWidth - 140, screenHeight - 40 });
    }
}

void DrawVolumeBar(const int x)
{
    const char* strVolume[6]{ "Muted", "Volume: #", "Volume: # #", "Volume: # # #", "Volume: # # # #", "Volume: # # # # #" };

    const Color volumeColors[4]{ RED, DARKGREEN, LIME, GREEN };

    int volume = (int)(audio.masterVolume * 10);

    if (volume > 5) volume = volume - 1;

    if (audio.masterVolume < 0.1f || !audioTimer.Done())
    {
        int volumeColor = 2;

        if (audio.masterVolume < 0.1f) volumeColor = 0;
        else if (audio.masterVolume > 0.1f && audio.masterVolume < 0.3f) volumeColor = 1;
        else if (audio.masterVolume > 0.3f && audio.masterVolume < 0.4f) volumeColor = 2;
        else if (audio.masterVolume > 0.4f) volumeColor = 3;

        DrawText(strVolume[volume], x, 10, 24, volumeColors[volumeColor]);
    }
}

void DrawMuteCheckBoxV(const Vector2D pos)
{
    const char* strMuteCheckBox = (audio.masterVolume < 0.1f) ? "Muted: True" : "Muted: False";

    GuiControlState state = guiState;
    Rectangle textBounds  = { 0 };
    Rectangle bounds      = { pos.x, pos.y, 20, 20 };
    
    textBounds.width  = (float)GetTextWidth(strMuteCheckBox);
    textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
    
    textBounds.x = bounds.x + bounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING);
    textBounds.y = bounds.y + bounds.height / 2 - GuiGetStyle(DEFAULT, TEXT_SIZE) / 2;

    if (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == GUI_TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(CHECKBOX, TEXT_PADDING);

    if ((state != GUI_STATE_DISABLED) && !guiLocked)
    {
        const Vector2D mousePoint = GetMousePosition();

        Rectangle totalBounds = {
            (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == GUI_TEXT_ALIGN_LEFT) ? textBounds.x : bounds.x,
            bounds.y,
            bounds.width + textBounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING),
            bounds.height,
        };

        if (CheckCollisionPointRec(mousePoint, totalBounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = GUI_STATE_PRESSED;
            else state = GUI_STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                audio.muted = !audio.muted;

                PlaySound(clickSound);
            }
        }
    }

    GuiDrawRectangle(bounds, GuiGetStyle(CHECKBOX, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(CHECKBOX, BORDER + (state * 3))), guiAlpha), BLANK);

    if (audio.muted || audio.masterVolume < 0.1f)
    {
        Rectangle check = { bounds.x + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.y + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.width - 2 * (GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)),
                            bounds.height - 2 * (GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)) };
        GuiDrawRectangle(check, 0, BLANK, Fade(GetColor(GuiGetStyle(CHECKBOX, TEXT + state * 3)), guiAlpha));
    }

    GuiDrawText(strMuteCheckBox, textBounds,
        (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == GUI_TEXT_ALIGN_RIGHT) ? GUI_TEXT_ALIGN_LEFT : GUI_TEXT_ALIGN_RIGHT,
        Fade(GetColor(GuiGetStyle(LABEL, TEXT + (state * 3))), guiAlpha));
}

static void* LoadDataThread()
{
    int timeCounter = 0;            
    clock_t prevTime = clock();

    while (timeCounter < 4000)
    {
        clock_t currentTime = clock() - prevTime;
        timeCounter = currentTime * 1000 / CLOCKS_PER_SEC;

        dataProgress = timeCounter / 10;
    }

    atomic_store(&dataLoaded, true);

    return nullptr;
}

void UpdateVolumeTimer()
{
    if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_L) || IsKeyPressed(KEY_K)) audioTimer.Start(4.0f);

    audioTimer.Update();
}

void UpdateGame()
{
    UpdateAudio();

    UpdateVolumeTimer();

    switch (applicationState)
    {
    case ApplicationStates::LOGO:
    {
        UpdateMusicStream(logoBGM);

        PlayMusicStream(logoBGM);

        framesCounter++;

        if (framesCounter > 180)
        {
            SetActiveScreen(&titleScreen);

            applicationState = ApplicationStates::TITLE;

            StopMusicStream(logoBGM);

            logoBGM.frameCount = 0;

            UnloadMusicStream(logoBGM);

            framesCounter = 0;
        }

    } break;
    case ApplicationStates::TITLE:
    {
        UpdateMusicStream(mainBGM);

        PlayMusicStream(mainBGM);

        if (IsKeyPressed(KEY_ENTER))
        {
            SetActiveScreen(&gameplayScreen);

            // applicationState = ApplicationStates::GAMEPLAY;

            PauseMusicStream(mainBGM);

            loadDataThread = std::thread(std::ref(LoadDataThread));

            applicationState = ApplicationStates::LOADING;
        }

    } break;
    case ApplicationStates::LOADING:
    {
        framesCounter++;
        
        if (atomic_load(&dataLoaded))
        {
            framesCounter = 0;

            applicationState = ApplicationStates::LOADING_FINISHED;
        }

    } break;
    case ApplicationStates::LOADING_FINISHED:
    {
        framesCounter++;

        if (framesCounter > 120)
        {
            framesCounter = 0;

            applicationState = ApplicationStates::GAMEPLAY;

            loadDataThread.join();
        }

    } break;
    case ApplicationStates::GAMEPLAY:
    {
        UpdateMusicStream(birdBGM);

        PlayMusicStream(birdBGM);

        if (IsKeyPressed(KEY_P))
        {
            SetActiveScreen(&pauseScreen);

            SetWindowTitle("Paused");

            applicationState = ApplicationStates::PAUSE;

            PauseMusicStream(birdBGM);
        }
        if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
        {
            SetActiveScreen(&menuScreen);

            SetWindowTitle("Menu");

            applicationState = ApplicationStates::MENU;

            PauseMusicStream(birdBGM);
        }

    } break;
    case ApplicationStates::MENU:
    {
        UpdateMusicStream(menuBGM);

        PlayMusicStream(menuBGM);

        SetActiveScreen(&menuScreen);

        if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
        {
            SetActiveScreen(&gameplayScreen);

            SetWindowTitle("Walking");

            applicationState = ApplicationStates::GAMEPLAY;

            PauseMusicStream(menuBGM);
        }

    } break;
    case ApplicationStates::PAUSE:
    {
        UpdateMusicStream(mainBGM);

#ifdef _DEBUG
    PlayMusicStream(mainBGM);
#else
    ResumeMusicStream(mainBGM);
#endif

        if (IsKeyPressed(KEY_P))
        {
            SetActiveScreen(&gameplayScreen);

            SetWindowTitle("Walking");

            applicationState = ApplicationStates::GAMEPLAY;

            PauseMusicStream(mainBGM);
        }

    } break;
    default: break;
    }

    BeginDrawing();

    ClearBackground(SKYBLUE);

    switch (applicationState)
    {
    case ApplicationStates::LOADING:
    {
        DrawRectangle(58, 200, dataProgress, 60, WHITE);
        
        if ((framesCounter / 15) % 2) DrawText("LOADING DATA...", 105, 210, 40, LIGHTGRAY);

    } break;
    case ApplicationStates::LOADING_FINISHED:
    {
        DrawRectangle(58, 200, 400, 60, LIME);
        
        DrawText("DATA LOADED!", 105, 210, 40, GREEN);

    } break;
    default: break;
    }
    if (applicationState == ApplicationStates::LOADING || applicationState == ApplicationStates::LOADING_FINISHED)
    {
        DrawRectangleLines(58, 200, 400, 60, DARKGRAY);
    }

    //if (applicationState == ApplicationStates::GAMEPLAY)
    //{
    //    UpdateCamera(*camera, staticGameObj->player);

    //    // staticGameObj->CheckCollision();

    //    // staticGameObj->PlayWalkSound();

    //    // camera->BeginMode();

    //    BeginMode2D(*camera);

    //    DrawGamePlayScreen();

    //    // camera->EndMode();

    //    EndMode2D();

    //    gameplayButton.isMenu = GuiButton({ screenWidth - 165, screenHeight - 120, 120, 35 }, "Menu");

    //    if (gameplayButton.isMenu)
    //    {
    //        SetActiveScreen(&menuScreen);

    //        SetWindowTitle("Menu");

    //        applicationState = ApplicationStates::MENU;

    //        PlaySound(clickSound);
    //    }

    //    DrawGamePlayHUD(*camera, staticGameObj->player);
    //}

    DrawScreen();

    EndDrawing();
}

void EndGame()
{
    if (magicFruit != nullptr)
    {
        delete magicFruit;
    }

    if (!flowers->empty())
    {
        flowers->clear();
    }

    if (!carrots->empty())
    {
        flowers->clear();
    }

    if (!dynamicObjPos->empty())
    {
        dynamicObjPos->clear();
    }

    UnloadSound(clickSound);

    menuScreen.FreeTexture();

    gameplayScreen.FreeTexture();

    UnloadScreenTexture();

    ShutdownAudio();

    CloseWindow();
}

void InitAudio()
{
    InitAudioDevice();

    InitBGM();
}

void UpdateAudio()
{
    if (IsKeyPressed(KEY_M)) audio.muted = !audio.muted;

    if (IsKeyPressed(KEY_L) && audio.masterVolume <= 0.5f && !audio.muted)
    {
        audio.masterVolume += 0.1f;
        audio.currentVolume += 0.1f;
    }
    if (IsKeyPressed(KEY_K) && audio.masterVolume >= 0.1f && !audio.muted)
    {
        audio.masterVolume -= 0.1f;
        audio.currentVolume -= 0.1f;
    }

    if (audio.muted  && IsKeyPressed(KEY_M) || audio.muted) audio.masterVolume  = 0.0f;
    if (!audio.muted && IsKeyPressed(KEY_M) || !audio.muted) audio.masterVolume = audio.currentVolume;

    if (IsKeyPressed(KEY_L) && audio.muted || IsKeyPressed(KEY_K) && audio.muted)
    {
        audio.muted = !audio.muted;
        audio.masterVolume = audio.currentVolume;
    }

    SetMasterVolume(audio.masterVolume);
}

void InitBGM()
{
    mainBGM = LoadMusicStream("sounds/epic_cinematic_trailer_elite.ogg");
    logoBGM = LoadMusicStream("sounds/magic_astral_sweep_effect.ogg");
    birdBGM = LoadMusicStream("sounds/birds-isaiah658.ogg");
    menuBGM = LoadMusicStream("sounds/harp.ogg");

    logoBGM.looping = 0;
    mainBGM.looping = 1;
    birdBGM.looping = 1;
    menuBGM.looping = 1;
}

void StopBGM()
{
    if (mainBGM.frameCount > 0)
    {
        StopMusicStream(mainBGM);

        mainBGM.frameCount = 0;

        UnloadMusicStream(mainBGM);
    }

    if (birdBGM.frameCount > 0)
    {
        StopMusicStream(birdBGM);

        birdBGM.frameCount = 0;

        UnloadMusicStream(birdBGM);
    }

    if (menuBGM.frameCount > 0)
    {
        StopMusicStream(menuBGM);

        menuBGM.frameCount = 0;

        UnloadMusicStream(menuBGM);
    }

    if (applicationState == ApplicationStates::LOGO)
    {
        StopMusicStream(logoBGM);

        logoBGM.frameCount = 0;

        UnloadMusicStream(logoBGM);
    }
}

void ShutdownAudio()
{
    StopBGM();

    CloseAudioDevice();
}

int main()
{
    InitGame();

    while (!endGame)
    {
        endGame = WindowShouldClose();

        UpdateGame();
    }

    EndGame();

    return 0;
}