#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
	class LineTestObject : public GameObject
	{
		TYPE_DECALRATIONS(LineTestObject, GameObject)
	public:
		void Draw() override;
	};


}
