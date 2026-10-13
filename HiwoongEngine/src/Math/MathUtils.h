#pragma once

#include "Core/Core.h"

namespace Hiwoong
{
    class Hiwoong_API MathUtils
    {
    public:
        //각도 보간용 공용함수. 쓸일이 많아서 엔진에서 따로 뺌
        static float LerpAngle(
            float currentAngle,
            float targetAngle,
            float ratio
        );
    };
}