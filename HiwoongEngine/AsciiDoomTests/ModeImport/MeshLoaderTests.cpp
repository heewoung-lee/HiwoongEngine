#include "ModelImport/IModelSource.h"
#include "../TestRunner.h"
#include "ModelImport/MeshLoader.h"
#include "ModelImport/UfbxModelSourceReader.h"
#include "ModelImport/ModelImporter.h"
#include "ModelImport/PngTextureLoader.h"
#include <memory>
#include <iostream>

namespace Hiwoong::Tests
{
    class FakeTriangleSource final : public IModelSource
    {
    public:

        std::size_t GetCornerSourceVertexIndex(
            std::size_t meshIndex,
            std::size_t cornerIndex
        ) const override
        {
            return cornerIndex;
        }

        ModelVertex GetCornerVertex(
            std::size_t meshIndex,
            std::size_t cornerIndex
        ) const override
        {
            ModelVertex vertex = GetVertex(meshIndex, cornerIndex);

            if (cornerIndex == 1)
            {
                vertex.u = 1.0f;
            }

            if (cornerIndex == 2)
            {
                vertex.v = 1.0f;
            }

            return vertex;
        }

        std::size_t GetCornerCount(
            std::size_t meshIndex
        ) const override
        {
            return 3;
        }

        std::vector<ModelTriangle> TriangulateFace(
            std::size_t meshIndex,
            std::size_t faceIndex
        ) const override
        {
            return { ModelTriangle{ 0, 1, 2 } };
        }

        std::size_t GetMeshCount() const override
        {
            return 1;
        }

        std::string GetMeshName(std::size_t meshIndex) const override
        {
            return "Triangle";
        }

        ModelVertex GetVertex(
            std::size_t meshIndex,
            std::size_t vertexIndex
        ) const override
        {
            return vertices.at(vertexIndex);
        }

        std::size_t GetFaceCount(std::size_t meshIndex) const override
        {
            return 1;
        }

        std::vector<std::string> GetDiffuseTexturePaths(
            std::size_t meshIndex
        ) const override
        {
            return { "triangle-diffuse.png", "" };
        }

    private:
        std::vector<ModelVertex> vertices =
        {
            { 0.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f }
        };
    };

    void RunMeshLoaderTests(TestRunner& testRunner)
    {
        ModelImportContext context;
        context.source = std::make_shared<FakeTriangleSource>();

        const MeshLoader loader;
        const MeshLoadResult result = loader.Load(context);

        testRunner.Check(
            result.success &&
            result.meshes.size() == 1 &&
            result.meshes[0].name == "Triangle" &&
            result.meshes[0].vertices.size() == 3 &&
            result.meshes[0].triangles.size() == 1,
            "MeshLoader converts a triangle"
        );


        if (!result.success ||
            result.meshes.size() != 1 ||
            result.meshes[0].vertices.size() != 3 ||
            result.meshes[0].triangles.size() != 1)
        {
            return;
        }

        const ModelMesh& mesh = result.meshes[0];

        testRunner.Check(
            mesh.diffuseTexturePaths ==
            std::vector<std::string>{ "triangle-diffuse.png", "" },
            "MeshLoader preserves material texture slots"
        );

        testRunner.Check(
            mesh.vertices[0].u == 0.0f &&
            mesh.vertices[0].v == 0.0f &&

            mesh.vertices[1].u == 1.0f &&
            mesh.vertices[1].v == 0.0f &&

            mesh.vertices[2].u == 0.0f &&
            mesh.vertices[2].v == 1.0f,
            "MeshLoader preserves corner UVs"
        );

        testRunner.Check(
            mesh.vertices[0].x == 0.0f &&
            mesh.vertices[0].y == 0.0f &&
            mesh.vertices[0].z == 0.0f &&

            mesh.vertices[1].x == 1.0f &&
            mesh.vertices[1].y == 0.0f &&
            mesh.vertices[1].z == 0.0f &&

            mesh.vertices[2].x == 0.0f &&
            mesh.vertices[2].y == 1.0f &&
            mesh.vertices[2].z == 0.0f &&

            mesh.triangles[0].index0 == 0 &&
            mesh.triangles[0].index1 == 1 &&
            mesh.triangles[0].index2 == 2 &&

            result.sourceMappings.size() == 1 &&
            result.sourceMappings[0].sourceMeshIndex == 0 &&
            result.sourceMappings[0].sourceVertexIndices ==
            std::vector<std::size_t>{ 0, 1, 2 },

            "MeshLoader preserves triangle data"
        );

        const UfbxModelSourceReader reader;
        const ModelSourceReadResult readResult =
            reader.Read("missing-model-for-test.fbx");

        testRunner.Check(
            readResult.success == false &&
            readResult.source == nullptr &&
            readResult.errorMessage.empty() == false,
            "ModelSourceReader rejects missing file"
        );
        const ModelSourceReadResult monsterResult = reader.Read(
            "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterLow.fbx"
        );

        testRunner.Check(
            monsterResult.success &&
            monsterResult.source != nullptr &&
            monsterResult.source->GetMeshCount() > 0,
            "ModelSourceReader reads monster FBX"
        );

        if (!monsterResult.success || monsterResult.source == nullptr)
            return;

        ModelImportContext monsterContext;
        monsterContext.source = monsterResult.source;

        const MeshLoadResult monsterMeshResult =
            loader.Load(monsterContext);

        testRunner.Check(
            monsterMeshResult.success &&
            !monsterMeshResult.meshes.empty() &&
            !monsterMeshResult.meshes[0].vertices.empty() &&
            !monsterMeshResult.meshes[0].triangles.empty(),
            "MeshLoader converts monster FBX"
        );

        if (!monsterMeshResult.success)
        {
            std::cout << monsterMeshResult.errorMessage << std::endl;
        }
        //10.9 다각형이 있음 삼각형만 있는게 아니라서 삼각형으로 조각조각 쪼개야함.


        for (const ModelMesh& modelMesh : monsterMeshResult.meshes)
        {
            std::cout
                << "Mesh: " << modelMesh.name
                << " / Vertices: " << modelMesh.vertices.size()
                << " / Triangles: " << modelMesh.triangles.size()
                << std::endl;

            for (std::size_t i = 0;
                i < modelMesh.diffuseTexturePaths.size();
                ++i)
            {
                std::cout
                    << "Material: " << i
                    << " / Texture: " << modelMesh.diffuseTexturePaths[i]
                    << std::endl;

                ModelTexture texture;

                const bool loaded = LoadPngTexture(
                    modelMesh.diffuseTexturePaths[i],
                    texture
                );

                testRunner.Check(
                    loaded &&
                    texture.width > 0 &&
                    texture.height > 0 &&
                    texture.rgbPixels.size() == texture.width * texture.height * 3,
                    "PngTextureLoader reads RGB pixels"
                );
            }

        }

        const ModelImporter importer(
            std::make_shared<UfbxModelSourceReader>(),
            std::make_shared<MeshLoader>()
        );



        const ModelImportResult importResult = importer.Import(
            "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterLow.fbx"
        );

        testRunner.Check(
            importResult.success &&
            !importResult.model.meshes.empty() &&
            !importResult.model.meshes[0].vertices.empty() &&
            !importResult.model.meshes[0].triangles.empty() &&
            importResult.model.meshes[0].diffuseTextures.size() == 2 &&
            importResult.model.meshes[0].diffuseTextures[0] != nullptr &&
            importResult.model.meshes[0].diffuseTextures[1] != nullptr,
            "ModelImporter imports monster FBX"
        );

    }

}