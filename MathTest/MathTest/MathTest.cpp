
#include <iostream>
#include "Math/Vector3.h"
#include "Math/AABBCollider.h"
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

    Vector3 expectJ2(0, 1, 1);
    Vector3 expectJ1(1, 0, 1);

    if (result.size() != 4)
    {
        return false;
    }
    return q == result[0]&& r == result[1] && expectJ2 == result[2] && expectJ1 == result[3];
}
bool CheckClipTriangleOneInside()
{
    Vector3 p(0, 0, 2);
    Vector3 q(2, 0, 0);
    Vector3 r(0, 2, 0);

    std::vector<Vector3> result = ClipTriangleNearPlane(p, q, r, 1.0f);

    Vector3 expectJ1(1, 0, 1);
    Vector3 expectJ2(0, 1, 1);

    if (result.size() != 3)
    {
        return false;
    }
    return p == result[0] && expectJ1 == result[1] && expectJ2 == result[2];
}
bool CheckClipTriangleAllOutside()
{
    Vector3 p(0, 0, 0);
    Vector3 q(2, 0, 0.5f);
    Vector3 r(0, 2, -1);

    std::vector<Vector3> result = ClipTriangleNearPlane(p, q, r, 1.0f);

    if (result.size() != 0)
    {
        return false;
    }
    return true;
}

bool CheckClipTriangleAllInside()
{
    Vector3 p(0, 0, 1);
    Vector3 q(2, 0, 1);
    Vector3 r(0, 2, 1);

    std::vector<Vector3> result = ClipTriangleNearPlane(p, q, r, 1.0f);

    if (result.size() != 3) return false;

    return result[0] == p && result[1] == q && result[2] == r;
}

bool CheckAllcaseOfTriale()
{
    return CheckClipTriangleTwoInside() &&
        CheckClipTriangleOneInside() &&
        CheckClipTriangleAllOutside() &&
        CheckClipTriangleAllInside();
}

bool CheckRayPlaneIntersection()
{
    Vector3 origin(1, 0, 2);
    Vector3 direction(1, 0, 0);

    Vector3 planePoint(5, 0, 0);
    Vector3 planeNormal(1, 0, 0);

    float result = Vector3::RayPlaneIntersectionT(
        origin,
        direction,
        planePoint,
        planeNormal);

    std::cout << result << std::endl;

    return std::abs(result - 4.0f) < 0.0001f;
}
//Ray와 벽이 평행할 때 교점이 없다고 판단하는지 확인하는 테스트.
bool CheckRayPlaneParallel()
{
    Vector3 origin(0, 0, 0);
    Vector3 direction(0, 0, 1);

    Vector3 planePoint(5, 0, 0);
    Vector3 planeNormal(1, 0, 0);

    float result = Vector3::RayPlaneIntersectionT(
        origin,
        direction,
        planePoint,
        planeNormal);
    
    std::cout << result << std::endl;

    return std::abs(result - (-1.0f)) < 0.0001f;
}

bool CheckRayPlaneBehind()
{
    Vector3 origin(5, 0, 0);
    Vector3 direction(1, 0, 0);

    Vector3 planePoint(1, 0, 0);
    Vector3 planeNormal(1, 0, 0);

    float result = Vector3::RayPlaneIntersectionT(
        origin,
        direction,
        planePoint,
        planeNormal);

    std::cout << result << std::endl;

    return std::abs(result - (-1.0f)) < 0.0001f;
}

bool CheckGetRayPoint()
{
    Vector3 start(1, 0, 2);
    Vector3 direction(1,0,0);
    float t = 4;

    Vector3 expect(5, 0, 2);

    return Vector3::GetPointOnRay(start, direction,t) == expect;
}

bool CheckOverlapATOB()
{
    AABBCollider player(2.5f, 3.5f, 4.5f, 5.5f);
    AABBCollider wall(3.2f,4.2f,5.2f,6.2f);

    bool expect = true;

    return expect == player.IsOverlapping(wall);
}

bool CheckAABBSeparatedOnZ()
{
    AABBCollider player(2.5f, 3.5f, 4.5f, 5.5f);
    AABBCollider wall(3.2f, 4.2f, 7.0f, 8.0f);

    bool expect = false;

    return expect == player.IsOverlapping(wall);
}

bool CheckCollisionTest()
{
    AABBCollider small(2, 3, 2, 3);
    AABBCollider big(0, 10, 0, 10);
    
    bool expect = true;

    return expect == small.IsOverlapping(big);
}

bool CheckWallSlide()
{
        Vector3 currentPos(3,0,5);
        Vector3 movement(1,0,2);
        float halfX = 0.5f;
        float halfZ = 1;
        AABBCollider wall(4,5,0,10);

        Vector3 expect(3,0,7);

        return AABBCollider::MoveWithWallSlide(
            currentPos, 
            movement,
            halfX,
            halfZ,
            wall) == expect;
}

bool CheckWallSlideNoCollision()
{
    Vector3 currentPos(0, 0, 0);
    Vector3 movement(1, 0, 2);
    float halfX = 0.5f;
    float halfZ = 1;
    AABBCollider wall(10, 11, 10, 11);

    Vector3 expect(1, 0, 2);

    return  expect == AABBCollider::MoveWithWallSlide(currentPos, movement, halfX, halfZ, wall);
}
bool CheckWallSlideZBlocked()
{
    Vector3 currentPos(3, 0, 3);
    Vector3 movement(2, 0, 1);
    float halfX = 0.5f;
    float halfZ = 0.5;
    AABBCollider wall(0, 10, 4, 5);

    Vector3 expect(5, 0, 3);

    return  expect == AABBCollider::MoveWithWallSlide(currentPos, movement, halfX, halfZ, wall);

}

bool CheckWallSlideBothBlocked()
{
    Vector3 currentPos(3, 0, 3);
    Vector3 movement(1, 0, 1);
    float halfX = 0.5f;
    float halfZ = 0.5;
    AABBCollider wall(3.5,4.5, 3.5, 4.5);

    Vector3 expect(3, 0, 3);

    return  expect == AABBCollider::MoveWithWallSlide(currentPos, movement, halfX, halfZ, wall);

}

int main()
{
    
    std::cout << CheckWallSlideBothBlocked() << std::endl;
}