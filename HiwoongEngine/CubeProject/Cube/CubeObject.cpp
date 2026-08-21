#include "CubeObject.h"
#include "Render/MeshFactory.h"
#include "Math/Matrix4x4.h"
#include "Render/SoftwareRasterizer.h"
#include "Render/Renderer.h"
#include "Scene/Scene.h"
#include "Component/Component.h"
#include "Cube/CubeRotationComponent.h"
#include <algorithm>
#include <cassert>

namespace Hiwoong
{
	CubeObject::CubeObject() : mesh(MeshFactory::CreateCube(2.0f))
	{

	}

	void CubeObject::Draw()
	{
		super::Draw();

        auto rotationComp = rotationComponent.lock();
        assert(rotationComp != nullptr);

        const Vector3& rotation = rotationComp->GetRotation();


        const Matrix4x4 rotationMatrix =
            Matrix4x4::RotationY(rotation.y) *
            Matrix4x4::RotationX(rotation.x);
        
        const Matrix4x4 model =
            Matrix4x4::Translation(Vector3(0, 0, 5)) *
            rotationMatrix;

        const Vector3 lightDirection =
            Vector3(-1, -1, -1).Normalized();


        const Matrix4x4 view =
            Matrix4x4::LookAt(
                Vector3(0, 0, 0),
                Vector3(0, 0, 1),
                Vector3(0, -1, 0)
            );

        const Matrix4x4 projection =
            Matrix4x4::Perspective(
                3.14159265f / 3.0f,
                180.0f / 110.0f,
                0.1f,
                100.0f
            );

        const Matrix4x4 mvp =
            projection * view * model;

        constexpr float halfSize = 1.0f;
        constexpr float sampleStep = 0.03f;

        const char frontCharacter =
            GetFaceCharacter(Vector3(0, 0, -1), rotationMatrix, lightDirection);

        const char rightCharacter =
            GetFaceCharacter(Vector3(1, 0, 0), rotationMatrix, lightDirection);

        const char topCharacter =
            GetFaceCharacter(Vector3(0, -1, 0), rotationMatrix, lightDirection);

        const char backCharacter =
            GetFaceCharacter(Vector3(0, 0, 1), rotationMatrix, lightDirection);

        const char leftCharacter =
            GetFaceCharacter(Vector3(-1, 0, 0), rotationMatrix, lightDirection);

        const char bottomCharacter =
            GetFaceCharacter(Vector3(0, 1, 0), rotationMatrix, lightDirection);

        // 카메라와 가까운 큐브의 앞면
        for (float y = -halfSize; y <= halfSize; y += sampleStep)
        {
            for (float x = -halfSize; x <= halfSize; x += sampleStep)
            {
                SubmitSurfacePoint(
                    Vector3(x, y, -halfSize),
                    mvp,
                    frontCharacter
                );
            }
        }
        // 큐브의 오른쪽 면
        for (float y = -halfSize; y <= halfSize; y += sampleStep)
        {
            for (float z = -halfSize; z <= halfSize; z += sampleStep)
            {
                SubmitSurfacePoint(
                    Vector3(halfSize, y, z),
                    mvp,
                    rightCharacter
                );
            }
        }
        // 큐브의 윗면
        for (float z = -halfSize; z <= halfSize; z += sampleStep)
        {
            for (float x = -halfSize; x <= halfSize; x += sampleStep)
            {
                SubmitSurfacePoint(
                    Vector3(x, -halfSize, z),
                    mvp,
                    topCharacter
                );
            }
        }

        // 큐브의 뒷면
        for (float y = -halfSize; y <= halfSize; y += sampleStep)
        {
            for (float x = -halfSize; x <= halfSize; x += sampleStep)
            {
                SubmitSurfacePoint(
                    Vector3(x, y, halfSize),
                    mvp,
                    backCharacter
                );
            }
        }
        // 큐브의 왼쪽 면
        for (float y = -halfSize; y <= halfSize; y += sampleStep)
        {
            for (float z = -halfSize; z <= halfSize; z += sampleStep)
            {
                SubmitSurfacePoint(
                    Vector3(-halfSize, y, z),
                    mvp,
                    leftCharacter
                );
            }
        }

        // 큐브의 아랫면
        for (float z = -halfSize; z <= halfSize; z += sampleStep)
        {
            for (float x = -halfSize; x <= halfSize; x += sampleStep)
            {
                SubmitSurfacePoint(
                    Vector3(x, halfSize, z),
                    mvp,
                    bottomCharacter
                );
            }
        }
 //       std::vector<Vector2> screenPositions;
 //       screenPositions.reserve(mesh.vertices.size());

 //       for (const Vertex& vertex : mesh.vertices)
 //       {
 //           const Vector4 localPosition(
 //               vertex.position.x,
 //               vertex.position.y,
 //               vertex.position.z,
 //               1.0f
 //           );

 //           const Vector4 clipPosition =
 //               mvp * localPosition;

 //           const Vector3 ndcPosition =
 //               clipPosition.PerspectiveDivide();

 //           const Vector2 screenPosition =
 //               SoftwareRasterizer::NdcToScreen(
 //                   ndcPosition,
 //                   40,
 //                   20
 //               );

 //           screenPositions.emplace_back(screenPosition);

 //           Renderer::Get().SubmitPoint3D(
 //               screenPosition,
 //               ndcPosition.z,
 //               '*',
 //               Color::Yellow,
 //               1
 //           );
 //       }


 //       for (const Edge& edge : mesh.edges)
 //       {
 //           const Vector2& start =
 //               screenPositions[edge.startIndex];

 //           const Vector2& end =
 //               screenPositions[edge.endIndex];

 //           Renderer::Get().SubmitLine(
 //               start,
 //               end,
 //               '@',
 //               Color::White,
 //               0
 //           );
 //       }

	}

    void CubeObject::Start()
    {
        super::Start();
        rotationComponent = AddComponent<CubeRotationComponent>();
    }

    char CubeObject::GetFaceCharacter(
        const Vector3& localNormal,
        const Matrix4x4& rotationMatrix,
        const Vector3& lightDirection) const
    {
        const Vector4 rotatedNormal =
            rotationMatrix * Vector4(
                localNormal.x,
                localNormal.y,
                localNormal.z,
                0.0f
            );

        const Vector3 worldNormal(
            rotatedNormal.x,
            rotatedNormal.y,
            rotatedNormal.z
        );

        const float diffuse = std::clamp(
            Vector3::Dot(worldNormal.Normalized(), lightDirection),
            0.0f,
            1.0f
        );

        return GetShadeCharacter(
            0.2f + diffuse * 0.8f
        );
    }

    void CubeObject::SubmitSurfacePoint(
        const Vector3& localPosition,
        const Matrix4x4& mvp,
        char character)
    {
        const Vector4 position(
            localPosition.x,
            localPosition.y,
            localPosition.z,
            1.0f
        );

        const Vector4 clipPosition = mvp * position;

        // 카메라 뒤쪽의 점은 그리지 않는다.
        if (clipPosition.w <= 0.0f)
        {
            return;
        }

        const Vector3 ndcPosition =
            clipPosition.PerspectiveDivide();

        std::shared_ptr<Scene> scene = GetOwner();
        assert(scene != nullptr);

        const Vector2 screenSize = scene->GetScreenSize();

        Vector2 screenPosition =
            SoftwareRasterizer::NdcToScreen(
                ndcPosition,
                screenSize.x,
                screenSize.y
            );
        screenPosition.y -= 15;

        Renderer::Get().SubmitPoint3D(
            screenPosition,
            ndcPosition.z,
            character,
            Color::White,
            0
        );

    }

    char CubeObject::GetShadeCharacter(float brightness) const
    {
        static constexpr char shades[] = " .:-=+*#%@";

        brightness = std::clamp(brightness, 0.0f, 1.0f);

        constexpr std::size_t lastIndex =
            sizeof(shades) - 2;

        const std::size_t index =
            static_cast<std::size_t>(brightness * lastIndex);

        return shades[index];
    }

}

