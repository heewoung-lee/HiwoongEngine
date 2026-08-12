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
		Instantiate<TestGameObject>();
		Instantiate<SpawnManager>(Vector2(4,4));
		Vector2 screenSize = LoadMap("map.txt");
		Scene::SetScreenSize(screenSize);
		Scene::SceneInitialize();
	}

	Vector2 TestScene::LoadMap(const std::string& filename)
	{
		// 최종 경로.
		std::string path = std::string("../Assets/Stages/") + filename;

		// 파일 열기 및 확인.
		std::ifstream file(path, std::ios_base::binary);
		assert(file.is_open());

		// 파일 전체 크기 확인.
		file.seekg(0, std::ios_base::end);
		const std::streampos fileSize = file.tellg();

		//파일 위치를 첫 위치로 되돌리기.
		file.seekg(0, std::ios_base::beg);

		// 파일 내용 전체를 읽기 위한 변수.
		std::string buffer;
		buffer.resize(static_cast<size_t>(fileSize));

		//파일 내용 전체 읽기
		file.read(&buffer[0], fileSize);

		// 문자열 buffer를 한문자씩 읽으면서 처리.
		int index = 0;
		Vector2 position;


		while (true)
		{
			// 종료조건
			if (index >= fileSize)
			{
				break;
			}

			// 현재 순번의 문자 값 읽기.
			char mapCharacter = buffer[index];

			// 다음 글자를 읽기 위한 준비.
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
		return position;
	}

}

