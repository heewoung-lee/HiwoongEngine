#pragma once

#include "Scene/Scene.h"



namespace Hiwoong
{
	class SpriteRendererComponent;

	class MenuScene : public Scene
	{

	public:
		void SceneInitialize() override;
		void Update(double deltaTime) override;


	private:
		void ProcessKeyboardInput(); // 방향키 선택 이동 + Enter 실행
		void UpdateHighlight();      // 선택 상태에 따라 색상 변경
		bool ISCancelMenu();
	private:
		// 0: 재개, 1: 종료
		int selectedIndex = 0;

		std::shared_ptr<SpriteRendererComponent> resumeRenderer;
		std::shared_ptr<SpriteRendererComponent> quitRenderer;
	};
}


