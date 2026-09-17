#include "Bullet.h"
#include "Component/BoxCollider3DComponent.h"
#include "Math/Vector3.h"
#include "Component/MeshRenderComponent.h"
#include "Render/MeshFactory.h"
#include <cmath>
#include <cassert>
namespace Hiwoong
{
    Bullet::Bullet(
        const Vector3& firePosition,
        const Vector3& fireDirection,
        float size,
        float speed,
        float spawnDistance)
        : speed(speed),
        firePosition(firePosition),
        fireDirection(fireDirection),
        spawnDistance(spawnDistance)
    {
        assert(size > 0.0f);
        assert(speed > 0.0f);
        assert(spawnDistance >= 0.0f);
        assert(fireDirection.Length() > 0.0f);

        size_width = size;
        size_height = size;
        size_depth = size;

        // 전달받은 방향의 길이를 1로 맞춰 보관한다.
        this->fireDirection = fireDirection.Normalized();
    }
    void Bullet::Start()
    {
        super::Start();

        // 발사 위치에서 진행 방향으로 조금 앞에 배치한다.
        transform->SetWorldPosition(
            firePosition + fireDirection * spawnDistance
        );

        // 발사 방향을 탄환의 회전각으로 변환한다.
        const float horizontalLength = std::sqrt(
            fireDirection.x * fireDirection.x +
            fireDirection.z * fireDirection.z
        );

        const float pitch = std::atan2(
            -fireDirection.y,
            horizontalLength
        );

        const float yaw = std::atan2(
            fireDirection.x,
            fireDirection.z
        );

        // 기존 Update()가 이 전방 방향으로 이동한다.
        transform->SetRotation(Vector3(pitch, yaw, 0.0f));

        collider = AddComponent<BoxCollider3DComponent>(
            Vector3(
                size_width / 2,
                size_height / 2,
                size_depth / 2
            )
        );

        // 기존 탄환의 너비를 지름으로 사용한다.
        const float radius = size_width * 0.5f;

        // 작은 탄환용 구의 분할 개수.
        const int sliceCount = 12;
        const int stackCount = 6;

        AddComponent<MeshRenderComponent>(
            MeshFactory::CreateSphere(radius, sliceCount, stackCount),
            Color::Yellow
        );

    }

    void Bullet::Update(double deltaTime)
    {
        super::Update(deltaTime);

        const Vector3 movement =
            transform->GetForward() *
            speed *
            static_cast<float>(deltaTime);

        transform->SetWorldPosition(
            transform->GetWorldPosition() + movement
        );
    }

}
