#pragma once

#include "Component/Component.h"
#include "Animation/SpriteAnimationClip.h"
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <functional>


namespace Hiwoong
{
	class SpriteRendererComponent;

	class Hiwoong_API SpriteAnimationComponent : public Component
	{
		TYPE_DECALRATIONS(SpriteAnimationComponent,Component)


	public :
		SpriteAnimationComponent(std::vector<std::vector<std::string>> frames, double duration);
		~SpriteAnimationComponent() = default;

		void Start() override;
		void Update(double deltaTime) override;
		bool Play(const SpriteAnimationClip& clip);

		using AnimationEndCallback = std::function<void()>;

		void AddOnAnimationEnd(
			const AnimationEndCallback& callback
		);

		inline bool CheckPlaying() const { return isPlaying; }
		
	private:
		//그림 한장을 표시하는 함수.
		void ApplyFrame(std::size_t frameIndex);
		void BroadcastOnAnimationEnd();
	private:

		std::vector<std::vector<std::string>> frames; //실행할 그림들
		double duration = 0.0f; // 전체 재생 시간. 이후 
		double elapsedTime = 0.0f; //경과 시간
		bool isPlaying = false;//현재 재생중인지.
		bool isLooping = false;
		std::size_t currentFrameIndex = 0; // 현재 그림 번호
		std::weak_ptr<SpriteRendererComponent> spriteRenderer;
		std::vector<AnimationEndCallback> animationEndCallbacks;
	};
}
