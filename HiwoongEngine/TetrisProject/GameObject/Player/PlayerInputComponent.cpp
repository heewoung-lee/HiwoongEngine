#include "GameObject/Player/PlayerInputComponent.h"
#include "Core/Input.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

#include <windows.h>
#include <cassert>

namespace Hiwoong
{
	PlayerInputComponent::PlayerInputComponent()
	{
	}

	
	void PlayerInputComponent::Start()
	{
		std::shared_ptr<GameObject> owner = GetOwner();
		assert(owner != nullptr);

		player = owner.get();

	}

	void PlayerInputComponent::Update(double deltaTime)
	{
		super::Update(deltaTime);

		Vector2 newPosition = player->GetPosition();

		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			player->QuitGame();
		}

		if (Input::Get().GetKey(VK_LEFT) && newPosition.x > 0)
		{
			newPosition.x -= 1;

		}

		if (Input::Get().GetKey(VK_RIGHT) && newPosition.x < GetScene()->GetScreenSize().x-1)
		{
			newPosition.x += 1;
		}

		if (Input::Get().GetKey(VK_UP) && newPosition.y > 0)
		{
			newPosition.y -= 1;
		}


		if (Input::Get().GetKey(VK_DOWN) && newPosition.y < GetScene()->GetScreenSize().y - 1)
		{
			newPosition.y += 1;
		}

		player->SetPosition(newPosition);

	}
}