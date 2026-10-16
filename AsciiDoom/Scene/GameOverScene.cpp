#include "GameOverScene.h"
#include "Render/Renderer.h"
#include "Menu/GameOverTitle.h"
#include "Menu/RestartButton.h"
#include "Menu/QuitButton.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Menu/MenuCursor.h"
#include <cassert>
namespace Hiwoong
{
    GameOverScene::GameOverScene(
        const std::function<void()>& onRestart)
        : onRestart(onRestart)
    {
        assert(this->onRestart);
    }

    void GameOverScene::SceneInitialize()
    {
        Scene::SceneInitialize();

        SetScreenSize(Renderer::Get().GetScreenSize());

        Input::Get().SetMouseLocked(false);
        Instantiate<GameOverTitle>();

        restartButton = Instantiate<RestartButton>();
        quitButton = Instantiate<QuitButton>();

        menuCursor = Instantiate<MenuCursor>();
        menuCursor->SetTarget(restartButton);
    }

    void GameOverScene::Update(double deltaTime)
    {
        Scene::Update(deltaTime);
        ProcessKeyboardInput();
    }

    void GameOverScene::ProcessKeyboardInput()
    {
        if (Input::Get().GetKeyDown(VK_UP) ||
            Input::Get().GetKeyDown(VK_DOWN))
        {
            selectedIndex = 1 - selectedIndex;

            menuCursor->SetTarget(
                selectedIndex == 0 ? restartButton : quitButton
            );
        }

        if (Input::Get().GetKeyDown(VK_RETURN))
        {
            ExecuteSelectedMenu();
        }
    }

    void GameOverScene::ExecuteSelectedMenu()
    {
        if (selectedIndex == 0)
        {
            onRestart();
            return;
        }

        Engine::Get().Quit();
    }
}