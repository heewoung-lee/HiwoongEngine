#pragma once

#include "Component/Component.h"
#include "Animation/SpriteAnimationClip.h"
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <functional>
#include <unordered_map>

namespace Hiwoong
{
	class SpriteRendererComponent;

	struct AnimationBinding
	{
		std::string name;
		SpriteAnimationClip clip;
	};


	class Hiwoong_API SpriteAnimationComponent : public Component
	{
		TYPE_DECALRATIONS(SpriteAnimationComponent,Component)


	public :
		SpriteAnimationComponent(std::vector<std::vector<std::string>> frames, double duration);
		SpriteAnimationComponent(
			const std::vector<AnimationBinding>& bindings,
			const std::string& initialAnimationName
		);
		~SpriteAnimationComponent() = default;

		void Start() override;
		void Update(double deltaTime) override;
		bool Play(const SpriteAnimationClip& clip);
		bool Play(const std::string& name);
		void Bind(const std::string& name, const SpriteAnimationClip& clip);
		using AnimationEndCallback = std::function<void()>;
		//현재 애니메이션 이름 확인
		const std::string& GetCurrentAnimationName() const
		{
			return currentAnimationName;
		}
		void AddOnAnimationEnd(
			const AnimationEndCallback& callback
		);

		


		inline bool CheckPlaying() const { return isPlaying; }
	private:
		//그림 한장을 표시하는 함수.
		void ApplyFrame(std::size_t frameIndex);
		void BroadcastOnAnimationEnd();
		void InitAnimationBindings();
		void InitPlayback();

		//초기화 구문이나 update에 실행
		//이유는 start에 넣어 버리면 컴포넌트의 순서에 영향이 생김.
		void InitStateSubscriptions();

	private:


		std::vector<AnimationBinding> animationBindings;
		std::string initialAnimationName;

		std::vector<std::vector<std::string>> frames; //실행할 그림들
		double duration = 0.0f; // 전체 재생 시간. 이후 
		double elapsedTime = 0.0f; //경과 시간
		bool isPlaying = false;//현재 재생중인지.
		bool isLooping = false;
		std::size_t currentFrameIndex = 0; // 현재 그림 번호
		std::weak_ptr<SpriteRendererComponent> spriteRenderer;
		std::vector<AnimationEndCallback> animationEndCallbacks;

		//애니메이션 클립을 이름으로 바인드해서 호출하기 편하게 만드는 딕셔너리.
		std::unordered_map<std::string, SpriteAnimationClip> animationClips;

		std::string currentAnimationName; //현재 재생중인 애니메이션 이름
	
		//현재 구독들이 전부 되었는지 확인. 이부분은 Start로 해야하나.
		//컴포넌트의 순서에 영향을 받을 수 있으므로
		//Update에 배치하기 위해 플래그 변수를 만들었음.
		bool hasSubscribedStates = false;

		bool lockUntilFinished = false;
	};
}
