#include "MathUtils.h"
#include "MathConstants.h"
#include <cmath>

namespace Hiwoong
{
    float MathUtils::LerpAngle(
        float currentAngle,
        float targetAngle,
        float ratio)
    {
        if (ratio < 0.0f)
            ratio = 0.0f;
        else if (ratio > 1.0f)
            ratio = 1.0f;

        // 각도는 라디안 기준. 가까운 회전 방향을 선택한다.
        const float difference = std::remainder(
            targetAngle - currentAngle,
            2.0f * MathConstants::Pi
        );

        return currentAngle + difference * ratio;
    }
}