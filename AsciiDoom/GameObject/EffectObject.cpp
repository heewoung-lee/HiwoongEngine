#include "EffectObject.h"
#include "Interfaces/IDamageable.h"
#include <cassert>
#include <memory>

namespace Hiwoong
{
	EffectObject::EffectObject(int damage, const SpriteAnimationClip& destoryAnimaition) : damage(damage),destoryAnimaition(destoryAnimaition)
	{
		assert(damage >= 0);
	}

	void EffectObject::OnCollision(const std::shared_ptr<GameObject>& other)
	{
		GameObject::OnCollision(other);

		std::shared_ptr<IDamageable> target = std::dynamic_pointer_cast<IDamageable>(other);

		if (target != nullptr)
		{
			target->TakeDamage(damage);
		}
		//여기에 애니메이션이 실행되어야함. Idamagable의 여부와 상관없이.
	}

}
