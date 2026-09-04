#include "GameObject/TextObject.h"
#include "GameStatesUI.h"
#include "Scene/Scene.h"
#include "Manager/TetrisGameState.h"
#include <string>
#include <cassert>


namespace Hiwoong
{
	GameStatesUI::GameStatesUI(const Vector2& position) :GameObject(Vector3(position.x,position.y,0))
	{
	}
	void GameStatesUI::Start()
	{
		GameObject::Start();

		std::shared_ptr<Scene> scene = GetOwner();
		assert(scene != nullptr);

		auto position = GetWorldPosition();

		const Vector2 origin(position.x, position.y);

		//Fill horizontal to '-' not boarder
		const std::string horizontal(Width - 2, '-');

		//Add Boader
		const std::string border = "+" + horizontal + "+";
		
		//Top
		scene->Instantiate<TextObject>(origin,border,Color::BrightWhite);
		
		//Bottom
		scene->Instantiate<TextObject>(origin + Vector2(0,Height-1), border, Color::BrightWhite);


		for (int y = 1; y < Height - 1; ++y)
		{
			//left
			scene->Instantiate<TextObject>(
				origin + Vector2(0, y),
				"|",
				Color::BrightWhite
			);


			//right
			scene->Instantiate<TextObject>(
				origin + Vector2(Width - 1, y),
				"|",
				Color::BrightWhite
			);
		}


		//
		scoreText = scene->Instantiate<TextObject>(
			origin + Vector2(2, 2),
			"",
			Color::White
		);

		levelText = scene->Instantiate<TextObject>(
			origin + Vector2(2, 4),
			"",
			Color::White
		);

		requireScoreText = scene->Instantiate<TextObject>(
			origin + Vector2(2, 6),
			"",
			Color::White
		);

		scene->Instantiate<TextObject>(
			origin + Vector2(2, 8),
			"NEXT BLOCK",
			Color::White
		);

		for (std::size_t index = 0;
			index < nextBlockTexts.size();
			++index)
		{
			nextBlockTexts[index] =
				scene->Instantiate<TextObject>(
					origin + Vector2(2, 10),
					"@",
					Color::White
				);
		}

		Refresh();


	}

	// display when state values change 
	void GameStatesUI::Update(double deltaTime)
	{
		GameObject::Update(deltaTime);

		const TetrisGameState& state =
			TetrisGameState::Get();


		if (state.GetCurrentLevel() != displayedLevel ||
			state.GetRequireNextLevelScore() != displayedRequireScore ||
			state.GetScore() != displayedScore||
			state.GetNextModuleNumber() != displayedNextModule)
		{
			displayedScore = state.GetScore();
			displayedLevel = state.GetCurrentLevel();
			displayedRequireScore = state.GetRequireNextLevelScore();
			displayedNextModule = state.GetNextModuleNumber();
			Refresh();
		}

	}
	void GameStatesUI::Refresh()
	{
		const TetrisGameState& state = TetrisGameState::Get();


		if (std::shared_ptr<TextObject> score = scoreText.lock())
		{
			score->SetText("Score: " + std::to_string(state.GetScore()));
		}

		if (std::shared_ptr<TextObject> level = levelText.lock())
		{
			level->SetText("Level: " + std::to_string(state.GetCurrentLevel()));
		}

		if (std::shared_ptr<TextObject> requireScore = requireScoreText.lock())
		{
			requireScore->SetText("NEXT: " + std::to_string(state.GetRequireNextLevelScore()));
		}
	
		RefreshNextBlock(state.GetNextModuleNumber());
	}
	void GameStatesUI::RefreshNextBlock(int moduleNumber)
	{
		std::array<Vector2, 4> positions;
		Color color = Color::White;

		switch (moduleNumber)
		{
		case 0:
			positions = { Vector2(0,0), Vector2(1,0),
						  Vector2(2,0), Vector2(3,0) };
			color = Color::Cyan;
			break;

		case 1:
			positions = { Vector2(0,0), Vector2(1,0),
						  Vector2(0,1), Vector2(1,1) };
			color = Color::Yellow;
			break;

		case 2:
			positions = { Vector2(0,0), Vector2(1,0),
						  Vector2(2,0), Vector2(1,1) };
			color = Color::Magenta;
			break;

		case 3:
			positions = { Vector2(0,0), Vector2(0,1),
						  Vector2(0,2), Vector2(1,2) };
			color = Color::Orange;
			break;

		case 4:
			positions = { Vector2(0,0), Vector2(0,1),
						  Vector2(1,1), Vector2(2,1) };
			color = Color::Blue;
			break;

		case 5:
			positions = { Vector2(1,0), Vector2(2,0),
						  Vector2(0,1), Vector2(1,1) };
			color = Color::Green;
			break;

		case 6:
			positions = { Vector2(0,0), Vector2(1,0),
						  Vector2(1,1), Vector2(2,1) };
			color = Color::Red;
			break;

		default:
			return;
		}

		//Show POsition

		Vector3 worldPos = GetWorldPosition();
		const Vector2 previewOrigin =Vector2(worldPos.x, worldPos.y) + Vector2(4, 10);


		for (std::size_t index = 0; index < nextBlockTexts.size();++index)
		{
			auto blockText = nextBlockTexts[index].lock();

			if (blockText == nullptr)
			{
				continue;
			}

			Vector2 setPos = previewOrigin + positions[index];

			blockText->SetPosition(Vector3(setPos.x, setPos.y,0));

			blockText->SetColor(color);
		}
		
	}
}