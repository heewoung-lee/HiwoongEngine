#pragma once
#include "Math/Vector3.h"

namespace Hiwoong
{
    struct SpotLight
    {
        Vector3 position = Vector3::Zero;
        Vector3 direction = Vector3(0.0f, 0.0f, 1.0f);
        float range = 5.0f;
        float outerHalfAngleDegrees = 30.0f;//빛의 퍼지는 각도 30은 빛 없음
        float innerHalfAngleDegrees = 20.0f;//안쪽각도 밝기 감소없음
        float intensity = 1.0f;
    };
}