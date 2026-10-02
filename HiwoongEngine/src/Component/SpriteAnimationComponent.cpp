#include "SpriteAnimationComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "GameObject/GameObject.h"
#include "Animation/IAnimationStateSource.h"
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
	SpriteAnimationComponent::SpriteAnimationComponent(
		const std::vector<AnimationBinding>& bindings,
		const std::string& initialAnimationName)
		: animationBindings(bindings),
		initialAnimationName(initialAnimationName)
	{
		assert(animationBindings.empty() == false);
		assert(initialAnimationName.empty() == false);
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

		InitAnimationBindings();
		InitPlayback();
	}
	void SpriteAnimationComponent::Update(double deltaTime)
	{
		super::Update(deltaTime);

		//IAnimationStateSource의 컴포넌트를 한번만 찾아서 콜백함수에 저장하는 초기화 함수
		//Start에 넣으면 컴포넌트 순서에 영향을 받으므로 update에서 한번만 호출하도록
		if (hasSubscribedStates == false)
		{
			InitStateSubscriptions();
			hasSubscribedStates = true;
		}


		if (isPlaying == false) return;

		elapsedTime += deltaTime;

		if (elapsedTime >= (duration / frames.size()))
		{
			elapsedTime = 0.0;
			currentFrameIndex++; // 0번은 이미 Play()에서 호출했으므로 다음 그림부터 출력

			if (currentFrameIndex >= frames.size())
			{
				currentFrameIndex = 0;
				//루프 애니메이션이 아닌 경우에만 종료처리.
				if (isLooping == false)
				{
					isPlaying = false;
					BroadcastOnAnimationEnd();
					return;
				}
			}
			ApplyFrame(currentFrameIndex);
		}

	}
	void SpriteAnimationComponent::AddOnAnimationEnd(const AnimationEndCallback& callback)
	{
		if (callback == nullptr) return;
		animationEndCallbacks.push_back(callback);
	}

	void SpriteAnimationComponent::BroadcastOnAnimationEnd()
	{
		for (const AnimationEndCallback& callback : animationEndCallbacks)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="clip"></param>
	/// <param name="force">새 요청이 현재 재생을 끊을지를 결정</param>
	/// <returns></returns>
	bool SpriteAnimationComponent::Play(const SpriteAnimationClip& clip)
	{
		assert(HasStared());
		if (isPlaying && lockUntilFinished)
			return false;

		assert(!clip.frames.empty());
		assert(clip.duration > 0.0);

		currentAnimationName.clear();

		frames = clip.frames;
		duration = clip.duration;
		isLooping = clip.isLooping;
		lockUntilFinished = clip.lockUntilFinished;

		currentFrameIndex = 0;
		elapsedTime = 0.0;
		isPlaying = true;

		ApplyFrame(currentFrameIndex);

		return true;
	}
	bool SpriteAnimationComponent::Play(const std::string& name)
	{
		if (isPlaying && currentAnimationName == name)
			return false;

		if (animationClips.find(name) != animationClips.end())
		{
			if (Play(animationClips[name]) == false)
				return false;

			currentAnimationName = name;
			return true;
		}

		return false;
	}
	void SpriteAnimationComponent::Bind(
		const std::string& name,
		const SpriteAnimationClip& clip)
	{
		//같은 이름이 있으면 오류
		assert(animationClips.find(name) == animationClips.end());

		animationClips.emplace(name, clip);
	}

	void SpriteAnimationComponent::InitAnimationBindings()
	{
		for (const AnimationBinding& binding : animationBindings)
		{
			Bind(binding.name, binding.clip);
		}
	}
	void SpriteAnimationComponent::InitPlayback()
	{
		if (initialAnimationName.empty()) return;

		assert(animationClips.find(initialAnimationName) != animationClips.end());

		Play(initialAnimationName);
	}
	void SpriteAnimationComponent::InitStateSubscriptions()
	{
		const auto ownerObject = GetOwner();
		assert(ownerObject != nullptr);

		const std::weak_ptr<SpriteAnimationComponent> animation =
			GetComponent<SpriteAnimationComponent>();

		for (const auto& component : ownerObject->GetComponents())
		{
			const auto source =
				std::dynamic_pointer_cast<IAnimationStateSource>(component);

			if (source == nullptr) continue;

			source->AddOnAnimationStateChanged(
				[animation](const std::string& name)
				{
					const auto instance = animation.lock();
					if (instance == nullptr) return;

					instance->Play(name);
				}
			);
		}

	}
}

