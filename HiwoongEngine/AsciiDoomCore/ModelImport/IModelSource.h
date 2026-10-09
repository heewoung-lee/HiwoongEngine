#pragma once


#include <cstddef>
#include <string>
#include <vector>
#include "ModelVertex.h"
#include "ModelTriangle.h"
namespace Hiwoong
{
	/// <summary>
	/// 읽어둔 원본 데이터를 조회하는 계약
	/// 이를 통해 가져온 메시정보 등 을 보고, 런타임에 필요한 정보들로 가공해서 쓸 숭 ㅣㅆ음.
	/// </summary>
	class IModelSource
	{
	public:
		virtual ~IModelSource() = default;


		//원본에 들어있는 메시 갯수.
		virtual std::size_t GetMeshCount() const = 0;

		// 지정한 원본 메시의 원래 이름.
		virtual std::string GetMeshName(
			std::size_t meshIndex
		)const = 0;

		// 지정한 원본 메쉬의 정점 갯수
		virtual std::size_t GetVertexCount(std::size_t meshindex) const = 0;

		// 지정한 원본 정점의 위치.
		// 예를 들어 Getvertex(0,2)는 0번 메시안에 있는 2번 정점의 위치
		virtual ModelVertex GetVertex(
			std::size_t meshIndex,
			std::size_t vertexIndex
		) const = 0;

		// 지정한 원본 메시의 면 개수
		virtual std::size_t GetFaceCount(std::size_t meshIndex) const = 0;

		// 지정한 면을 이루는 원본 정점 번호 목록
		//예를 들어 반환이 0,2,5라면
		//원본정점 0-2-5를 연결한 삼각형이라는 뜻.
		//이렇게 한 이유는 우리는 삼각형만 렌더링 할 수 있는데 원본에 
		//사각형이 있을 수 있기에. 번호를 목록으로 받음.
		virtual std::vector<std::size_t> GetFaceVertexIndices(
			std::size_t meshIndex,
			std::size_t faceIndex
		) const = 0;

		//해당 면을 삼각형으로 쪼개 원본 정점 번호로 반환한다.
		//즉 다각형을 받고 삼각형으로 쪼갠다
		virtual std::vector<ModelTriangle> TriangulateFace(
			std::size_t meshIndex,
			std::size_t faceIndex
		) const = 0;

	};
}