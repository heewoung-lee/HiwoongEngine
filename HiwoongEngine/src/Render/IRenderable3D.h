#pragma once

#include "Core/Core.h"
#include "Render/Mesh.h"
#include "Render/RenderView.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"

namespace Hiwoong
{
	class Hiwoong_API IRenderable3D
	{
	public:
		virtual ~IRenderable3D() = default;
		virtual const Mesh& GetMesh() const = 0;
		virtual Matrix4x4 GetModelMatrix(const RenderView& renderView) const = 0;

		virtual Color GetRenderColor() const = 0;

		//픽셀의 정보를 넣을 때 나오는 문자.
		virtual bool TryGetCharactor(
			float u,
			float v,
			float brightness,
			char& outCharacter
		) const = 0;

	};
}

