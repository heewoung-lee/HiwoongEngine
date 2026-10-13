#pragma once

#include "IAnimationLoader.h"

namespace Hiwoong
{
    class AnimationLoader final : public IAnimationLoader
    {
    public:
        bool Load(
            const ModelImportContext& context,
            const std::vector<SkeletonNode>& modelNodes,
            std::vector<AnimationClip>& clips
        ) const override;


    private:
        int FindNodeIndex(
            const std::vector<SkeletonNode>& nodes,
            const std::string& name
        ) const;
    };
}