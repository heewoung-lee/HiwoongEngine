#pragma once

#include "ModelVertex.h"
#include "ModelTriangle.h"
#include "ModelTexture.h"
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
        // 재질 번호 순서로 보관하는 색상 이미지 경로.
        // 이미지가 없는 재질은 빈 문자열로 유지한다.
        // 예를 들어 삼각형의 머테리얼 인덱스가 1 이면 
        //diffuseTexturePaths[1]에서 사용할 이미지 경로를 찾는다.
        std::vector<std::string> diffuseTexturePaths;

        // 재질 번호 순서대로 보관하는 색상 이미지
        std::vector<std::shared_ptr<const ModelTexture>> diffuseTextures;
    };
}