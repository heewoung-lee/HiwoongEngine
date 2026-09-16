#include "Gun.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"
#include "Math/Vector2.h"
#include "Scene/DoomScene.h"
#include "Component/SpriteAnimationComponent.h"
#include "GunAnimationData.h"
#include "Core/Input.h"

#include <cassert>

namespace Hiwoong
{

	Gun::Gun()
	{
		//발사 클립 애니메이션 정보
		const SpriteAnimationClip& clip = GunAnimationData::Fire;

		AddComponent<SpriteRendererComponent>(
			clip.frames.back(),
            Color::White,
            100
        );

		animation = AddComponent<SpriteAnimationComponent>(
			clip.frames,
			clip.duration
		);
	}

	void Gun::Update(double deltaTime)
	{
		super::Update(deltaTime);
		Reload();//리로드 함수
		Fire();//발사 함수
	}

	void Gun::Start()
	{
		super::Start();



		//씬불러와서 게임플레이 화면의 크기 가져오기.
		//타입 변환때문에 다이나믹 포인터 씀.
		const auto scene =
			std::dynamic_pointer_cast<DoomScene>(GetOwner());
		assert(scene != nullptr);

		const Vector2 size = scene->GetGameSize();

        constexpr int gunWidth = 25;
        constexpr int gunHeight = 18;

        GetComponent<TransformComponent>()->SetLocalPosition(
            Vector3((size.x - gunWidth) / 2, size.y - gunHeight, 0)
        );

       player = std::dynamic_pointer_cast<Player>(GetParent());
       assert(player != nullptr);
	}

	void Gun::Fire()
	{
        //마우스 왼쪽버튼을 눌렀을때만, 동작
		if (Input::Get().GetKeyDown(VK_LBUTTON) == false) return;

        if (player->GetAmmo() <= 0) return;

		assert(animation != nullptr);

		// 컴포넌트 초기화가 끝난 뒤 재생한다.
		if (!animation->HasStared()) return;

		if (animation->Play(GunAnimationData::Fire))
		{
			player->SetAmmo(player->GetAmmo() - 1);
		}

	}

	void Gun::Reload()
	{
		if (!Input::Get().GetKeyDown('R')) return;

		assert(animation != nullptr);
		if (!animation->HasStared()) return;

		animation->Play(GunAnimationData::Reload);
	}

}
