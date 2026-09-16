#pragma once

#include "GameObject/GameObject.h"
#include "Player/Player.h"
namespace Hiwoong
{
	class SpriteAnimationComponent;

	class Gun : public GameObject
	{
		TYPE_DECALRATIONS(Gun,GameObject)


	public:
		Gun();
		void Update(double deltaTime) override;
		void Start() override;


	private:
		//발사함수
		void Fire();
		void Reload();
		std::shared_ptr<Player> player;

		std::shared_ptr< SpriteAnimationComponent> animation;
	};


}
