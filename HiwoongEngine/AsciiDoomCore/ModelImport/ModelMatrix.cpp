#include "ModelMatrix.h"

namespace Hiwoong
{
	ModelVertex ModelMatrix::TransformPoint(
		const ModelVertex& vertex
	) const
	{
		ModelVertex result = vertex;

		result.x =
			values[0][0] * vertex.x +
			values[0][1] * vertex.y +
			values[0][2] * vertex.z +
			values[0][3];

		result.y =
			values[1][0] * vertex.x +
			values[1][1] * vertex.y +
			values[1][2] * vertex.z +
			values[1][3];

		result.z =
			values[2][0] * vertex.x +
			values[2][1] * vertex.y +
			values[2][2] * vertex.z +
			values[2][3];

		return result;
	}
	ModelMatrix ModelMatrix::Multiply(const ModelMatrix& other) const
	{
		ModelMatrix result;

		for (int row = 0; row < 4; ++row)
		{
			for (int column = 0; column < 4; ++column)
			{
				result.values[row][column] =
					values[row][0] * other.values[0][column] +
					values[row][1] * other.values[1][column] +
					values[row][2] * other.values[2][column] +
					values[row][3] * other.values[3][column];
			}
		}

		return result;
	}
	ModelMatrix ModelMatrix::FromNode(const SkeletonNode& node)
	{
		ModelMatrix result;

		const float x = node.localRotationX;
		const float y = node.localRotationY;
		const float z = node.localRotationZ;
		const float w = node.localRotationW;

		// 저장된 회전값을 행렬로 바꾸는 공식.
		result.values[0][0] = 1.0f - 2.0f * (y * y + z * z);
		result.values[0][1] = 2.0f * (x * y - z * w);
		result.values[0][2] = 2.0f * (x * z + y * w);

		result.values[1][0] = 2.0f * (x * y + z * w);
		result.values[1][1] = 1.0f - 2.0f * (x * x + z * z);
		result.values[1][2] = 2.0f * (y * z - x * w);

		result.values[2][0] = 2.0f * (x * z - y * w);
		result.values[2][1] = 2.0f * (y * z + x * w);
		result.values[2][2] = 1.0f - 2.0f * (x * x + y * y);

		// 크기를 반영.
		for (int row = 0; row < 3; ++row)
		{
			result.values[row][0] *= node.localScaleX;
			result.values[row][1] *= node.localScaleY;
			result.values[row][2] *= node.localScaleZ;
		}

		// 위치를 반영.
		result.values[0][3] = node.localPositionX;
		result.values[1][3] = node.localPositionY;
		result.values[2][3] = node.localPositionZ;

		return result;
	}
}