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

    void ModelAnimationComponent::Update(double deltaTime)
    {
        super::Update(deltaTime);

        if (currentClipIndex == -1)
            return;

        const AnimationClip& clip =
            model->animationClips[currentClipIndex];

        elapsedTime += deltaTime;

        if (lockUntilFinished)
        {
            if (elapsedTime > clip.duration)
                elapsedTime = clip.duration;
        }
        else
        {
            elapsedTime = std::fmod(elapsedTime, clip.duration);
        }

        ApplyPose();
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
        const std::vector<ModelMatrix> matrices =
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
}