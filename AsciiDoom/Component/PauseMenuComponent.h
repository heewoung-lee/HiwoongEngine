#pragma once

#include "Component/Component.h"


namespace Hiwoong
{
	class PauseMenuComponent : public Component
	{
		TYPE_DECALRATIONS(PauseMenuComponent,Component)

	public:
		void Update(double deltatime) override;

	};
}
