#include "GameObject/Player/PlayerInputComponent.h"
#include "Core/Input.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Interface/TetrisModule.h"

#include <windows.h>
#include <cassert>

namespace Hiwoong
{
	PlayerInputComponent::PlayerInputComponent()
	{
	}

	
	void PlayerInputComponent::Start()
	{
		super::Start();
		std::shared_ptr<GameObject> owner = GetOwner();
		assert(owner != nullptr);

		player = owner.get();
		std::shared_ptr<TetrisModule> ownerModule =
			std::dynamic_pointer_cast<TetrisModule>(owner);

		assert(ownerModule != nullptr);

		module = ownerModule.get();
	}

    void PlayerInputComponent::Update(double deltaTime)
    {
        super::Update(deltaTime);

        assert(module != nullptr);

        if (Input::Get().GetKeyDown(VK_ESCAPE))
        {
            module->QuitGame();
        }

        if (Input::Get().GetKeyDown(VK_LEFT))
        {
            module->TryMove(Vector2(-1, 0));
        }

        if (Input::Get().GetKeyDown(VK_RIGHT))
        {
            module->TryMove(Vector2(1, 0));
        }

        if (Input::Get().GetKeyDown(VK_DOWN))
        {
            module->TryMove(Vector2(0, 1));
        }
    }
}