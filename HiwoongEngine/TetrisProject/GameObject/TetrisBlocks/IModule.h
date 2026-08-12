#pragma once

#include <array>
#include "Interface/TetrisModule.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
namespace Hiwoong 
{
	class IModule : public TetrisModule
	{
		TYPE_DECALRATIONS(IModule, TetrisModule)
	public:
		IModule(const Vector2& position);
		const std::array<Vector2, 4>& GetBlockPosition() const override;
		Color GetColor();
	
	
	private:
		std::array<Vector2, 4>blockPositions;
		Color color;
	};

}


