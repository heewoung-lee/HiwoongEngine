#pragma once

#if defined(_DEBUG)
#include "Render/Mesh.h"
#include "Core/Core.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <memory>
#include <vector>
/// <summary>
/// 10.14일 콜라이어의 외곽선을 보기위한 디버깅 툴
/// </summary>
namespace Hiwoong
{
    class BoxCollider3DComponent;
    class GameObject;
    struct RenderView;

    class Hiwoong_API ColliderDebugRenderer
    {
    public:
        void Render(
            const std::vector<std::shared_ptr<GameObject>>& gameObjects,
            const RenderView& renderView
        ) const;
        void ToggleVisibility();
    private:
        Mesh CreateColliderMesh(
            const BoxCollider3DComponent& collider,
            const Vector3& worldPosition
        ) const;

        //카메라에 너무 가깝거나 뒤에 있는 선을 자르는 함수
        bool ClipLineNearPlane(
            Vector3& start,
            Vector3& end,
            float nearPlane
        ) const;

        Vector2 ProjectToScreen(
            const Vector3& cameraPosition,
            const RenderView& renderView
        ) const;

        void DrawWorldLine(
            const Vector3& start,
            const Vector3& end,
            const RenderView& renderView,
            Color color
        ) const;
        // 화면 밖을 넘어가는 선을 자르는 함수
        bool ClipScreenBoundary(
            float direction,
            float distance,
            float& startRatio,
            float& endRatio
        ) const;
        //상하좌우 경계 검사
        bool ClipLineScreen(
            Vector2& start,
            Vector2& end,
            const Vector2& screenSize
        ) const;

        bool isVisible = true;
    };
}

#endif