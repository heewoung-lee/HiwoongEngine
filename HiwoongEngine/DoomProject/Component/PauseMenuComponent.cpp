#include "PauseMenuComponent.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Scene/MenuScene.h"

namespace Hiwoong
{
	void PauseMenuComponent::Update(double deltaTime)
	{
		super::Update(deltaTime);

		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			//ESC를 눌렀을때 MenuScene 을 메인메뉴로 갈아끼기
			Engine::Get().OpenPauseScene<MenuScene>();



		}
	}
}


