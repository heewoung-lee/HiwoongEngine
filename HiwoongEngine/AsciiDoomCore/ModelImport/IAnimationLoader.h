#pragma once

#include "../Animation/AnimationClip.h"
#include "SkeletonNode.h"
#include <vector>

namespace Hiwoong
{
    struct ModelImportContext;

    class IAnimationLoader
    {
    public:
        virtual ~IAnimationLoader() = default;

        virtual bool Load(
            const ModelImportContext& context,
            const std::vector<SkeletonNode>& modelNodes,
            std::vector<AnimationClip>& clips
        ) const = 0;
    };
}