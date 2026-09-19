#include "MeshFactory.h"
#include "Math/MathConstants.h"
#include <cmath>
#include <cassert>


namespace Hiwoong
{
    Mesh MeshFactory::CreateSphere(
        float radius, //구의 반지름 
        int sliceCount,//원의 둘레를 나누는 개수
        int stackCount) //구를 위에서 아래까지 나누는 갯수 
    {
        assert(radius > 0.0f);
        assert(sliceCount >= 3);// 원 둘레를 구성하려면 최소 3개로 나눈다.
        assert(stackCount >= 2); // 양 끝과 중간 둘레가 있어야 한다.

        Mesh result;

        // 구의 한쪽 끝부터 반대쪽 끝까지, 높이별로 원을 만든다.
        for (int stack = 0; stack <= stackCount; ++stack)
        {
            // 세로 각도: 0 ~ Pi(180도).
            // 시작 끝점 → 가운데 → 반대쪽 끝점 순서로 이동한다.
            const float verticalAngle =
                MathConstants::Pi *
                static_cast<float>(stack) /
                static_cast<float>(stackCount);

            // 현재 높이에서 원의 반지름.
            // 양 끝에서는 약 0, 가운데에서는 구의 반지름과 같다.
            const float ringRadius = radius * std::sin(verticalAngle);
            // 현재 원의 높이: +radius → 0 → -radius.
            const float y = radius * std::cos(verticalAngle);

            // 현재 원의 둘레에 정점들을 배치한다.
            for (int slice = 0; slice <= sliceCount; ++slice)
            {
                // 가로 각도: 0 ~ 2Pi(360도).
               // 원의 둘레를 한 바퀴 돌며 정점 위치를 구한다.
                const float horizontalAngle =
                    2.0f * MathConstants::Pi *
                    static_cast<float>(slice) /
                    static_cast<float>(sliceCount);

                // 같은 높이(y)에서 원 둘레의 x, z 좌표를 계산한다.
                const float x = ringRadius * std::cos(horizontalAngle);
                const float z = ringRadius * std::sin(horizontalAngle);


                // 계산한 위치를 메쉬의 정점 목록에 추가한다.
           // 마지막 정점은 시작점과 거의 같은 위치로,
           // 이후 둘레를 삼각형으로 연결하기 편하게 중복 생성한다.
                result.vertices.push_back(
                    Vertex{ Vector3(x, y, z) }
                );
            }
        }

        // 한 줄의 정점 개수.
// 둘레의 시작점과 끝점을 중복 생성했으므로 +1.
        const std::size_t rowSize =
            static_cast<std::size_t>(sliceCount) + 1;

        for (int stack = 0; stack < stackCount; ++stack)
        {
            for (int slice = 0; slice < sliceCount; ++slice)
            {
                // 이웃한 두 줄에서 사각형을 이루는 정점 네 개를 찾는다.
                const std::size_t topLeft =
                    static_cast<std::size_t>(stack) * rowSize +
                    static_cast<std::size_t>(slice);

                const std::size_t topRight = topLeft + 1;
                const std::size_t bottomLeft = topLeft + rowSize;
                const std::size_t bottomRight = bottomLeft + 1;

                // 사각형의 첫 번째 삼각형.
                // 구의 시작 끝점에서는 위쪽 정점들이 겹치므로 생략한다.
                if (stack > 0)
                {
                    result.triangles.push_back(
                        Triangle{ topLeft, topRight, bottomLeft }
                    );
                }

                // 사각형의 두 번째 삼각형.
                // 구의 반대쪽 끝점에서는 아래쪽 정점들이 겹치므로 생략한다.
                if (stack < stackCount - 1)
                {
                    result.triangles.push_back(
                        Triangle{ topRight, bottomRight, bottomLeft }
                    );
                }
            }
        }

        // 현재는 정점만 들어 있다.
   // 면을 그리려면 이후 삼각형 연결 정보를 추가해야 한다.
        return result;
    }

    Mesh MeshFactory::CreateQuad(float width, float height)
    {
        assert(width > 0.0f);
        assert(height > 0.0f);

        const float halfWidth = width * 0.5f;
        const float halfHeight = height * 0.5f;

        Mesh result;

        result.vertices =
        {
            //위치
            Vertex{Vector3(-halfWidth,-halfHeight,0.0f),0.0f,0.0f}, //왼위 1사분면
            Vertex{Vector3(halfWidth,-halfHeight,0.0f),1.0f,0.0f}, //오위 2사분면
            Vertex{Vector3(halfWidth,halfHeight,0.0f),1.0f,1.0f}, //오아래 4사분면
            Vertex{Vector3(-halfWidth,halfHeight,0.0f),0.0f,1.0f}, //왼아래 3사분면
        };

        result.triangles =
        {
           Triangle{0,2,1},
           Triangle{0,3,2}
        };
        return result;
    }


    Mesh MeshFactory::CreateCube(float size)
    {
        assert(size > 0.0f);

        const float halfSize = size * 0.5f;

        Mesh result;

        result.vertices =
        {
            Vertex{ Vector3(-halfSize, -halfSize, -halfSize) }, // 0
            Vertex{ Vector3(halfSize, -halfSize, -halfSize) }, // 1
            Vertex{ Vector3(halfSize,  halfSize, -halfSize) }, // 2
            Vertex{ Vector3(-halfSize,  halfSize, -halfSize) }, // 3

            Vertex{ Vector3(-halfSize, -halfSize,  halfSize) }, // 4
            Vertex{ Vector3(halfSize, -halfSize,  halfSize) }, // 5
            Vertex{ Vector3(halfSize,  halfSize,  halfSize) }, // 6
            Vertex{ Vector3(-halfSize,  halfSize,  halfSize) }  // 7
        };

        result.edges =
        {
            // Near face
            Edge{ 0, 1 },
            Edge{ 1, 2 },
            Edge{ 2, 3 },
            Edge{ 3, 0 },

            // Far face
            Edge{ 4, 5 },
            Edge{ 5, 6 },
            Edge{ 6, 7 },
            Edge{ 7, 4 },

            // Connect near and far faces
            Edge{ 0, 4 },
            Edge{ 1, 5 },
            Edge{ 2, 6 },
            Edge{ 3, 7 }
        };

        result.triangles =
        {
            // Front
            Triangle{ 0, 2, 1 },
            Triangle{ 0, 3, 2 },

            // Back
            Triangle{ 4, 5, 6 },
            Triangle{ 4, 6, 7 },

            // Left
            Triangle{ 0, 4, 7 },
            Triangle{ 0, 7, 3 },

            // Right
            Triangle{ 1, 2, 6 },
            Triangle{ 1, 6, 5 },

            // Top
            Triangle{ 0, 1, 5 },
            Triangle{ 0, 5, 4 },

            // Bottom
            Triangle{ 3, 7, 6 },
            Triangle{ 3, 6, 2 }
        };


        return result;
    }
}
