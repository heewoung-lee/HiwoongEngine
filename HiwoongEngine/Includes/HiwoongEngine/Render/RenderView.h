#pragma once

#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"

namespace Hiwoong
{
	//이번 프레임에서 모든 MEsh가 공유할 카메라 계산 결과를 전달하는 구조체.
	//순서는 카메라에서 view, projection행렬 계산이 끝난뒤,
	//씬에게 계산된 결과를 전달하고
	//씬에서는 RenderView를 내보냄.
	//RenderView는 MeshRenderer로 보내지고
	//MeshRenderer에서 메쉬의 오브젝트 참조를 받아, 씬에게 전달받은 RenderView를
	//통해 메쉬의 오브젝트 모델행렬 -> RenderView의 뷰행렬,Projection행렬계산을
	//통해 최종적으로 렌더링함.
	struct RenderView
	{
		RenderView(
			const Matrix4x4& view,
			const Matrix4x4& projection,
			const Vector2& screenSize,
			const float nearPlane)
			: view(view), projection(projection), screenSize(screenSize), nearPlane(nearPlane)
		{
		}
		Matrix4x4 view; // 카메라 위치,회전으로 만든 행렬
		Matrix4x4 projection; //FOV 화면 비율 Near or Far 옵션으로 만든 행렬
		Vector2 screenSize;// 화면 크기.
		float nearPlane; // 거리
	};
}

