#pragma once

#include "Component/Component.h"
#include "ModelImport/ModelData.h"
#include <vector>
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
    };
}