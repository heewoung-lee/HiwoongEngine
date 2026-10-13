#pragma once

#include "../ModelImport/ModelMesh.h"
#include "Math/Matrix4x4.h"
#include <vector>

namespace Hiwoong
{		
	/// <summary>
	/// 관절 행렬과 가중치로 정점의 새 위치를 계산하는 클래스
	/// </summary>
	class SkinningCalculator
	{
	public:
		//기존 정점의 위치와 UV를 복사해서 반환하는 틀
		static std::vector<ModelVertex> Calculate(
			const ModelMesh& mesh,
			const std::vector<Matrix4x4>& nodeMatrices,
			const Matrix4x4& modelAlignment
		);


	private:
		//관절 번호에 맞는 바인딩을 찾는 함수
		static int FindBindingIdx(
			const std::vector<SkinBinding>& bindings,
			int nodeIdx
		);

	};
}