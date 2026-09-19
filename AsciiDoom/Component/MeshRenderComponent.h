#pragma once
#include "Component/Component.h"
#include "Render/Mesh.h"
#include "Render/RenderView.h"
#include "Component/TransformComponent.h"
#include "Math/Color.h"
#include "Render/IRenderable3D.h"
#include <memory>

namespace Hiwoong
{

	//9.17일 굳이 GameOjbect에 렌더러가 있음에도 컴포넌트를 만든이유는
	//좀더 쉽게 렌더링을 하기 위함.
	//기존에 Renderer는 렌더링을 하기 위해서 메쉬, 모델, 렌더뷰 세개의 매개변수가 필요했는데.
	//이미 GameObject가 Mesh와 모델을 알기 때문에 새로바뀐 렌더뷰만 던지면됨.
	//즉 쉽게 렌더링을 하기위해 이미지 교체용 컴포넌트를 만듦.
	//맘에 안들지만, 우선 이렇게 만들고 나중에 엔진을 수정하던가 해야겠다.
	
	class MeshRenderComponent : public Component, public IRenderable3D
	{
		TYPE_DECALRATIONS(MeshRenderComponent,Component)
		
	public:
		explicit MeshRenderComponent(
			const Mesh& mesh,
			Color color = Color::White
		);

		void Start() override;

		const Mesh& GetMesh() const override;
		Matrix4x4 GetModelMatrix(const RenderView& renderView) const override;
		Color GetRenderColor() const override;

		bool TryGetCharactor(
			float u,
			float v,
			float brightness,
			char& outCharacter
		) const override;


	private:
		Mesh mesh;
		std::shared_ptr<TransformComponent> transform;
		Color color;
	};
}


