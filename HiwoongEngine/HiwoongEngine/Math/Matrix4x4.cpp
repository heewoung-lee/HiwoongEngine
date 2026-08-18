#include "Matrix4x4.h"
#include <cstddef>
#include <cmath>
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
}


