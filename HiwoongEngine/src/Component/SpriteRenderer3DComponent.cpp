#include "SpriteRenderer3DComponent.h"
#include "GameObject/GameObject.h"
#include "Math/Vector4.h"
#include "Render/SoftwareRasterizer.h"
#include <cassert>

namespace Hiwoong
{
	void SpriteRenderer3DComponent::Start()
	{
		super::Start();

		transform = GetComponent<TransformComponent>();
		assert(transform != nullptr);

	}
	void SpriteRenderer3DComponent::Draw()
	{

	}

	void SpriteRenderer3DComponent::Render(const RenderView& renderView)
	{
		assert(transform != nullptr);

		const Vector3 worldPosition = transform->GetWorldPosition();

		//알죠? 뷰행렬 계산을 통해서 월드포지션 까지 계산한 객체의 포지션(모델행렬 계산후)을 카메라 포지션으로 옮기는거랍니다.
		const Vector4 cameraPosition =
			renderView.view * Vector4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);

		if (cameraPosition.z < renderView.nearPlane) return; // 카메라 뒤쪽이나 nearPlane 보다 가까운 위치는 제외.

		const Vector4 clipPos = renderView.projection * cameraPosition;//투영행렬 계산.



	}
}