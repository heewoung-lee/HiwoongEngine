#include "TetrisModule.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Engine/Engine.h"
#include <vector>

namespace Hiwoong
{
	SModule::SModule(const Vector2& position)
		: TetrisModule(position)
	{
		blockPositions = {
			 Vector2(1, 0),
			Vector2(2, 0),
			Vector2(0, 1),
			Vector2(1, 1)
		};

		color = Color::Green;

	}

	const std::array<Vector2, 4>& SModule::GetBlockPosition() const
	{
		return blockPositions;
	}

	Color SModule::GetColor()
	{
		return color;
	}

}

