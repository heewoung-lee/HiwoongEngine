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
			Vector2(0, 0),
			Vector2(0, 1),
			Vector2(1, 1),
			Vector2(2, 1)
		};
		rotationPivot = Vector2(1, 1);
		color = Color::Blue;

	}

	Color JModule::GetColor()
	{
		return color;
	}

}

