#include "MeshFactory.h"
#include <cassert>


namespace Hiwoong
{
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
