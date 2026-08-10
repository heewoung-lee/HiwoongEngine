#pragma once

#include "Component/Component.h"

namespace Hiwoong
{
	class  PlayerInputComponent : public Hiwoong::Component
	{
		TYPE_DECALRATIONS(PlayerInputComponent, Component)

	public:
		PlayerInputComponent();
		~PlayerInputComponent() = default;

		virtual void Start() override;
		virtual void Update(double deltaTime)override;


	private:
		GameObject* player = nullptr;
	};

}
