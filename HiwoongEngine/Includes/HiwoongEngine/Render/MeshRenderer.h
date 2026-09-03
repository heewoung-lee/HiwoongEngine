#pragma once
#include "Core/Core.h"
#include "Render/Mesh.h"
#include "Math/Matrix4x4.h"
#include "Render/RenderView.h"

namespace Hiwoong
{
	class Hiwoong_API MeshRenderer
	{

	public : 
		//메쉬를 모델->뷰->프로젝션 행렬순으로 계산한뒤 렌더링
		void Render
		(
			const Mesh& mesh,
			const Matrix4x4& model,
			const RenderView& renderView
		) const;
	};
}
