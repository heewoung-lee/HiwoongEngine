#pragma once

namespace Hiwoong
{
    //FBX를 임포트할때 각 정점의 위치 정보를 담는 구조체
    struct ModelVertex
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        //어느 위치의 색을 사용할지
        float u = 0.0f;
        float v = 0.0f;
    };
}