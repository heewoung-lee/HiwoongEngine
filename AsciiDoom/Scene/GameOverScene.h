#pragma once

#include "Scene/Scene.h"
#include <functional>

namespace Hiwoong
{
    class MenuCursor;

    class GameOverScene : public Scene
    {
    public:
        explicit GameOverScene(
            const std::function<void()>& onRestart
        );
        void Update(double deltaTime) override;
        void SceneInitialize() override;

    private:
        std::function<void()> onRestart;

        void ProcessKeyboardInput();
        void ExecuteSelectedMenu();

        int selectedIndex = 0; // 0: Restart, 1: Quit

        std::shared_ptr<GameObject> restartButton;
        std::shared_ptr<GameObject> quitButton;
        std::shared_ptr<MenuCursor> menuCursor;

    };
}