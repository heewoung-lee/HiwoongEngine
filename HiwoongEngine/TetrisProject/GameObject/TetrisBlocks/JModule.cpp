#include "TetrisModule.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Engine/Engine.h"
#include <vector>

namespace Hiwoong
{
	JModule::JModule(const Vector2& position)
		: TetrisModule(position)
	{
		blockPositions = {
			Vector2(0,0),
			Vector2(1,0),
			Vector2(2,0),
			Vector2(3,0)
		};

		color = Color::Blue;

	}

	const std::array<Vector2, 4>& JModule::GetBlockPosition() const
	{
		return blockPositions;
	}

	Color JModule::GetColor()
	{
		return color;
	}

}

