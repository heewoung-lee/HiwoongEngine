#include "ModelAnimationComponent.h"
#include "MeshRenderComponent.h"
#include "Animation/SkeletonPoseCalculator.h"
#include "Animation/SkinningCalculator.h"
#include <cmath>
#include <cassert>

namespace Hiwoong
{

    ModelAnimationComponent::ModelAnimationComponent(
        const std::shared_ptr<const ModelData>& model,
        const std::string& initialAnimationName
    )
        : model(model),
        initialAnimationName(initialAnimationName)
    {
        assert(this->model != nullptr);
    }
    void ModelAnimationComponent::BindRenderer(
        std::size_t meshIndex,
        const std::shared_ptr<MeshRenderComponent>& renderer
    )
    {
        assert(meshIndex < model->meshes.size());
        assert(renderer != nullptr);

        rendererBindings.push_back({ meshIndex, renderer });
    }
    void ModelAnimationComponent::Start()
    {
        super::Start();

        if (currentClipIndex == -1)
            Play(initialAnimationName);
    }
    void ModelAnimationComponent::AddAnimationEvent(const std::string& name, double normalizedTime, const std::function<void()>& callback)
    {
        assert(normalizedTime >= 0.0 && normalizedTime <= 1.0);
        assert(callback);

        animationEvents.push_back({
            name,
            normalizedTime,
            callback
            });
    }
    void ModelAnimationComponent::Update(double deltaTime)
    {
        super::Update(deltaTime);

        if (currentClipIndex == -1)
            return;

        const AnimationClip& clip =
            model->animationClips[currentClipIndex];

        if (lockUntilFinished && elapsedTime >= clip.duration)
            return;

        double previousTime = elapsedTime;
        elapsedTime += deltaTime;

        if (lockUntilFinished)
        {
            if (elapsedTime > clip.duration)
                elapsedTime = clip.duration;
        }
        else
        {
            while (elapsedTime >= clip.duration)
            {
                //락이 안걸린 애니메이션 이벤트
                ProcessAnimationEvents(previousTime, clip.duration);

                elapsedTime -= clip.duration;

                // 새 회차의 0초 이벤트도 검사하기 위한 시작값.
                previousTime = -1.0;
            }
        }

        ApplyPose();
        ProcessAnimationEvents(previousTime, elapsedTime);
    }
    bool ModelAnimationComponent::Play(const std::string& name)
    {
        return Play(name, false);

    }

    bool ModelAnimationComponent::Play(const std::string& name, bool lockUntilFinished)
    {
        if (this->lockUntilFinished)
        {
            const AnimationClip& currentClip =
                model->animationClips[currentClipIndex];

            if (elapsedTime < currentClip.duration)
                return false;
        }

        for (int i = 0;
            i < static_cast<int>(model->animationClips.size());
            ++i)
        {
            const AnimationClip& clip = model->animationClips[i];

            if (clip.name != name)
                continue;

            // 같은 애니메이션이 아직 재생 중이면 시간을 유지한다.
            if (currentClipIndex == i && elapsedTime < clip.duration)
                return true;

            assert(!clip.frames.empty());
            assert(clip.duration > 0.0);

            currentClipIndex = i;
            elapsedTime = 0.0;
            this->lockUntilFinished = lockUntilFinished;

            ApplyPose();
            //0초에서도 이벤트를 호출할 수 있게 처음을 -1초 해둠
            ProcessAnimationEvents(-1.0, 0.0);

            return true;
        }

        assert(false && "Animation name not found");
        return false;
    }

    void ModelAnimationComponent::ApplyPose()
    {
        const AnimationClip& clip =
            model->animationClips[currentClipIndex];

        // 현재 재생 시간에 맞는 관절 행렬을 구한다.
        const std::vector<Matrix4x4> matrices =
            SkeletonPoseCalculator::Calculate(
                model->skeltonNodes,
                clip,
                elapsedTime
            );

        for (std::size_t meshIndex = 0;
            meshIndex < model->meshes.size();
            ++meshIndex)
        {
            // 관절의 움직임을 반영한 정점 위치를 구한다.
            const std::vector<ModelVertex> vertices =
                SkinningCalculator::Calculate(
                    model->meshes[meshIndex],
                    matrices,
                    model->modelAlignment
                );

            for (const RendererBinding& binding : rendererBindings)
            {
                if (binding.meshIndex != meshIndex)
                    continue;

                const auto renderer = binding.renderer.lock();

                if (renderer == nullptr)
                    continue;

                renderer->SetVertexPositions(vertices);
            }
        }
    }
    void ModelAnimationComponent::Stop()
    {
        currentClipIndex = -1;
        elapsedTime = 0.0;
        lockUntilFinished = false;
    }

    void ModelAnimationComponent::ProcessAnimationEvents(
        double previousTime,
        double currentTime
    )
    {
        const AnimationClip& clip =
            model->animationClips[currentClipIndex];

        for (const AnimationEvent& event : animationEvents)
        {
            if (event.animationName != clip.name)
                continue;

            const double eventTime =
                event.normalizedTime * clip.duration;

            if (previousTime < eventTime &&
                eventTime <= currentTime)
            {
                event.callback();
            }
        }
    }
}