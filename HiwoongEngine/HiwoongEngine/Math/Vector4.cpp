#include "Vector4.h"
#include "Math/Vector3.h"
#include <cmath>
#include <cassert>
namespace Hiwoong
{
	Vector4::Vector4(float newX, float newY, float newZ, float newW)
		:x(newX), y(newY), z(newZ), w(newW)
	{

	}
    Vector3 Vector4::PerspectiveDivide() const
    {
        assert(w != 0.0f);

        return Vector3(
            x / w,
            y / w,
            z / w
        );
    }
    bool Vector4::NearyEquals(const Vector4& a, const Vector4& b)
    {
        return std::abs(a.x - b.x) < Epsilon &&
            std::abs(a.y - b.y) < Epsilon &&
            std::abs(a.z - b.z) < Epsilon &&
            std::abs(a.w - b.w) < Epsilon ;
    }

    bool Vector4::operator==(const Vector4& other) const
    {
        return
            x == other.x&&
            y == other.y&&
            z == other.z&&
            w == other.w
            ;
    }
}

