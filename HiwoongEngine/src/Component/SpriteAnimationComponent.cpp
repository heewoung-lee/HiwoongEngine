#include "SpriteAnimationComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "GameObject/GameObject.h"
#include <cassert>
#include <cstddef>

namespace Hiwoong
{
	SpriteAnimationComponent::SpriteAnimationComponent
	(std::vector<std::vector<std::string>> frames, double duration) : frames(frames), duration(duration)
	{
		assert(this->frames.empty() == false);
		assert(duration > 0.0);
	}
	//frames가 가진 그림 한장을 렌더러에 넘기기.
	void SpriteAnimationComponent::ApplyFrame(std::size_t frameIndex)
	{
		//만약 인덱스가 원래 그림보다 더 커지면 리턴, 없는 그림을 출력할 수 없으니깐.
		assert(frameIndex < frames.size());

		const std::shared_ptr<SpriteRendererComponent> renderer = spriteRenderer.lock();
		assert(renderer != nullptr);

		const std::vector<std::string>& frame = frames[frameIndex];
		renderer->SetImage(frame);

	}
	void SpriteAnimationComponent::Start()
	{
		super::Start();

		const std::shared_ptr<SpriteRendererComponent> renderer
			= GetComponent<SpriteRendererComponent>();

		assert(renderer != nullptr);

		spriteRenderer = renderer;
	}
	void SpriteAnimationComponent::Update(double deltaTime)
	{
		super::Update(deltaTime);

		if (isPlaying == false) return;

		elapsedTime += deltaTime;

		if (elapsedTime >= (duration / frames.size()))
		{
			elapsedTime = 0.0;
			currentFrameIndex++; // 0번은 이미 Play()에서 호출했으므로 다음 그림부터 출력

			if (currentFrameIndex >= frames.size())
			{
				currentFrameIndex = 0;
				isPlaying = false;
				return;
			}
			ApplyFrame(currentFrameIndex);
		}

	}

	bool SpriteAnimationComponent::Play(const SpriteAnimationClip& clip)
	{
		assert(HasStared());

		if (isPlaying) return false;

		assert(!clip.frames.empty());
		assert(clip.duration > 0.0);

		frames = clip.frames;
		duration = clip.duration;

		currentFrameIndex = 0;
		elapsedTime = 0.0;
		isPlaying = true;

		ApplyFrame(currentFrameIndex);

		return true;
	}
}

