#pragma once

#include "GameObject/GameObject.h"
#include "Animation/SpriteAnimationClip.h"
#include "Math/Vector3.h"
#include <memory>

namespace Hiwoong
{
	class BoxCollider3DComponent;

	class EffectObject : public GameObject
	{
		TYPE_DECALRATIONS(EffectObject,GameObject)

		
	public:
		EffectObject(
			int damage,
			const SpriteAnimationClip& destoryAnimaition
		);

		void OnCollision(const std::shared_ptr<GameObject>& other) override;

	private:
		int damage;
		SpriteAnimationClip destoryAnimaition;
	};
}


