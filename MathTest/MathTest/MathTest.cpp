
#include <iostream>
#include "Math/Vector3.h"

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

int main()
{
    
    std::cout << IsAllTest() << std::endl;
}