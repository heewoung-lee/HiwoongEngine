#include "SpriteRenderer3DComponent.h"
#include "GameObject/GameObject.h"
#include "Math/Vector4.h"
#include "Render/SoftwareRasterizer.h"
#include "Render/MeshFactory.h"
#include <cassert>
#include <algorithm>
#include <cmath>
namespace Hiwoong
{
	void SpriteRenderer3DComponent::Start()
	{
		super::Start();

		transform = GetComponent<TransformComponent>();
		assert(transform != nullptr);

		const float imageWidth = static_cast<float>(GetMaxWidth());
		const float imageHeight = static_cast<float>(GetImage().size());

		assert(imageWidth > 0.0f);
		assert(imageHeight > 0.0f);
		//이미지가 찌그러지는것을 막기 위해, 비율 계산,
		//그러면 이후에 비율이 적용된 메시 크기 x scale을 통해 크기를 조절 할 수 있음.
		const float aspectRatio = imageWidth / imageHeight;

		mesh = MeshFactory::CreateQuad(aspectRatio, 1.0f);
	}
	void SpriteRenderer3DComponent::Draw()
	{
	}



	const Mesh& SpriteRenderer3DComponent::GetMesh() const
	{
		return mesh;
	}
	//카메라를 향하도록 모델행렬 리턴
	Matrix4x4 SpriteRenderer3DComponent::GetModelMatrix(const RenderView& renderView) const
	{
		assert(transform != nullptr);

		
		const Vector3 worldPosition = transform->GetWorldPosition();

		//카메라의 월즈 좌표.
		//이거 굳이 Vector4(0.0f, 0.0f, 0.0f, 1.0f)를 곱해서 Vector3를 뽑는 이유는
		//renderView.cameraToWorld는 private 4x4행렬멤버여서 . x,y,z 절편을 뽑을 수 없음.
		//그래서 기본크기를 곱해준 다음에 x,y,z를 뽑음

		const Vector4 cameraWorldPos4 = renderView.cameraToWorld * Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		

		const Vector3 cameraWorldPosition(
			cameraWorldPos4.x,
			cameraWorldPos4.y,
			cameraWorldPos4.z
		);

		const Vector3 cameraSprite = worldPosition - cameraWorldPosition;

		//회전각도 계산 x와 z의 방향을 보고 y축의 회전각도를 구한다.
		const float yaw = std::atan2(cameraSprite.x, cameraSprite.z);

		//최종 모델 행렬 계산
		return 
			Matrix4x4::Translation(worldPosition) *
			Matrix4x4::RotationY(yaw) *
			Matrix4x4::Scale(transform->GetScale());
	}
	Color SpriteRenderer3DComponent::GetRenderColor() const
	{
		return GetColor();
	}
	bool SpriteRenderer3DComponent::TryGetCharactor(float u, float v, float brightness, char& outCharacter) const
	{
		if (isVisible == false) return false;

		if (image.empty() == true) return false;

		const int width = GetMaxWidth();
		if (width <= 0) return false;
		
		//레스터라이징 할때 부동소수점 오차로 0아래, 1위로 나올 수 있다고
		//AI가 추천해줌. 솔직히 이거 넣는다고 문제 생길 것도 아니라서 그냥 넣겠음.
		u = std::clamp(u, 0.0f, 1.0f);
		v = std::clamp(v, 0.0f, 1.0f);

		//x: UV를 이미지의 가로 문자 위치로 변환
		const std::size_t x = (std::min)(
			static_cast<std::size_t>(u * width),
			static_cast<std::size_t>(width - 1)
			);

		//y: UV를 이미지의 세로 줄 위치로 변환
		const std::size_t y = (std::min)(
			static_cast<std::size_t>(v * image.size()),
			image.size() - 1
			);

		const std::string& row = image[y];

		//x가 범위를 벗어나거나 빈문자면 false반환.
		if (x >= row.size() || row[x] == ' ') return false;

		//해당 픽셀에 정보가 있다면, 반환
		outCharacter = row[x];
		return true;
	}
}