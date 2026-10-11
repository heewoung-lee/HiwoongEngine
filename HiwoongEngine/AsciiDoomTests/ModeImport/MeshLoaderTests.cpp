#include "ModelImport/IModelSource.h"
#include "../TestRunner.h"
#include "ModelImport/MeshLoader.h"
#include "ModelImport/UfbxModelSourceReader.h"
#include "ModelImport/ModelImporter.h"
#include "ModelImport/PngTextureLoader.h"
#include "ModelImport/SkeletonLoader.h"
#include "ModelImport/SkinWeightLoader.h"
#include "ModelImport/ModelMatrix.h"
#include "Animation/SkeletonPoseCalculator.h"
#include <cmath>
#include <memory>
#include <iostream>

namespace Hiwoong::Tests
{
	class FakeTriangleSource final : public IModelSource
	{
	public:
		std::vector<SkinBinding> GetSkinBindings(
			std::size_t meshIndex
		) const override
		{
			return {};
		}

		std::vector<std::vector<SkinWeight>> GetSkinWeights(
			std::size_t meshIndex
		) const override
		{
			std::vector<std::vector<SkinWeight>> weights(vertices.size());

			weights[0] = {
				{ 0, 1.0f },
				{ 1, 3.0f }
			};

			weights[1] = {
	{ 0, 1.0f },
	{ 1, 2.0f },
	{ 2, 3.0f },
	{ 3, 4.0f },
	{ 4, 5.0f }
			};

			return weights;
		}

		std::vector<SkeletonNode> GetSkeletonNodes() const override
		{
			return {};
		}
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
		SkeletonNode parent;
		parent.localPositionX = 10.0f;
		parent.localScaleX = 2.0f;

		// Z축으로 90도 회전하는 값.
		parent.localRotationZ = std::sqrt(0.5f);
		parent.localRotationW = std::sqrt(0.5f);

		SkeletonNode child;
		child.localPositionX = 2.0f;

		const ModelMatrix parentMatrix = ModelMatrix::FromNode(parent);
		const ModelMatrix childMatrix = ModelMatrix::FromNode(child);
		const ModelMatrix combinedMatrix = parentMatrix.Multiply(childMatrix);

		const ModelVertex finalPosition =
			combinedMatrix.TransformPoint({ 0.0f, 0.0f, 0.0f });

		testRunner.Check(
			std::abs(finalPosition.x - 10.0f) < 0.0001f &&
			std::abs(finalPosition.y - 4.0f) < 0.0001f &&
			std::abs(finalPosition.z) < 0.0001f,
			"ModelMatrix applies parent transform to child"
		);

		// 자식이 부모보다 먼저 있어도 모든 조상의 움직임을 반영한다.
		std::vector<SkeletonNode> hierarchyNodes(3);
		hierarchyNodes[0].parentIdx = 1;
		hierarchyNodes[0].localPositionX = 2.0f;

		hierarchyNodes[1].localPositionX = 10.0f;
		hierarchyNodes[1].localScaleX = 2.0f;
		hierarchyNodes[1].localRotationZ = std::sqrt(0.5f);
		hierarchyNodes[1].localRotationW = std::sqrt(0.5f);

		hierarchyNodes[2].parentIdx = 0;
		hierarchyNodes[2].localPositionX = 1.0f;

		const std::vector<ModelMatrix> hierarchyMatrices =
			SkeletonPoseCalculator::Calculate(hierarchyNodes);
		const std::vector<ModelVertex> expectedPositions = {
			{ 10.0f, 4.0f, 0.0f },
			{ 10.0f, 0.0f, 0.0f },
			{ 10.0f, 6.0f, 0.0f }
		};

		bool hasCorrectHierarchy =
			hierarchyMatrices.size() == expectedPositions.size();

		if (hasCorrectHierarchy)
		{
			for (std::size_t index = 0; index < hierarchyMatrices.size(); ++index)
			{
				const ModelVertex position =
					hierarchyMatrices[index].TransformPoint({ 0.0f, 0.0f, 0.0f });
				const ModelVertex& expected = expectedPositions[index];

				if (!(std::abs(position.x - expected.x) < 0.0001f &&
					std::abs(position.y - expected.y) < 0.0001f &&
					std::abs(position.z - expected.z) < 0.0001f))
				{
					hasCorrectHierarchy = false;
					break;
				}
			}
		}

		testRunner.Check(
			hasCorrectHierarchy,
			"SkeletonPoseCalculator applies all parents in source order"
		);

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

		//스켈레톤 부모 검사
		const std::vector<SkeletonNode> nodes =
			monsterResult.source->GetSkeletonNodes();
		bool hasCorrectParent = false;

		for (const SkeletonNode& node : nodes)
		{
			if (node.name == "mixamorig5:Spine")
			{
				hasCorrectParent =
					node.parentIdx >= 0 &&
					node.parentIdx < static_cast<int>(nodes.size()) &&
					nodes[node.parentIdx].name == "mixamorig5:Hips";

				break;
			}
		}

		testRunner.Check(
			hasCorrectParent,
			"ModelSourceReader preserves skeleton parent"
		);

		const auto weights = monsterResult.source->GetSkinWeights(0);

		bool hasPositiveWeight = false;
		bool hasValidNodes = true;

		for (const auto& vertexWeights : weights)
		{
			for (const SkinWeight& weight : vertexWeights)
			{
				if (weight.weight > 0.0f)
					hasPositiveWeight = true;

				if (weight.nodeIdx < 0 ||
					weight.nodeIdx >= static_cast<int>(nodes.size()))
				{
					hasValidNodes = false;
				}
			}
		}

		testRunner.Check(
			hasPositiveWeight && hasValidNodes,
			"ModelSourceReader reads skin weights"
		);


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
			std::make_shared<MeshLoader>(),
			std::make_shared<SkeletonLoader>(),
			std::make_shared<SkinWeightLoader>()
		);


		const ModelImportResult importResult = importer.Import(
			"C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterLow.fbx"
		);

		testRunner.Check(
			importResult.success &&
			importResult.model.skeltonNodes.size() == nodes.size() &&
			!importResult.model.meshes.empty() &&
			!importResult.model.meshes[0].vertices.empty() &&
			!importResult.model.meshes[0].triangles.empty() &&
			importResult.model.meshes[0].diffuseTextures.size() == 2 &&
			importResult.model.meshes[0].diffuseTextures[0] != nullptr &&
			importResult.model.meshes[0].diffuseTextures[1] != nullptr &&
			importResult.model.meshes[0].skinWeights.size() ==
			importResult.model.meshes[0].vertices.size() &&
			!importResult.model.meshes[0].skinWeights[0].empty() &&
			!importResult.model.meshes[0].skinBindings.empty(),
			"ModelImporter imports monster FBX"
		);




		monsterContext.meshMappings = monsterMeshResult.sourceMappings;
		auto skinMeshes = monsterMeshResult.meshes;

		const SkinWeightLoader skinLoader;
		const bool loadedSkin = skinLoader.Load(monsterContext, skinMeshes);

		testRunner.Check(
			loadedSkin &&
			!skinMeshes.empty() &&
			!skinMeshes[0].vertices.empty() &&
			skinMeshes[0].skinWeights.size() == skinMeshes[0].vertices.size() &&
			!skinMeshes[0].skinWeights[0].empty(),
			"SkinWeightLoader fills output vertex weights"
		);
		context.meshMappings = result.sourceMappings;
		auto normalizedMeshes = result.meshes;

		const bool normalized = skinLoader.Load(context, normalizedMeshes);

		bool hasFirstWeight = false;
		bool hasSecondWeight = false;

		if (normalized &&
			normalizedMeshes.size() == 1 &&
			normalizedMeshes[0].skinWeights.size() == 3)
		{
			for (const SkinWeight& weight : normalizedMeshes[0].skinWeights[0])
			{
				if (weight.nodeIdx == 0 && weight.weight == 0.25f)
					hasFirstWeight = true;

				if (weight.nodeIdx == 1 && weight.weight == 0.75f)
					hasSecondWeight = true;
			}
		}

		testRunner.Check(
			hasFirstWeight && hasSecondWeight,
			"SkinWeightLoader normalizes weights"
		);
		bool keepsLargestFour = false;

		if (normalized &&
			normalizedMeshes.size() == 1 &&
			normalizedMeshes[0].skinWeights.size() == 3)
		{
			const auto& vertexWeights = normalizedMeshes[0].skinWeights[1];

			float totalWeight = 0.0f;
			bool hasSmallestWeight = false;

			for (const SkinWeight& weight : vertexWeights)
			{
				totalWeight += weight.weight;

				if (weight.nodeIdx == 0)
					hasSmallestWeight = true;
			}

			keepsLargestFour =
				vertexWeights.size() == 4 &&
				!hasSmallestWeight &&
				std::abs(totalWeight - 1.0f) < 0.0001f;
		}

		testRunner.Check(
			keepsLargestFour,
			"SkinWeightLoader keeps largest four and normalizes"
		);

	}

}