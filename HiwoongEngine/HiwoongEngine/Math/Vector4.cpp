#include "Vector4.h"
#include "Math/Vector3.h"
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
}

