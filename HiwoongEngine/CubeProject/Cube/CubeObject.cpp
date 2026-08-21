#include "CubeObject.h"
#include "Render/MeshFactory.h"
#include "Math/Matrix4x4.h"
#include "Render/SoftwareRasterizer.h"
#include "Render/Renderer.h"
namespace Hiwoong
{
	CubeObject::CubeObject() : mesh(MeshFactory::CreateCube(2.0f))
	{

	}

	void CubeObject::Draw()
	{
		super::Draw();

        const Matrix4x4 model =
            Matrix4x4::Translation(Vector3(0, 0, 5));

        const Matrix4x4 view =
            Matrix4x4::LookAt(
                Vector3(0, 0, 0),
                Vector3(0, 0, 1),
                Vector3(0, -1, 0)
            );

        const Matrix4x4 projection =
            Matrix4x4::Perspective(
                3.14159265f / 3.0f,
                40.0f / 20.0f,
                0.1f,
                100.0f
            );

        const Matrix4x4 mvp =
            projection * view * model;

        std::vector<Vector2> screenPositions;
        screenPositions.reserve(mesh.vertices.size());

        for (const Vertex& vertex : mesh.vertices)
        {
            const Vector4 localPosition(
                vertex.position.x,
                vertex.position.y,
                vertex.position.z,
                1.0f
            );

            const Vector4 clipPosition =
                mvp * localPosition;

            const Vector3 ndcPosition =
                clipPosition.PerspectiveDivide();

            screenPositions.emplace_back(
                SoftwareRasterizer::NdcToScreen(
                    ndcPosition,
                    40,
                    20
                )
            );
        }

        for (const Edge& edge : mesh.edges)
        {
            const Vector2& start =
                screenPositions[edge.startIndex];

            const Vector2& end =
                screenPositions[edge.endIndex];

            Renderer::Get().SubmitLine(
                start,
                end,
                '@',
                Color::White,
                0
            );
        }

	}

}

