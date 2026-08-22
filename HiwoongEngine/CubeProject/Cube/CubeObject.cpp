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

        std::vector<Vector2> screenPositions;
        std::vector<float> depths;

        screenPositions.reserve(mesh.vertices.size());
        depths.reserve(mesh.vertices.size());


        std::shared_ptr<Scene> scene = GetOwner();
        assert(scene != nullptr);

        const Vector2 screenSize = scene->GetScreenSize();

        for (const Vertex& vertex : mesh.vertices)
        {
            const Vector4 clipPosition =
                mvp * Vector4(
                    vertex.position.x,
                    vertex.position.y,
                    vertex.position.z,
                    1.0f
                );

            assert(clipPosition.w > 0.0f);

            const Vector3 ndcPosition =
                clipPosition.PerspectiveDivide();

            Vector2 screenPosition =
                SoftwareRasterizer::NdcToScreen(
                    ndcPosition,
                    screenSize.x,
                    screenSize.y
                );

            screenPosition.y -= 15;

            screenPositions.emplace_back(screenPosition);
            depths.emplace_back(ndcPosition.z);
        }



        for (const Triangle& triangle : mesh.triangles)
        {
            const Vector2& point0 = screenPositions[triangle.index0];
            const Vector2& point1 = screenPositions[triangle.index1];
            const Vector2& point2 = screenPositions[triangle.index2];


            const Vector3& local0 =
                mesh.vertices[triangle.index0].position;

            const Vector3& local1 =
                mesh.vertices[triangle.index1].position;

            const Vector3& local2 =
                mesh.vertices[triangle.index2].position;

            const Vector3 localNormal =
                Vector3::Cross(
                    local1 - local0,
                    local2 - local0
                ).Normalized();

            const char triangleCharacter =
                GetFaceCharacter(
                    localNormal,
                    rotationMatrix,
                    lightDirection
                );

            if (SoftwareRasterizer::IsBackFace(
                point0,
                point1,
                point2))
            {
                continue;
            }

            const std::vector<Vector2> pixels =
                SoftwareRasterizer::RasterizeTriangle(
                    point0,
                    point1,
                    point2
                );

           

            for (const Vector2& pixel : pixels)
            {

                const Vector3 weights =
                    SoftwareRasterizer::CalculateBarycentric(
                        pixel,
                        point0,
                        point1,
                        point2
                    );

                const float depth =
                    weights.x * depths[triangle.index0] +
                    weights.y * depths[triangle.index1] +
                    weights.z * depths[triangle.index2];

                Renderer::Get().SubmitPoint3D(
                    pixel,
                    depth,
                    triangleCharacter,
                    Color::White,
                    0
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

