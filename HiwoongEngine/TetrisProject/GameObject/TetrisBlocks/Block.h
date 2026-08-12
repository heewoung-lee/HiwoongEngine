#pragma once
#include "GameObject/GameObject.h"


namespace Hiwoong
{
	class Block: public GameObject
	{
	public:
		Block(const Vector2& position, const Color& color);
		~Block() = default;
	};

}


