#pragma once

#include "GameObject/GameObject.h"
#include "Animation/SpriteAnimationClip.h"
#include "Math/Vector3.h"
#include <memory>

namespace Hiwoong
{
	class SpriteAnimationComponent;
	class SpriteRenderer3DComponent;

	class EffectViewObject : public GameObject
	{
		TYPE_DECALRATIONS(EffectViewObject, GameObject)

		
	public:
		void Start() override;
		void Update(double deltaTime) override;
		EffectViewObject(
			const Vector3& position,
			const SpriteAnimationClip& animationClip
		);

	private:
		SpriteAnimationClip animationClip;
		std::shared_ptr<SpriteAnimationComponent> animationComponent;
		std::shared_ptr<SpriteRenderer3DComponent> spriteRenderer;
		bool hasPlayedAnimation = false;
	};
}


