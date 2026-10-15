#if defined(_DEBUG)

#include "ColliderDebugRenderer.h"
#include "Render/MeshFactory.h"
#include "GameObject/GameObject.h"
#include "Component/BoxCollider3DComponent.h"
#include "Math/Vector4.h"
#include "Render/RenderView.h"
#include "Render/SoftwareRasterizer.h"
#include "Render/Renderer.h"
#include <cmath>
namespace Hiwoong
{
    void ColliderDebugRenderer::Render(
        const std::vector<std::shared_ptr<GameObject>>& gameObjects,
        const RenderView& renderView
    ) const
    {

        if (!isVisible)
            return;
        
        for (const auto& gameObject : gameObjects)
        {
            if (gameObject == nullptr || !gameObject->IsActive())
                continue;

            const auto collider =
                gameObject->GetComponent<BoxCollider3DComponent>();

            if (collider == nullptr)
                continue;

            const Mesh mesh = CreateColliderMesh(
                *collider,
                gameObject->GetWorldPosition()
            );
            //일반 콜라이어는 초록색
            //트리거는 우선 빨간색으로 표현
            Color color = Color::Green;

            if (collider->IsTrigger())
                color = Color::Red;

            for (const Edge& edge : mesh.edges)
            {
                DrawWorldLine(
                    mesh.vertices[edge.startIndex].position,
                    mesh.vertices[edge.endIndex].position,
                    renderView,
                    color
                );
            }
        }
    }

    Mesh ColliderDebugRenderer::CreateColliderMesh(
        const BoxCollider3DComponent& collider,
        const Vector3& worldPosition
    ) const
    {
        Mesh mesh = MeshFactory::CreateCube(2.0f);
        const Vector3& halfExtents = collider.GetHalfExtents();

        for (Vertex& vertex : mesh.vertices)
        {
            vertex.position = Vector3(
                worldPosition.x + vertex.position.x * halfExtents.x,
                worldPosition.y + vertex.position.y * halfExtents.y,
                worldPosition.z + vertex.position.z * halfExtents.z
            );
        }

        return mesh;
    }
    void ColliderDebugRenderer::ToggleVisibility()
    {
        isVisible = !isVisible;
    }
    bool ColliderDebugRenderer::ClipLineNearPlane(
        Vector3& start,
        Vector3& end,
        float nearPlane
    ) const
    {
        if (start.z < nearPlane && end.z < nearPlane)
            return false;

        if (start.z >= nearPlane && end.z >= nearPlane)
            return true;

        const float ratio =
            (nearPlane - start.z) / (end.z - start.z);

        const Vector3 intersection =
            start + (end - start) * ratio;

        if (start.z < nearPlane)
            start = intersection;
        else
            end = intersection;

        return true;
    }
    Vector2 ColliderDebugRenderer::ProjectToScreen(
        const Vector3& cameraPosition,
        const RenderView& renderView
    ) const
    {
        const Vector4 clipPosition =
            renderView.projection * Vector4(
                cameraPosition.x,
                cameraPosition.y,
                cameraPosition.z,
                1.0f
            );

        const Vector3 ndcPosition =
            clipPosition.PerspectiveDivide();

        return SoftwareRasterizer::NdcToScreen(
            ndcPosition,
            renderView.screenSize.x,
            renderView.screenSize.y
        );
    }
    void ColliderDebugRenderer::DrawWorldLine(
        const Vector3& start,
        const Vector3& end,
        const RenderView& renderView,
        Color color
    ) const
    {
        const Vector4 start4 =
            renderView.view * Vector4(start.x, start.y, start.z, 1.0f);

        const Vector4 end4 =
            renderView.view * Vector4(end.x, end.y, end.z, 1.0f);

        Vector3 cameraStart(start4.x, start4.y, start4.z);
        Vector3 cameraEnd(end4.x, end4.y, end4.z);

        if (!ClipLineNearPlane(
            cameraStart, cameraEnd, renderView.nearPlane))
        {
            return;
        }

        Vector2 screenStart = ProjectToScreen(cameraStart, renderView);
        Vector2 screenEnd = ProjectToScreen(cameraEnd, renderView);

        if (!ClipLineScreen(screenStart, screenEnd, renderView.screenSize))
            return;

        Renderer::Get().SubmitLine(
            screenStart,
            screenEnd,
            '+',
            color,
            100
        );
    }
    bool ColliderDebugRenderer::ClipScreenBoundary(
        float direction,
        float distance,
        float& startRatio,
        float& endRatio
    ) const
    {
        if (direction == 0.0f)
            return distance >= 0.0f;

        const float ratio = distance / direction;

        if (direction < 0.0f)
        {
            if (ratio > startRatio)
                startRatio = ratio;
        }
        else
        {
            if (ratio < endRatio)
                endRatio = ratio;
        }

        return startRatio <= endRatio;
    }
    bool ColliderDebugRenderer::ClipLineScreen(
        Vector2& start,
        Vector2& end,
        const Vector2& screenSize
    ) const
    {
        const float startX = static_cast<float>(start.x);
        const float startY = static_cast<float>(start.y);
        const float deltaX = static_cast<float>(end.x) - startX;
        const float deltaY = static_cast<float>(end.y) - startY;

        const float right = static_cast<float>(screenSize.x - 1);
        const float bottom = static_cast<float>(screenSize.y - 1);

        float startRatio = 0.0f;
        float endRatio = 1.0f;

        if (!ClipScreenBoundary(-deltaX, startX, startRatio, endRatio) ||
            !ClipScreenBoundary(deltaX, right - startX, startRatio, endRatio) ||
            !ClipScreenBoundary(-deltaY, startY, startRatio, endRatio) ||
            !ClipScreenBoundary(deltaY, bottom - startY, startRatio, endRatio))
        {
            return false;
        }

        start = Vector2(
            static_cast<int>(std::round(startX + deltaX * startRatio)),
            static_cast<int>(std::round(startY + deltaY * startRatio))
        );

        end = Vector2(
            static_cast<int>(std::round(startX + deltaX * endRatio)),
            static_cast<int>(std::round(startY + deltaY * endRatio))
        );

        return true;
    }
}

#endif