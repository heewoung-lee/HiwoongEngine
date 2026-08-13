#include "TetrisModule.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Engine/Engine.h"
#include <vector>

namespace Hiwoong
{
	TModule::TModule(const Vector2& position)
		: TetrisModule(position)
	{
		blockPositions = {
			Vector2(0, 0),
			Vector2(1, 0),
			Vector2(2, 0),
			Vector2(1, 1)
		};

		color = Color::Magenta;

	}

	const std::array<Vector2, 4>& TModule::GetBlockPosition() const
	{
		return blockPositions;
	}

	Color TModule::GetColor()
	{
		return color;
	}

}

