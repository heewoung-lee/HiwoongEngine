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

		// 지정한 원본 정점의 위치.
		// 예를 들어 Getvertex(0,2)는 0번 메시안에 있는 2번 정점의 위치
		virtual ModelVertex GetVertex(
			std::size_t meshIndex,
			std::size_t vertexIndex
		) const = 0;

		// 지정한 원본 메시의 면 개수
		virtual std::size_t GetFaceCount(std::size_t meshIndex) const = 0;

		//해당 면을 삼각형으로 쪼개 원본 정점 번호로 반환한다.
		//즉 다각형을 받고 삼각형으로 쪼갠다
		virtual std::vector<ModelTriangle> TriangulateFace(
			std::size_t meshIndex,
			std::size_t faceIndex
		) const = 0;

		// 모든 면의 꼭지점 갯수.
		// 공유하는 위치라도 면이 다르면 각각 새기 때문에 필요
		// 예를 들어 같은 위치의 정점이라도 면마다 UV좌표가 다를 수 있음.모서리 부분
		virtual std::size_t GetCornerCount(
			std::size_t meshIndex
		) const = 0;
		
		// 해당 코너에서 사용하는 위치와 UV를 반환한다.
		virtual ModelVertex GetCornerVertex(
			std::size_t meshIndex,
			std::size_t cornerIndex
		) const = 0;

		// 이 코너가 사용하는 원본 정점 번호를 반환한다.
		// 그러니깐 같은 정점인데 면에따라 UV가 다를 수 있기에
		// 정확한 정점번호를 찾기위함.
		virtual std::size_t GetCornerSourceVertexIndex(
			std::size_t meshIndex,
			std::size_t cornerIndex
		) const = 0;

		// 재질 번호 순서의 색상 이미지 경로.
		// 이미지가 없는 재질도 빈 문자열로 자리를 유지한다.
		virtual std::vector<std::string> GetDiffuseTexturePaths(
			std::size_t meshIndex
		) const = 0;

	};
}