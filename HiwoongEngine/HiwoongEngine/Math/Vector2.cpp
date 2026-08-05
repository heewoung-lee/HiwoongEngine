#include "Vector2.h"
#include <cassert>

namespace Hiwoong
{
	Vector2 Vector2::Zero(0, 0);
	Vector2 Vector2::One(1, 1);
	Vector2 Vector2::Right(0, 1);
	Vector2 Vector2::Up(-1, 0);

	Vector2::Vector2(int x, int y) : x(x),y(y)
	{
	}
	Vector2::~Vector2()
	{
	}
	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 Vector2::operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y * other.y);
	}
	Vector2 Vector2::operator*(float value) const
	{
		return Vector2(x * value, y * value);
	}
	Vector2 Vector2::operator/(const Vector2& other) const
	{
		assert(other.x != 0 || other.y != 0);
		return Vector2(x / other.x , y/ other.y);
	}
	Vector2& Vector2::operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}
	bool Vector2::operator==(const Vector2& other) const
	{
		return x == other.x && y == other.y;
	}
	bool Vector2::operator!=(const Vector2& other) const
	{
		return  x != other.x || y != other.y;
	}
}
