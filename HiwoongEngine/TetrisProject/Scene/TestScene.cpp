#include "TestScene.h"
#include "GameObject/TestGameObject.h"
#include "Math/Vector2.h"
#include "Scene/Scene.h"
#include "GameObject/BackGround/Wall.h"
#include "GameObject/BackGround/BackGround.h"
#include <fstream>
#include <cassert>

#include <iostream>
#include <algorithm>
#include "GameObject/SpawnManager/SpawnManager.h"

namespace Hiwoong
{
	TestScene::TestScene(int level) : Scene(), currentLevel(level)
	{
			
	}
	TestScene::~TestScene()
	{
	}
	double TestScene::CalculateDropInterval() const
	{
		return (std::max)(
			0.1,
			1.0 - (currentLevel - 1) * 0.1
		);
	}

	void TestScene::SceneInitialize()
	{
		Vector2 screenSize = LoadMap("map.txt");
		Instantiate<SpawnManager>(Vector2(4,4));
		Scene::SetScreenSize(screenSize);
		Scene::SceneInitialize();
		
	}

	Vector2 TestScene::LoadMap(const std::string& filename)
	{
		std::vector<Vector2> wallPositions;

		// Path
		std::string path = std::string("../Assets/Stages/") + filename;

		// File Open and Check
		std::ifstream file(path, std::ios_base::binary);
		assert(file.is_open());

		// Check file's whole length.
		file.seekg(0, std::ios_base::end);
		const std::streampos fileSize = file.tellg();

		//revert file location to first
		file.seekg(0, std::ios_base::beg);

		// variable to read whole file's contends 
		std::string buffer;
		buffer.resize(static_cast<size_t>(fileSize));

		//To Read File's contends
		file.read(&buffer[0], fileSize);

		// read a string buffer step by step
		int index = 0;
		Vector2 position;


		while (true)
		{
			//quit
			if (index >= fileSize)
			{
				break;
			}

			//read current char
			char mapCharacter = buffer[index];

			//for reading next char
			++index;

			// Windows의 개행 문자는 '\n' 이 아니라 '\r\n' 이기 때문에
			// \r를 건너뛰기 해야함
			if (mapCharacter == '\r') continue;

			// 개행 문자 처리 - 좌표값 업데이트
			if (mapCharacter == '\n')
			{
				++position.y;
				position.x = 0;
				continue;
			}

			// 읽은 문자 별로 처리.

			switch (mapCharacter)
			{
			case '#':
				Instantiate<Wall>(position);
				wallPositions.emplace_back(position);
				break;
			case '.':
				Instantiate<BackGround>(position);
				break;
			}
			
			//문자 처리후 x 위치 업데이트.
			++position.x;
		}
		//파일닫기


		file.close();
		++position.y;
		const Vector2 screenSize = position;

		board = std::make_unique<TetrisBoard>(
			screenSize.x,
			screenSize.y
		);

		for (const Vector2& wallPosition : wallPositions)
		{
			board->SetOccupied(wallPosition, true);
		}



		return position;
	}

}

