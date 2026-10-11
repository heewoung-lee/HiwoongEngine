#pragma once

#include "ModelMatrix.h"

namespace Hiwoong
{
    /// <summary>
    /// FBX데이터에 있는 바인딩행렬 정보
    /// 예를들어 팔꿈치 위치가 1 피부위치가 2라면
    /// 관절을 기준으로 정점위치는 1이다.
    /// 그 정보를 변환하는게 바인딩행렬
    /// ufbx가 마지막 정보를 geometry_to_bone으로 제공함.
    /// </summary>
    struct SkinBinding
    {
        // SkeletonNode 목록의 관절 번호.
        int nodeIdx = -1;

        // 기본 자세의 원본 메시 정점을 관절 기준으로 바꾸는 행렬.
        ModelMatrix geometryToBone;
    };
}