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
			const Vector3& firePosition,  
			const Vector3& fireDirection, 
			float size = 0.03f,          
			float speed = 4.0f,          
			float spawnDistance = 0.1f    
		);

		void Start() override;
		void Update(double deltaTime) override;

		void OnCollision(
			const std::shared_ptr<GameObject>& other
		) override;

	private:
		void UpdateLifetime(double deltaTime);
		void UpdateMovement(double deltaTime);
		Vector3 GetRotationFromDirection();
	private:
		std::shared_ptr<BoxCollider3DComponent> collider;
		float speed;


		//TODO:잠깐 임시로 크기를 지정함 나중에 예뻐질때까지 수정할 것
		float size_width = 1;
		float size_height = 1;
		float size_depth = 1;

		Vector3 firePosition;
		Vector3 fireDirection;
		float spawnDistance;
		Vector3 previousPosition = Vector3::Zero; 
		std::weak_ptr<GameObject> spawner;

		float lifeTime = 5;
		float elapsedTime = 0;
		
	};


	
}


