
#include <iostream>
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Render/Vertex.h"
#include <cmath>

namespace
{
    Hiwoong::Vector3 testVector(3.0f, 4.0f, 0.0f);
}



    bool TestVector3Constructor()
    {
        Hiwoong::Vector3 vector(1.0f, 2.0f, 3.0f);

        return vector.x == 1.0f && vector.y == 2.0f && vector.z == 3.0f;
    }


    bool TestGetLength()
    {
        float x = 1.0f;
        float y = 2.0f;
        float z = 3.0f;
        Hiwoong::Vector3 vector(x, y, z);

        return vector.Length() == sqrt(x*x + y*y + z*z);
    }


    bool GetNormalized()
    {
        Hiwoong::Vector3 normal = testVector.Normalized();
        
        Hiwoong::Vector3 testNormal(0.6f, 0.8f, 0);
        return testNormal == normal;
    }


    bool GetDot()
    {
        Hiwoong::Vector3 a(1,2,3);
        Hiwoong::Vector3 b(4, 5, 6);

        return Hiwoong::Vector3::Dot(a,b) == 32;
    }

    bool TestCross()
    {
        Hiwoong::Vector3 forward(0, 0, 1);
        Hiwoong::Vector3 left(-1, 0, 0);
        Hiwoong::Vector3 expectedUp(0, -1, 0);

        return Hiwoong::Vector3::Cross(forward, left) == expectedUp;
    }

    bool TestIdentityMatrix()
    {
        Hiwoong::Matrix4x4 identity =
            Hiwoong::Matrix4x4::Identity();

        Hiwoong::Vector4 input(1, 2, 3, 1);

        Hiwoong::Vector4 result = identity * input;

        return result.x == input.x &&
            result.y == input.y &&
            result.z == input.z &&
            result.w == input.w;
    }

    bool TestTranslationDoesNotMoveDirection()
    {
        Hiwoong::Matrix4x4 matrix =
            Hiwoong::Matrix4x4::Translation(
                Hiwoong::Vector3(5, 6, 7)
            );

        Hiwoong::Vector4 direction(1, 2, 3, 0);

        Hiwoong::Vector4 result = matrix * direction;

        return result.x == 1 &&
            result.y == 2 &&
            result.z == 3 &&
            result.w == 0;
    }

    bool TestScaleMatrix()
    {
        Hiwoong::Matrix4x4 matrix =
            Hiwoong::Matrix4x4::Scale(
                Hiwoong::Vector3(2, 3, 4)
            );

        Hiwoong::Vector4 position(1, 2, 3, 1);

        Hiwoong::Vector4 result = matrix * position;

        return result.x == 2 &&
            result.y == 6 &&
            result.z == 12 &&
            result.w == 1;
    }

    bool TestRotationZ()
    {
        constexpr float Pi = 3.14159265f;
        constexpr float HalfPi = Pi / 2.0f;

        Hiwoong::Matrix4x4 matrix =
            Hiwoong::Matrix4x4::RotationZ(HalfPi);

        Hiwoong::Vector4 right(1, 0, 0, 0);
        Hiwoong::Vector4 result = matrix * right;

        constexpr float Epsilon = 0.0001f;

        return std::abs(result.x - 0.0f) < Epsilon &&
            std::abs(result.y - 1.0f) < Epsilon &&
            std::abs(result.z - 0.0f) < Epsilon &&
            std::abs(result.w - 0.0f) < Epsilon;
    }

    bool TestRotationX()
    {
        constexpr float Pi = 3.14159265f;
        constexpr float HalfPi = Pi / 2.0f;
        constexpr float Epsilon = 0.0001f;

        Hiwoong::Matrix4x4 matrix =
            Hiwoong::Matrix4x4::RotationX(HalfPi);

        Hiwoong::Vector4 down(0, 1, 0, 0);
        Hiwoong::Vector4 result = matrix * down;

        return std::abs(result.x - 0.0f) < Epsilon &&
            std::abs(result.y - 0.0f) < Epsilon &&
            std::abs(result.z - 1.0f) < Epsilon &&
            std::abs(result.w - 0.0f) < Epsilon;
    }

    bool TestRotationY()
    {
        constexpr float Pi = 3.14159265f;
        constexpr float HalfPi = Pi / 2.0f;
        constexpr float Epsilon = 0.0001f;

        Hiwoong::Matrix4x4 matrix =
            Hiwoong::Matrix4x4::RotationY(HalfPi);

        Hiwoong::Vector4 forward(0, 0, 1, 0);
        Hiwoong::Vector4 result = matrix * forward;

        return std::abs(result.x - 1.0f) < Epsilon &&
            std::abs(result.y - 0.0f) < Epsilon &&
            std::abs(result.z - 0.0f) < Epsilon &&
            std::abs(result.w - 0.0f) < Epsilon;
    }



    bool TestMatrixMultiplication()
    {
        Hiwoong::Matrix4x4 translation =
            Hiwoong::Matrix4x4::Translation(
                Hiwoong::Vector3(10, 20, 30)
            );

        Hiwoong::Matrix4x4 scale =
            Hiwoong::Matrix4x4::Scale(
                Hiwoong::Vector3(2, 3, 4)
            );

        Hiwoong::Matrix4x4 combined = translation * scale;

        Hiwoong::Vector4 position(1, 2, 3, 1);
        Hiwoong::Vector4 result = combined * position;

        return result.x == 12 &&
            result.y == 26 &&
            result.z == 42 &&
            result.w == 1;
    }
    bool TestModelMatrixOrder()
    {
        constexpr float Pi = 3.14159265f;
        constexpr float HalfPi = Pi / 2.0f;
        constexpr float Epsilon = 0.0001f;

        Hiwoong::Matrix4x4 model =
            Hiwoong::Matrix4x4::Translation(
                Hiwoong::Vector3(10, 20, 30)
            )
            * Hiwoong::Matrix4x4::RotationZ(HalfPi)
            * Hiwoong::Matrix4x4::Scale(
                Hiwoong::Vector3(2, 1, 1)
            );

        Hiwoong::Vector4 localPosition(1, 0, 0, 1);
        Hiwoong::Vector4 result = model * localPosition;

        return std::abs(result.x - 10.0f) < Epsilon &&
            std::abs(result.y - 22.0f) < Epsilon &&
            std::abs(result.z - 30.0f) < Epsilon &&
            std::abs(result.w - 1.0f) < Epsilon;
    }
    bool TestViewMatrix()
    {
        Hiwoong::Matrix4x4 view =
            Hiwoong::Matrix4x4::LookAt(
                Hiwoong::Vector3(0, 0, 5),  // 카메라 위치
                Hiwoong::Vector3(0, 0, 6),  // 바라보는 위치
                Hiwoong::Vector3(0, -1, 0)  // 위쪽 방향
            );

        Hiwoong::Vector4 worldPosition(0, 0, 10, 1);
        Hiwoong::Vector4 result = view * worldPosition;

        return result.x == 0 &&
            result.y == 0 &&
            result.z == 5 &&
            result.w == 1;
    }
    bool TestViewMatrixX()
    {
        Hiwoong::Matrix4x4 view =
            Hiwoong::Matrix4x4::LookAt(
                Hiwoong::Vector3(0, 0, 0),   // 카메라
                Hiwoong::Vector3(1, 0, 0),   // +X 방향
                Hiwoong::Vector3(0, -1, 0)   // 위쪽
            );

        Hiwoong::Vector4 worldPosition(5, 0, 0, 1);
        Hiwoong::Vector4 result = view * worldPosition;

        return result.x == 0 &&
            result.y == 0 &&
            result.z == 5 &&
            result.w == 1;
    }
    bool TestPerspectiveMakesFarObjectsSmaller()
    {
        constexpr float HalfPi = 3.14159265f / 2.0f;

        Hiwoong::Matrix4x4 projection =
            Hiwoong::Matrix4x4::Perspective(
                HalfPi,
                1.0f,
                0.1f,
                100.0f
            );

        Hiwoong::Vector4 closePoint(1, 0, 2, 1);
        Hiwoong::Vector4 farPoint(1, 0, 4, 1);

        Hiwoong::Vector4 closeClip = projection * closePoint;
        Hiwoong::Vector4 farClip = projection * farPoint;

        float closeX = closeClip.x / closeClip.w;
        float farX = farClip.x / farClip.w;

        return std::abs(closeX) > std::abs(farX);
    }
    bool TestPerspectiveDepthRange()
    {
        constexpr float HalfPi = 3.14159265f / 2.0f;
        constexpr float Epsilon = 0.0001f;

        Hiwoong::Matrix4x4 projection =
            Hiwoong::Matrix4x4::Perspective(
                HalfPi,
                1.0f,
                1.0f,
                10.0f
            );

        Hiwoong::Vector4 nearClip =
            projection * Hiwoong::Vector4(0, 0, 1, 1);

        Hiwoong::Vector4 farClip =
            projection * Hiwoong::Vector4(0, 0, 10, 1);

        const float nearDepth = nearClip.z / nearClip.w;
        const float farDepth = farClip.z / farClip.w;

        return std::abs(nearDepth - 0.0f) < Epsilon &&
            std::abs(farDepth - 1.0f) < Epsilon;
    }


    bool RunAllTests()
    {
        bool constructorPassed = TestVector3Constructor();
        bool lengthPassed = TestGetLength();
        bool normalizedPassed = GetNormalized();
        bool dotPassed = GetDot();
        bool crossPassed = TestCross();
        bool identityPassed = TestIdentityMatrix();
        bool scalePassed = TestScaleMatrix();
        bool rotationXPassed = TestRotationX();
        bool rotationYPassed = TestRotationY();
        bool rotationZPassed = TestRotationZ();
        bool translationDirectionPassed =
            TestTranslationDoesNotMoveDirection();

        bool testViewMatrix =  TestViewMatrix();

        return constructorPassed &&
            lengthPassed &&
            normalizedPassed &&
            dotPassed &&
            crossPassed &&
            identityPassed &&
            scalePassed &&
            rotationXPassed &&
            rotationYPassed &&
            rotationZPassed &&
            translationDirectionPassed &&
            testViewMatrix;
    }

    bool TestPerspectiveDivide()
    {
        Hiwoong::Vector4 clipPosition(4, 2, 8, 2);

        Hiwoong::Vector3 result =
            clipPosition.PerspectiveDivide();

        constexpr float Epsilon = 0.0001f;

        return std::abs(result.x - 2.0f) < Epsilon &&
            std::abs(result.y - 1.0f) < Epsilon &&
            std::abs(result.z - 4.0f) < Epsilon;
    }
    bool TestModelViewProjection()
    {
        constexpr float HalfPi = 3.14159265f / 2.0f;
        constexpr float Epsilon = 0.0001f;

        Hiwoong::Matrix4x4 model =
            Hiwoong::Matrix4x4::Translation(
                Hiwoong::Vector3(1, 0, 10)
            );

        Hiwoong::Matrix4x4 view =
            Hiwoong::Matrix4x4::LookAt(
                Hiwoong::Vector3(0, 0, 5),
                Hiwoong::Vector3(0, 0, 6),
                Hiwoong::Vector3(0, -1, 0)
            );

        Hiwoong::Matrix4x4 projection =
            Hiwoong::Matrix4x4::Perspective(
                HalfPi, 1.0f, 1.0f, 100.0f
            );

        Hiwoong::Matrix4x4 mvp =
            projection * view * model;

        Hiwoong::Vector4 localPosition(0, 0, 0, 1);

        Hiwoong::Vector3 screenPosition =
            (mvp * localPosition).PerspectiveDivide();

        return std::abs(screenPosition.x - 0.2f) < Epsilon &&
            std::abs(screenPosition.y) < Epsilon &&
            screenPosition.z >= 0.0f &&
            screenPosition.z <= 1.0f;
    }
    bool TestVertexPosition()
    {
        Hiwoong::Vertex vertex{
            Hiwoong::Vector3(1, 2, 3)
        };

        return vertex.position ==
            Hiwoong::Vector3(1, 2, 3);
    }

    int main()
    {
        if (TestVertexPosition() == true)
        {
            std::cout << "Success" << std::endl;
        }
        else
        {
            std::cout << "failure" << std::endl;
        }

        return 0;
    }

