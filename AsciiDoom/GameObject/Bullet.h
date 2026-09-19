#pragma once

#include "GameObject/GameObject.h"
#include "Component/BoxCollider3DComponent.h"
#include "Math/Vector3.h"

#include <memory>

namespace Hiwoong
{
	class Bullet : public GameObject
	{
		TYPE_DECALRATIONS(Bullet,GameObject)

	public:
		Bullet(
			const std::shared_ptr<GameObject>& spawner, //충돌 거르기용
			const Vector3& firePosition,  // 발사 시작 위치
			const Vector3& fireDirection, // 날아갈 방향
			float size = 0.03f,           // 탄환 크기
			float speed = 4.0f,           // 이동 속도
			float spawnDistance = 0.1f    // 시작 위치에서 앞으로 띄울 거리
		);

		void Start() override;
		void Update(double deltaTime) override;

		void OnCollision(
			const std::shared_ptr<GameObject>& other
		) override;

	private:
		std::shared_ptr<BoxCollider3DComponent> collider;
		float speed;



		//TODO:잠깐 임시로 크기를 지정함 나중에 예뻐질때까지 수정할 것
		float size_width = 1;
		float size_height = 1;
		float size_depth = 1;


		Vector3 firePosition; // 발사 시작 위치
		Vector3 fireDirection; // 날아갈 방향
		float spawnDistance; // 시작 위치에서 앞으로 띄울 거리
		Vector3 previousPosition = Vector3::Zero; 
		std::weak_ptr<GameObject> spawner;

		float lifeTime = 5;
		float elapsedTime = 0;
		
	};


	
}


