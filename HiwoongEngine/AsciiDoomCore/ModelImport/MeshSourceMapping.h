#pragma once

#include <cstddef>
#include <vector>
namespace Hiwoong
{
	struct MeshSourceMapping
	{
		// FBX에서 불러온 원본 메시 번호
		std::size_t sourceMeshIndex = 0;

		//해당 원본 메시 안에서 각 출력 정점이 어느 원본 정점에서 왔는가?
		std::vector<std::size_t> sourceVertexIndices;


	};


}
