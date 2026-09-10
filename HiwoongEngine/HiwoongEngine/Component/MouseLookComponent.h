#pragma once

#include "Core/Core.h"
#include "Component/Component.h"

namespace Hiwoong
{
	class Hiwoong_API MouseLookComponent : public Component
	{
		TYPE_DECALRATIONS(MouseLookComponent, Component)

	public:
		void Update(double deltatime) override;

	private:
		float sensitivity = 0.003f;
	};
}
