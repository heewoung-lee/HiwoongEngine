
#include <iostream>
#include "Math/Vector3.h"
#include <cmath>

bool IsNormalized()
{
    Vector3 normalized = Vector3(6, 8, 0).Normalized();

    return normalized.GetLength() == 1;
}
bool IsNormalizedZero()
{
    Vector3 normalized = Vector3(0, 0, 0).Normalized();

    return normalized.GetLength() == 0;
}

bool IsGetDot()
{
    Vector3 a(1,2,3);
    Vector3 b(4, -5, 6);

    float dotResult = 12.0f;

    return a.Dot(b) == dotResult;
}


bool IsAllTest()
{
    return
        IsNormalized() &&
        IsNormalizedZero() &&
        IsGetDot();
}

bool CheckCross()
{
    Vector3 a(1,0,0);
    Vector3 b(0, 0, 1);

    Vector3 expect(0, -1, 0);

    return a.Cross(b) == expect;
}

bool CheckFromYaw()
{
    Vector3 expect(1,0,0);
    constexpr float Pi = 3.14159265f;
    Vector3 test = Vector3::FromYaw(Pi/2);

    return expect.NearlyEquals(test,0.0001f);
}

bool CheckNearlyEquels()
{
    Vector3 a(1, 1, 1);
    Vector3 b(1.0000000001f, 1.0000000001f, 1.0000000001f);

    return a.NearlyEquals(b,0.0001f);
}

bool CheckPeojection()
{
    Vector3 projection = Vector3::GetProjection(Vector3(4,2,2));

    Vector3 expected(2, 1, 2);

    return projection == expected;
}

int main()
{
    
    std::cout << CheckPeojection() << std::endl;
}