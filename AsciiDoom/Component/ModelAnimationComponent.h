#pragma once

#include "Component/Component.h"
#include "ModelImport/ModelData.h"
#include <vector>
#include <functional>
#include <cstddef>
#include <memory>
#include <string>

namespace Hiwoong
{
    class MeshRenderComponent;

    class ModelAnimationComponent : public Component
    {
        TYPE_DECALRATIONS(ModelAnimationComponent, Component)

    public:
        //10.14일 추가 유니티에서 애니메이션 이벤트처럼
        //특정 애니메이션에 특정 프레임에 이벤트를 넣어 처리
        void AddAnimationEvent(
            const std::string& name,
            double normalizedTime,
            const std::function<void()>& callback
        );

        ModelAnimationComponent(
            const std::shared_ptr<const ModelData>& model,
            const std::string& initialAnimationName
        );
        void BindRenderer(
            std::size_t meshIndex,
            const std::shared_ptr<MeshRenderComponent>& renderer
        );
        void Start() override;
        void Update(double deltaTime) override;

        bool Play(const std::string& name);

        bool Play(
            const std::string& name,
            bool lockUntilFinished
        );
        //10.16일 추가.
        //공격 애니메이션은 강제로 끊을 수 없기에.
        //공격중 죽는다면 Stop()을 호출하고 -> 죽은 애니메이션을 실행해야함
        void Stop();
    private:



        void ApplyPose();

        std::shared_ptr<const ModelData> model;
        std::string initialAnimationName;

        int currentClipIndex = -1;
        double elapsedTime = 0.0;

        struct RendererBinding
        {
            std::size_t meshIndex;
            std::weak_ptr<MeshRenderComponent> renderer;
        };

        std::vector<RendererBinding> rendererBindings;

        //애니메이션이 끝날때까지 상태를 변경할 것인지에 대한 플래그
        bool lockUntilFinished = false;

        struct AnimationEvent
        {
            std::string animationName;
            double normalizedTime;
            std::function<void()> callback;
        };

        std::vector<AnimationEvent> animationEvents;

        //이전 시간과 현재 시간 사이에 이벤트 시점을 지났는지 검사해서
        //지났다면 갖고 있는 콜백을 전부 호출
        void ProcessAnimationEvents(
            double previousTime,
            double currentTime
        );

    };
}