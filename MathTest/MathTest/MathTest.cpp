
#include <iostream>
#include "Math/Vector3.h"
#include <cmath>
#include<vector>
#include<cassert>
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
    std::vector<Vector3> vertices = { p, q, r };
    std::vector<bool> insides(3, false);

    std::vector<Vector3> positions;

    int insideCount = 0;

    //어떤 정점이 밖에 있나 확인.
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i].GetZ() >= nearPlane)
        {
            insideCount++;
            insides[i] = true;
        }
    }

    
    switch (insideCount)
    {
    case 0: return {};
    case 1:
    {
        int findidx = -1;
        for (int i = 0; i < insides.size(); ++i)
        {
            if (insides[i] == true)
            {
                findidx = i;
                break;
            }
        }
        //찾은 정점에서 이전과 이후의 정점을 찾은 다음.
        int nextIdx = (findidx + 1) % 3;
        int preIdx = (findidx + 2) % 3;
        //찾은 정점과 바깥쪽 정점 사이의 정점 두개를 구한다.
        Vector3 j1 = Vector3::IntersectNearPlane(vertices[findidx], vertices[nextIdx], nearPlane);
        Vector3 j2 = Vector3::IntersectNearPlane(vertices[findidx], vertices[preIdx], nearPlane);
        return{vertices[findidx],j1,j2};
    }
    case 2:
    {
        int outidx = -1;//나간정점
        for (int i = 0; i < insides.size(); ++i)
        {
            if (insides[i] == false)
            {
                outidx = i;
                break;
            }
        }
        //나간 정점에서 이전과 이후의 정점을 찾은 다음.
        int nextIdx = (outidx + 1) % 3;
        int preIdx = (outidx + 2) % 3;
        //나간과 안쪽 정점 사이의 정점 두개를 구한다.
        Vector3 j1 = Vector3::IntersectNearPlane(vertices[outidx], vertices[nextIdx], nearPlane);
        Vector3 j2 = Vector3::IntersectNearPlane(vertices[outidx], vertices[preIdx], nearPlane);
        return{vertices[nextIdx],vertices[preIdx],j2,j1};
    }
    case 3: return {p,q,r};
    default: break;
    }

    return {};
}
bool CheckClipTriangleTwoInside()
{
    Vector3 p(0, 0, 0);
    Vector3 q(2, 0, 2);
    Vector3 r(0, 2, 2);

    std::vector<Vector3> result =  ClipTriangleNearPlane(p, q, r, 1.0f);


    return false;
}


int main()
{
    
    std::cout << CheckClipTriangleTwoInside() << std::endl;
}