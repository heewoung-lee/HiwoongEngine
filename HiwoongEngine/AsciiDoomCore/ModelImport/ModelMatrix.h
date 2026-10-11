#pragma once
#include "ModelVertex.h"
#include "SkeletonNode.h"

namespace Hiwoong
{
    /// <summary>
    /// 정점 위치를 관절 기준으로 변환할 때 사용할 행렬
    /// </summary>
    struct ModelMatrix
    {
        float values[4][4] = {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };

        //ModelMatrix에 정점 위치를 변환하는 함수
        //저장한 행렬을 실제 정점에 적용하는 역할.
        ModelVertex TransformPoint(const ModelVertex& vertex) const;

        //부모와 자식 행렬을 합치는 행렬
        // 예를 들어 팔꿈치가 자식이라면, 
        // 팔꿈치 자체의 움직임에 어깨의 움직임까지 반영한 행렬을 계산
        ModelMatrix Multiply(const ModelMatrix& other) const;

        //관절에 저장한 위치,회전,크기를 행렬 하나로 만듦
        static ModelMatrix FromNode(const SkeletonNode& node);
    };
}