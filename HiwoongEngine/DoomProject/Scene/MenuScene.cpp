#include "MenuScene.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Menu/MenuBackground.h"
#include "Menu/MenuFrame.h"
#include "Menu/PauseTitle.h"
#include "Menu/ResumeButton.h"
#include "Menu/QuitButton.h"
#include "Core/Input.h"
#include "Engine/Engine.h"

namespace Hiwoong
{
	void MenuScene::SceneInitialize()
	{
		Scene::SceneInitialize();

		const auto size = GetScreenSize();
		Instantiate<MenuBackground>();
		Instantiate<MenuFrame>();
		Instantiate<PauseTitle>();
		
		auto resumeButton = Instantiate<ResumeButton>();
		auto quitButton = Instantiate<QuitButton>();

		resumeRenderer =
			resumeButton->GetComponent<SpriteRendererComponent>();

		quitRenderer =
			quitButton->GetComponent<SpriteRendererComponent>();


	}

	void MenuScene::ProcessKeyboardInput()
	{
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			if (selectedIndex == 0)
			{
				Engine::Get().ResumeScene();
			}
			else
			{
				Engine::Get().Quit();
			}
		}
	}

	void MenuScene::UpdateHighlight()
	{
		if (Input::Get().GetKeyDown(VK_UP))
		{
			selectedIndex = (2 + selectedIndex - 1) % 2;
		}
		else if (Input::Get().GetKeyDown(VK_DOWN))
		{
			selectedIndex = (selectedIndex + 1) % 2;
		}

		resumeRenderer->SetColor(
			selectedIndex == 0 ? Color::Green : Color::Gray
		);

		quitRenderer->SetColor(
			selectedIndex == 1 ? Color::Green : Color::Gray
		);
	}

	void MenuScene::CancelMenu()
	{
		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			Engine::Get().ResumeScene();
			return;
		}
	}

	void MenuScene::Update(double deltaTime)
	{
		Scene::Update(deltaTime);

		CancelMenu();
		ProcessKeyboardInput();
		UpdateHighlight();

	}



}
