#pragma once

namespace Hiwoong
{
	class IDamageable
	{
	public:
		virtual ~IDamageable() = default;

		virtual void TakeDamage(int damage) = 0;
	};
}
