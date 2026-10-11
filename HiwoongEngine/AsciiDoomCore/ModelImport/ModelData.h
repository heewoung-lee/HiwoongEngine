#pragma once

#include "ModelMesh.h"
#include "SkeletonNode.h"
#include "ModelMatrix.h"
#include "../Animation/AnimationClip.h"
#include <vector>

namespace Hiwoong
{
    struct ModelData
    {
        std::vector<ModelMesh> meshes;
        // 10.11일 스켈레톤 추가.
        // FBX내에 애니메이션을 재생하려면 스켈레톤 정보를 가져와야함
        std::vector<SkeletonNode> skeltonNodes;

        // 애니메이션 계산 결과를 기존 모델과 같은 중심으로 정렬.
        ModelMatrix modelAlignment;

        // 이 모델에서 사용할 애니메이션 목록.
        std::vector<AnimationClip> animationClips;
    };
}