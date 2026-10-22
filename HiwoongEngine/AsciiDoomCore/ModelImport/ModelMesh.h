#pragma once

#include "ModelVertex.h"
#include "ModelTriangle.h"
#include "ModelTexture.h"
#include "SkinWeight.h"
#include "SkinBinding.h"
#include <memory>
#include <string>
#include <vector>

namespace Hiwoong
{
    struct ModelMesh
    {
        std::string name;
        std::vector<ModelVertex> vertices;
        std::vector<ModelTriangle> triangles;
        // 재질 번호 순서의 이미지 경로와 반복 설정.
        std::vector<ModelTextureInfo> diffuseTextureInfos;

        // 재질 번호 순서대로 보관하는 색상 이미지
        std::vector<std::shared_ptr<const ModelTexture>> diffuseTextures;
        
        // 각 정점에 영향을 주는 관절과 비율 목록
        std::vector<std::vector<SkinWeight>> skinWeights;

        // 이 메시의 정점을 각 관절 기준으로 바꾸는 바인딩 목록.
        std::vector<SkinBinding> skinBindings;

        // 위치 변환 전의 원본 메시 정점 위치.
        // vertices와 같은 순서로 보관한다.
        std::vector<ModelVertex> localVertices;
    };
}