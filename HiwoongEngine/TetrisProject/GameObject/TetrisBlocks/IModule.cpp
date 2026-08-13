#include "TetrisModule.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Engine/Engine.h"
#include <vector>

namespace Hiwoong
{
	IModule::IModule(const Vector2& position)
		: TetrisModule(position)
	{
		blockPositions = {
			Vector2(0,0),
			Vector2(1,0),
			Vector2(2,0),
			Vector2(3,0)
		};

		color = Color::Cyan;

	}



	//스타트를 구현해서 Imodule의 블록들을 스폰할 것
	const std::array<Vector2, 4>& IModule::GetBlockPosition() const
	{
		return blockPositions;
	}

	Color IModule::GetColor()
	{
		return color;
	}

}

