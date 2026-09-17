#include "MeshRenderComponent.h"
#include "GameObject/GameObject.h"

#include <cassert>


namespace Hiwoong
{
	MeshRenderComponent::MeshRenderComponent(
		const Mesh& mesh,
		Color color)
		: mesh(mesh), color(color)
	{
	}

	void MeshRenderComponent::Start()
	{
		super::Start();

		transform = GetComponent<TransformComponent>();
		assert(transform != nullptr);
	}

	void MeshRenderComponent::Render(const RenderView& renderView)
	{
		assert(transform != nullptr);

		const Matrix4x4 model =
			Matrix4x4::Translation(transform->GetWorldPosition());

		meshRenderer.Render(mesh, model, renderView, color);

	}

}
