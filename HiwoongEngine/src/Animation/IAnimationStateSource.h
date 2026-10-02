#pragma once

#include <functional>
#include <string>

namespace Hiwoong
{
	/// <summary>
	/// 애니메이션의 상태를 바꾸는 컴포넌트에 상속할 인터페이스
	/// </summary>
	class IAnimationStateSource
	{
	public:
		using AnimationStateCallback = std::function<void(const std::string&)>;

		virtual ~IAnimationStateSource() = default;

		virtual void AddOnAnimationStateChanged(
			const AnimationStateCallback& callback
		) = 0;
	};
}