#pragma once

#include "GameObject/GameObject.h"
#include <memory>
#include <array>
namespace Hiwoong
{
	class TextObject;

	class GameStatesUI : public GameObject
	{
	public:
		GameStatesUI(const Vector2& position);
		//fixed width, height for view State UI
		static constexpr int Width = 18;
		static constexpr int Height = 15;


		void Start() override;
		void Update(double deltaTime) override;
		void Refresh();

		void RefreshNextBlock(int moduleNumber);

	private:
		std::weak_ptr<TextObject> scoreText;
		std::weak_ptr<TextObject> levelText;
		std::weak_ptr<TextObject> requireScoreText;

		int displayedScore = -1;
		int displayedLevel = -1;
		int displayedRequireScore = -1;
		int displayedNextModule = -2;

		std::array<std::weak_ptr<TextObject>,4> nextBlockTexts;
	};
}

