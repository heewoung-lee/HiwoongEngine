
#include <iostream>
#include "Math/Vector3.h"
#include <cmath>
#include<vector>

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
bool CanProject(
    const Vector3& cameraPosition,
    float nearPlane)
{
    return cameraPosition.GetZ() >= nearPlane;
}
bool CheckCannotProjectBehindCamera()
{
    Vector3 cameraPosition(4, 2, 0.0001f);
    float nearPlane = 0.1f;

    return CanProject(cameraPosition, nearPlane) == false;
}

bool CheckIntersectNearPlane()
{
    Vector3 a(0, 0, -1);
    Vector3 b(4, 0, 3);
    float nearPlane = 1;

    Vector3 expect(2, 0,1);
    return Vector3::IntersectNearPlane(a,b,nearPlane) == expect;
}

std::vector<Vector3> ClipTriangleNearPlane(
    const Vector3& p,
    const Vector3& q,
    const Vector3& r,
    float nearPlane)
{
    Vector3 vertices[3] = { p, q, r };
    std::vector<Vector3> positions;

    int insideCount = 0;

    for (int i = 0; i < vertices->GetLength(); ++i)
    {
        if (vertices[i].GetZ() >= nearPlane)
        {
            insideCount++;
        }
    }

    




    
    switch (insideCount)
    {
    case 0: return {};
    case 3: return {p,q,r};
    
    case 2: 
        


    default:
        break;
    }

    Vector3 j1 = Vector3::IntersectNearPlane(p, q, nearPlane);
    Vector3 j2 = Vector3::IntersectNearPlane(p, r, nearPlane);

    return {j1,q,r,j2};
}
bool CheckClipTriangleTwoInside()
{
    Vector3 p(0, 0, 0);
    Vector3 q(2, 0, 2);
    Vector3 r(0, 2, 2);

    std::vector<Vector3> result =  ClipTriangleNearPlane(p, q, r, 1.0f);




    return result.size() == 4 &&  
}


int main()
{
    
    std::cout << CheckClipTriangleTwoInside() << std::endl;
}