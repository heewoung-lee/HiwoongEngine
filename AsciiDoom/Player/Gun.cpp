#include "Gun.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Render/Renderer.h"
#include "Math/Vector2.h"
#include "Scene/DoomScene.h"
#include "Component/SpriteAnimationComponent.h"
#include "GunAnimationData.h"
#include "Core/Input.h"
#include "Player/CrossHair.h"
#include "Camera/Camera3D.h"
#include "GameObject/Bullet.h"
#include <cmath>
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

		gameSize = scene->GetGameSize();


        constexpr int gunWidth = 25;
        constexpr int gunHeight = 18;

		GetComponent<TransformComponent>()->SetLocalPosition(
			Vector3(
				(gameSize.x - gunWidth) / 2,
				gameSize.y - gunHeight,
				0
			)
		);

		assert(player != nullptr);

		// 초기화할 때 소속 씬에서 카메라를 한 번 가져와 보관한다.
		camera = scene->GetCamera();
		assert(camera != nullptr);
	}

	void Gun::Fire()
	{
        //마우스 왼쪽버튼을 눌렀을때만, 동작
		if (Input::Get().GetKeyDown(VK_LBUTTON) == false) return;

		if (player->GetCurrentAmmo() <= 0)
		{
			ForceReload();
			return;
		}

		assert(animation != nullptr);

		// 컴포넌트 초기화가 끝난 뒤 재생한다.
		if (!animation->HasStared()) return;

		if (animation->Play(GunAnimationData::Fire))
		{
			// 보관한 약한 참조에서 크로스헤어를 가져온다.
			const auto aimCrosshair = crosshair.lock();
			assert(aimCrosshair != nullptr);

			// 현재 크로스헤어 중심을 향하는 3D 방향을 구한다.
			const Vector3 shotDirection = camera->ScreenPointToDirection(
				aimCrosshair->GetScreenCenter(),
				gameSize,
				Renderer::Get().GetCharacterSize()
			);

			const auto bullet = Instantiate<Bullet>(
				player->GetWorldPosition(),
				shotDirection
			);
			assert(bullet != nullptr);

			player->SetAmmo(player->GetCurrentAmmo() - 1);
		}

	}

	void Gun::Reload()
	{

		assert(animation != nullptr);
		if (!animation->HasStared()) return;


		//리로드는 됐는데 애니메이션은 끝난경우, 즉 재장전 애니메이션이 다 된경우.
		if (animation->CheckPlaying() == false && isReload == true)
		{
			isReload = false;
			player->SetAmmo(player->GetMaxAmmo());
		}

		if (Input::Get().GetKeyDown('R') == false) return;
		ForceReload();
	}

	void Gun::ForceReload()
	{
		if (player->GetCurrentAmmo() >= player->GetMaxAmmo()) return; //최대 탄약이랑 현재 탄약이랑 같으면 무시.

		if (animation->Play(GunAnimationData::Reload)) // 재장전 모션이 다 끝나면 숫자 올라가게
		{
			isReload = true;
		}
	}

}
