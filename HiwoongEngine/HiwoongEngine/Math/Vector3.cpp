#include "Math/Vector3.h"
#include <cmath>
#include <cassert>


namespace Hiwoong
{

    Vector3::Vector3(float newX, float newY, float newZ)
        : x(newX), y(newY), z(newZ)
    {
    }
    Vector3 Vector3::Zero(0, 0, 0);

    Vector3 Vector3::operator+(const Vector3& other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 Vector3::operator-(const Vector3& other) const
    {
        return  Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 Vector3::operator*(const Vector3& other) const
    {
        return  Vector3(x * other.x, y * other.y, z * other.z);
    }
    Vector3 Vector3::operator*(float value) const
    {
        return  Vector3(x * value, y * value, z * value);
    }
    Vector3 Vector3::operator/(const Vector3& other) const
    {
        //The denominator cannot be zero.
        assert(
            other.x != 0.0f &&
            other.y != 0.0f &&
            other.z != 0.0f
        );

        return Vector3(x / other.x, y / other.y, z / other.z);
    }
    Vector3& Vector3::operator=(const Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }
    Vector3& Vector3::operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }


    float Vector3::Length() const
    {
        return sqrt(x*x + y*y + z*z);
    }
    float Vector3::Dot(const Vector3& a, const Vector3& b)
    {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }
    Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
    bool Vector3::NearyEquals(const Vector3& a, const Vector3& b)
    {
        return std::abs(a.x - b.x) < Epsilon &&
            std::abs(a.y - b.y) < Epsilon &&
            std::abs(a.z - b.z) < Epsilon;
    }
    Vector3 Vector3::Normalized() const
    {
        float length = Length();

        assert(length != 0.0f);

        return Vector3(x / length, y / length, z / length);
    }
    Vector3 Vector3::IntersectNearPlane(const Vector3& inside, const Vector3& outside, float nearPlane)
    {
        float t = (nearPlane - inside.z) / (outside.z - inside.z);

        return Vector3(
            inside.x + t * (outside.x - inside.x),
            inside.y + t * (outside.y - inside.y),
            inside.z + t * (outside.z - inside.z)
        );
    }
    bool Vector3::operator==(const Vector3& other) const
    {
        if (x == other.x && y == other.y && z == other.z)
        {
            return true;
        }
        return false;
    }
    bool Vector3::operator!=(const Vector3& other) const
    {
        if (x == other.x && y == other.y && z == other.z)
        {
            return false;
        }
        return true;
    }
}