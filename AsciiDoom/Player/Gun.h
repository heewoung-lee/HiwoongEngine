#pragma once

#include "GameObject/GameObject.h"
#include "Animation/SpriteAnimationClip.h"
#include "Player/Player.h"
#include "Math/Vector2.h"
namespace Hiwoong
{
	class SpriteAnimationComponent;
	class Crosshair;
	class Camera3D;

	class Gun : public GameObject
	{
		TYPE_DECALRATIONS(Gun,GameObject)


	public:
		Gun();
		void Update(double deltaTime) override;
		void Start() override;
		void SetCrosshair(const std::shared_ptr<Crosshair>& target)
		{
			crosshair = target;
		}
		void SetPlayer(const std::shared_ptr<Player>& target)
		{
			player = target;
		}
	private:
		//발사함수
		void Fire();
		void Reload();
		void ForceReload();

		SpriteAnimationClip fireAnimationClip;

		std::shared_ptr<Player> player;
		std::weak_ptr<Crosshair> crosshair;
		std::shared_ptr< SpriteAnimationComponent> animation;

		bool isReload = false; //현재 재장전중인가.

		// 카메라의 소유권은 씬에 있고, 총은 참조만 보관한다.
		Camera3D* camera = nullptr;

		Vector2 gameSize = Vector2::Zero;

	};


}
