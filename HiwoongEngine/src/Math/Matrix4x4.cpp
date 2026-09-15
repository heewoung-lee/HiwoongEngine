#include "Matrix4x4.h"
#include <cstddef>
#include <cmath>
#include <cassert>
namespace Hiwoong 
{
    Hiwoong::Matrix4x4::Matrix4x4() : values{}
    {
    }

    Matrix4x4 Hiwoong::Matrix4x4::Identity()
    {
        Matrix4x4 result;

        for (std::size_t index = 0; index < 4; ++index)
        {
            result.values[index][index] = 1.0f;
        }
        return result;
    }

    Vector4 Hiwoong::Matrix4x4::operator*(const Vector4& vector) const
    {
        return Vector4(
            values[0][0] * vector.x +
            values[0][1] * vector.y +
            values[0][2] * vector.z +
            values[0][3] * vector.w,

            values[1][0] * vector.x +
            values[1][1] * vector.y +
            values[1][2] * vector.z +
            values[1][3] * vector.w,

            values[2][0] * vector.x +
            values[2][1] * vector.y +
            values[2][2] * vector.z +
            values[2][3] * vector.w,

            values[3][0] * vector.x +
            values[3][1] * vector.y +
            values[3][2] * vector.z +
            values[3][3] * vector.w
        );
    }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
    {
        Matrix4x4 result;
        for (size_t row = 0; row < 4; ++row)
        {
            for (size_t col = 0; col < 4; ++col)
            {
                for (size_t idx = 0; idx < 4; ++idx)
                {
                    result.values[row][col] += values[row][idx] * other.values[idx][col];
                }
            }
        }
        return result;
    }
   


    Matrix4x4 Matrix4x4::Translation(const Vector3& translation)
    {
        Matrix4x4 result = Identity();

        result.values[0][3] = translation.x;
        result.values[1][3] = translation.y;
        result.values[2][3] = translation.z;

        return result;
    }

    Matrix4x4 Matrix4x4::RotationZ(float radians)
    {
        Matrix4x4 result = Identity();

        const float cosine = std::cos(radians);
        const float sine = std::sin(radians);

        result.values[0][0] = cosine;
        result.values[0][1] = -sine;

        result.values[1][0] = sine;
        result.values[1][1] = cosine;

        return result;
    }

    Matrix4x4 Matrix4x4::RotationX(float radians)
    {
        Matrix4x4 result = Identity();

        const float cosine = std::cos(radians);
        const float sine = std::sin(radians);

        result.values[1][1] = cosine;
        result.values[1][2] = -sine;

        result.values[2][1] = sine;
        result.values[2][2] = cosine;

        return result;
    }

    Matrix4x4 Matrix4x4::RotationY(float radians)
    {
        Matrix4x4 result = Identity();

        const float cosine = std::cos(radians);
        const float sine = std::sin(radians);

        result.values[0][0] = cosine;
        result.values[0][2] = sine;

        result.values[2][0] = -sine;
        result.values[2][2] = cosine;

        return result;
    }


    Matrix4x4 Matrix4x4::Scale(const Vector3& scale)
    {
        Matrix4x4 result = Identity();

        result.values[0][0] = scale.x;
        result.values[1][1] = scale.y;
        result.values[2][2] = scale.z;

        return result;
    }


    //Convert  WorldPosition To CameraPosition
    //Create Camera's View Matrix
    /// @param cameraPosition CameraPosition
    /// @param target Look At the Target
    /// @param up The camera's up direction.
    /// @return calculated View Matrix
    Matrix4x4 Matrix4x4::LookAt(const Vector3& cameraPosition, const Vector3& target, const Vector3& up)
    {
        const Vector3 forward = (target - cameraPosition).Normalized();
        const Vector3 right = Vector3::Cross(forward, up).Normalized();
        const Vector3 down = Vector3::Cross(forward, right).Normalized();

        Matrix4x4 result = Identity();

        result.values[0][0] = right.x;
        result.values[0][1] = right.y;
        result.values[0][2] = right.z;
        result.values[0][3] =
            -Vector3::Dot(right, cameraPosition);

        result.values[1][0] = down.x;
        result.values[1][1] = down.y;
        result.values[1][2] = down.z;
        result.values[1][3] =
            -Vector3::Dot(down, cameraPosition);

        result.values[2][0] = forward.x;
        result.values[2][1] = forward.y;
        result.values[2][2] = forward.z;
        result.values[2][3] =
            -Vector3::Dot(forward, cameraPosition);

        return result;

    }

    Matrix4x4 Matrix4x4::Perspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
    {
        assert(aspectRatio > 0.0f);
        assert(nearPlane > 0.0f);
        assert(farPlane > nearPlane);

        Matrix4x4 result;

        //Convert the vertical field of view into a projection scale.
        const float yScale = 1.0f / std::tan(fieldOfView * 0.5f);
        // Correct the horizontal scale using the sceen's aspect ratio.
        const float xScale = yScale / aspectRatio;
        // Calculate the visible depth range between the near and far planes.
        const float depthRange = farPlane - nearPlane;

        //Apply ratio to horizon
        result.values[0][0] = xScale;
        //Apply ratio to vertical
        result.values[1][1] = yScale;
        //make value for indicator which how to far
        result.values[2][2] = farPlane / depthRange;
        result.values[2][3] = -(nearPlane * farPlane) / depthRange;
        //current object distance
        result.values[3][2] = 1.0f;

        return result;
    }

}


