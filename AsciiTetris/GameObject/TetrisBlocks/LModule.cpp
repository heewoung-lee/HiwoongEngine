#include "TetrisModule.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Engine/Engine.h"
#include <vector>

namespace Hiwoong
{
	LModule::LModule(const Vector2& position)
		: TetrisModule(position)
	{
		blockPositions = {
			Vector2(0, 0),
			Vector2(0, 1),
			Vector2(0, 2),
			Vector2(1, 2)
		};
		rotationPivot = Vector2(1, 1);
		color = Color::Orange;

	}

	Color LModule::GetColor()
	{
		return color;
	}

}

