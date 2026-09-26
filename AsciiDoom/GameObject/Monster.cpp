#include "Monster.h"
#include "Component/SpriteRenderer3DComponent.h"
#include "Component/SpriteAnimationComponent.h"
#include "Component/BoxCollider3DComponent.h"
#include "Component/NavAIComponent.h"

#include <algorithm>
#include <cassert>

namespace Hiwoong
{
	Monster::Monster(
		const Vector3& spawnPosition, 
		const MonsterAnimationSet& animations,
		int hp, 
		float speed, 
		float attackRange,
		float scale) : GameObject(spawnPosition), animations(animations),currentHp(hp),speed(speed),attackRange(attackRange),scale(scale)
	{
		
		
	}

	void Monster::Start()
	{
		super::Start();

		transform->SetScale(
			Vector3(scale, scale, scale)
		);

		spriteRenderer = AddComponent<SpriteRenderer3DComponent>(
			animations.idle.frames.front(),
			Color::Magenta
		);

		animationComponent = AddComponent<SpriteAnimationComponent>(
			animations.idle.frames,
			animations.idle.duration
		);

		Vector3 size =  InitSetColliderSize();
		AddComponent<BoxCollider3DComponent>(
			Vector3(
				size.x * 0.5f,
				size.y * 0.5f,
				size.z * 0.5f
			)
		);
		AddComponent<NavAIComponent>(speed, attackRange);
	}

	void Monster::TakeDamage(int damage)
	{
		currentHp -= damage;

		if (currentHp <= 0)
		{
			//TODO: 죽음 애니메이션 실행.
			Destroy();
		}

	}

	Vector3 Monster::InitSetColliderSize()
	{
		assert(animations.idle.frames.empty() == false);

		const std::vector<std::string>& idleFrame = animations.idle.frames.front();

		assert(idleFrame.empty() == false);

		std::size_t imageWidth = 0;

		for (const std::string& row : idleFrame)
		{
			imageWidth = (std::max)(imageWidth, row.size());
		}

		const float imageHeight = static_cast<float>(idleFrame.size());

		const float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

		const float worldHeight = scale;
		const float worldWidth = scale * aspectRatio;


		return Vector3(worldWidth, worldHeight, worldWidth);
	}

}

