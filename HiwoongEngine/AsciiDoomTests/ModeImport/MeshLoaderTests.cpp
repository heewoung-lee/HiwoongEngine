#include "ModelImport/IModelSource.h"
#include "../TestRunner.h"
#include "ModelImport/MeshLoader.h"
#include "ModelImport/UfbxModelSourceReader.h"
#include "ModelImport/ModelImporter.h"
#include "ModelImport/PngTextureLoader.h"
#include "ModelImport/SkeletonLoader.h"
#include "ModelImport/SkinWeightLoader.h"
#include "ModelImport/ModelMatrix.h"
#include "ModelImport/AnimationLoader.h"
#include "Animation/SkeletonPoseCalculator.h"
#include "Animation/SkinningCalculator.h"
#include "ThirdParty/ufbx.h"
#include <cmath>
#include <memory>
#include <iostream>

namespace Hiwoong::Tests
{
	class FakeTriangleSource final : public IModelSource
	{
	public:
		std::vector<AnimationClip> GetAnimationClips() const override
		{
			return {};
		}

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

	// 원본을 해제한 뒤에도 복사한 프레임 데이터가 유지되는지 검사한다.
	void RunAnimationSourceTests(TestRunner& testRunner)
	{
		const std::vector<std::string> fileNames = {
			"MonsterIdle.fbx", "MonsterWalk.fbx"
		};
		const UfbxModelSourceReader reader;

		for (const std::string& fileName : fileNames)
		{
			const std::string filePath =
				"C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/" + fileName;
			const std::string readLabel = "AnimationSource reads clips from " + fileName;
			const std::string timeLabel = "AnimationSource preserves frame times in " + fileName;
			const std::string poseLabel = "AnimationSource preserves animated poses in " + fileName;

			ModelSourceReadResult sourceResult = reader.Read(filePath);
			if (!sourceResult.success || sourceResult.source == nullptr)
			{
				testRunner.Check(false, readLabel.c_str());
				continue;
			}

			const std::vector<AnimationClip> clips = sourceResult.source->GetAnimationClips();
			sourceResult.source.reset();

			// 별도 ufbx 조회로 클립 길이와 시간별 자세의 기대값을 구한다.
			ufbx_load_opts options{};
			options.target_axes = {
				UFBX_COORDINATE_AXIS_POSITIVE_X,
				UFBX_COORDINATE_AXIS_NEGATIVE_Y,
				UFBX_COORDINATE_AXIS_NEGATIVE_Z
			};
			options.target_unit_meters = 1.0;
			options.space_conversion = UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY;
			ufbx_error error{};
			ufbx_scene* referenceScene = ufbx_load_file(filePath.c_str(), &options, &error);
			if (referenceScene == nullptr)
			{
				testRunner.Check(false, readLabel.c_str());
				continue;
			}

			bool hasClips = !clips.empty() &&
				clips.size() == referenceScene->anim_stacks.count &&
				referenceScene->nodes.count > 0;
			if (hasClips)
			{
				for (std::size_t clipIndex = 0; clipIndex < clips.size(); ++clipIndex)
				{
					const AnimationClip& clip = clips[clipIndex];
					const ufbx_anim_stack* sourceClip = referenceScene->anim_stacks.data[clipIndex];
					if (!(clip.name == sourceClip->name.data &&
						std::isfinite(clip.duration) && clip.duration > 0.0 &&
						std::abs(clip.duration - (sourceClip->time_end - sourceClip->time_begin)) < 0.00000001 &&
						clip.frames.size() > 1 &&
						clip.frames.front().nodes.size() == referenceScene->nodes.count))
					{
						hasClips = false;
						break;
					}
				}
			}
			testRunner.Check(hasClips, readLabel.c_str());

			bool hasCorrectTimes = hasClips;
			bool matchesSourcePoses = hasClips;
			bool allClipsMove = hasClips;
			if (hasClips)
			{
				for (std::size_t clipIndex = 0; clipIndex < clips.size(); ++clipIndex)
				{
					const AnimationClip& clip = clips[clipIndex];
					const ufbx_anim_stack* sourceClip = referenceScene->anim_stacks.data[clipIndex];
					bool hasMovement = false;

					if (!(clip.frames.front().time == 0.0 &&
						std::abs(clip.frames.back().time - clip.duration) < 0.00000001))
						hasCorrectTimes = false;

					for (std::size_t frameIndex = 0; frameIndex < clip.frames.size(); ++frameIndex)
					{
						const AnimationFrame& frame = clip.frames[frameIndex];
						if (!(std::isfinite(frame.time) && frame.time >= 0.0 && frame.time <= clip.duration))
						{
							hasCorrectTimes = false;
							matchesSourcePoses = false;
							continue;
						}
						if (frameIndex > 0)
						{
							const double interval = frame.time - clip.frames[frameIndex - 1].time;
							// 마지막 간격은 1/30초보다 짧아도 된다.
							if (!(interval > 0.0 && interval <= 1.0 / 30.0 + 0.00000001) ||
								(frameIndex + 1 < clip.frames.size() &&
									std::abs(interval - 1.0 / 30.0) > 0.00000001))
								hasCorrectTimes = false;
						}

						if (frame.nodes.size() != referenceScene->nodes.count)
						{
							matchesSourcePoses = false;
							continue;
						}
						for (std::size_t nodeIndex = 0; nodeIndex < frame.nodes.size(); ++nodeIndex)
						{
							const SkeletonNode& node = frame.nodes[nodeIndex];
							const ufbx_node* referenceNode = referenceScene->nodes.data[nodeIndex];
							const int expectedParent = referenceNode->parent == nullptr ? -1 :
								static_cast<int>(referenceNode->parent->typed_id);
							if (node.name != referenceNode->name.data || node.parentIdx != expectedParent)
								matchesSourcePoses = false;

							const ufbx_transform expectedTransform = ufbx_evaluate_transform(
								sourceClip->anim, referenceNode, sourceClip->time_begin + frame.time);
							const ufbx_matrix expectedMatrix = ufbx_transform_to_matrix(&expectedTransform);
							const ModelMatrix actualMatrix = ModelMatrix::FromNode(node);
							const ModelMatrix firstMatrix = ModelMatrix::FromNode(
								clip.frames.front().nodes[nodeIndex]);

							// 회전 q와 -q는 같은 자세이므로 행렬로 비교한다.
							for (int row = 0; row < 3; ++row)
							{
								for (int column = 0; column < 4; ++column)
								{
									if (!(std::abs(actualMatrix.values[row][column] -
										expectedMatrix.cols[column].v[row]) <= 0.0001))
										matchesSourcePoses = false;
									if (std::abs(actualMatrix.values[row][column] -
										firstMatrix.values[row][column]) > 0.00001f)
										hasMovement = true;
								}
							}
						}
					}
					if (!hasMovement)
						allClipsMove = false;
				}
			}
			ufbx_free_scene(referenceScene);

			testRunner.Check(hasCorrectTimes, timeLabel.c_str());
			testRunner.Check(matchesSourcePoses && allClipsMove, poseLabel.c_str());
		}
	}

    bool AnimationPoseMatches(
        const std::vector<ModelMatrix>& matrices,
        const std::vector<float>& expectedXPositions
    )
    {
        if (matrices.size() != expectedXPositions.size())
            return false;

        for (std::size_t i = 0; i < matrices.size(); ++i)
        {
            const ModelVertex position = matrices[i].TransformPoint({ 0.0f, 0.0f, 0.0f });
            if (!(std::abs(position.x - expectedXPositions[i]) < 0.0001f &&
                std::abs(position.y) < 0.0001f && std::abs(position.z) < 0.0001f))
                return false;
        }
        return true;
    }

    void RunAnimationPoseTests(TestRunner& testRunner)
    {
        // 모델과 애니메이션의 관절 순서를 일부러 다르게 만든다.
        std::vector<SkeletonNode> modelNodes(3);
        modelNodes[0].name = "Root";
        modelNodes[0].localPositionX = 1.0f;
        modelNodes[1].name = "Child";
        modelNodes[1].parentIdx = 0;
        modelNodes[1].localPositionX = 2.0f;
        modelNodes[2].name = "Helper";
        modelNodes[2].localPositionX = 7.0f;

        AnimationFrame firstFrame;
        firstFrame.nodes = { modelNodes[1], modelNodes[0] };
        firstFrame.nodes[0].parentIdx = 1;
        firstFrame.nodes[1].localPositionX = 10.0f;

        AnimationFrame secondFrame = firstFrame;
        secondFrame.time = 0.2;
        secondFrame.nodes[0].localPositionX = 3.0f;
        secondFrame.nodes[1].localPositionX = 20.0f;

        AnimationClip clip;
        clip.duration = 0.2;
        clip.frames = { firstFrame, secondFrame };
        clip.sourceNodeIndices = { 1, 0, -1 };

        testRunner.Check(
            AnimationPoseMatches(SkeletonPoseCalculator::Calculate(modelNodes, clip, 0.0),
                { 10.0f, 12.0f, 7.0f }),
            "AnimationPose maps reordered nodes and preserves missing helper"
        );
        testRunner.Check(
            AnimationPoseMatches(SkeletonPoseCalculator::Calculate(modelNodes, clip, 0.2),
                { 20.0f, 23.0f, 7.0f }),
            "AnimationPose selects pose at exact frame time"
        );
        testRunner.Check(
            AnimationPoseMatches(SkeletonPoseCalculator::Calculate(modelNodes, clip, -1.0),
                { 10.0f, 12.0f, 7.0f }) &&
            AnimationPoseMatches(SkeletonPoseCalculator::Calculate(modelNodes, clip, 1.0),
                { 20.0f, 23.0f, 7.0f }),
            "AnimationPose clamps times to clip endpoints"
        );

        AnimationClip emptyClip;
        testRunner.Check(
            AnimationPoseMatches(SkeletonPoseCalculator::Calculate(modelNodes, emptyClip, 0.0),
                { 1.0f, 3.0f, 7.0f }),
            "AnimationPose preserves default pose for empty clip"
        );

        clip.frames = { firstFrame };
        testRunner.Check(
            AnimationPoseMatches(SkeletonPoseCalculator::Calculate(modelNodes, clip, 1.0),
                { 10.0f, 12.0f, 7.0f }),
            "AnimationPose supports a single frame"
        );

        const UfbxModelSourceReader reader;
        const ModelSourceReadResult modelSource = reader.Read(
            "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterLow.fbx");
        if (!modelSource.success || modelSource.source == nullptr)
        {
            testRunner.Check(false, "AnimationPose reads base skeleton");
            return;
        }

        const std::vector<SkeletonNode> monsterNodes = modelSource.source->GetSkeletonNodes();
        const std::vector<ModelMatrix> defaultMatrices = SkeletonPoseCalculator::Calculate(monsterNodes);
        std::vector<int> requiredNodeIndices;
        for (std::size_t meshIndex = 0; meshIndex < modelSource.source->GetMeshCount(); ++meshIndex)
        {
            for (const SkinBinding& binding : modelSource.source->GetSkinBindings(meshIndex))
                requiredNodeIndices.push_back(binding.nodeIdx);
        }

        const AnimationLoader animationLoader;
        const std::vector<std::string> fileNames = { "MonsterIdle.fbx", "MonsterWalk.fbx" };
        for (const std::string& fileName : fileNames)
        {
            const std::string filePath =
                "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/" + fileName;
            const std::string mappingLabel = "AnimationLoader maps model bones in " + fileName;
            const std::string poseLabel = "AnimationPose matches source world poses in " + fileName;
            ModelSourceReadResult sourceResult = reader.Read(filePath);
            ModelImportContext animationContext;
            animationContext.source = sourceResult.source;
            std::vector<AnimationClip> clips;
            const bool loaded = sourceResult.success && animationLoader.Load(
                animationContext, monsterNodes, clips);

            // 원본을 가진 두 참조를 모두 해제한 뒤 복사본을 사용한다.
            sourceResult.source.reset();
            animationContext.source.reset();

            ufbx_load_opts options{};
            options.target_axes = {
                UFBX_COORDINATE_AXIS_POSITIVE_X,
                UFBX_COORDINATE_AXIS_NEGATIVE_Y,
                UFBX_COORDINATE_AXIS_NEGATIVE_Z
            };
            options.target_unit_meters = 1.0;
            options.space_conversion = UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY;
            ufbx_error error{};
            ufbx_scene* referenceScene = ufbx_load_file(filePath.c_str(), &options, &error);
            if (referenceScene == nullptr)
            {
                testRunner.Check(false, mappingLabel.c_str());
                testRunner.Check(false, poseLabel.c_str());
                continue;
            }

            bool hasMapping = loaded && !clips.empty() && !requiredNodeIndices.empty() &&
                clips.size() == referenceScene->anim_stacks.count &&
                defaultMatrices.size() == monsterNodes.size();
            if (hasMapping)
            {
                for (const AnimationClip& loadedClip : clips)
                {
                    if (loadedClip.sourceNodeIndices.size() != monsterNodes.size() ||
                        loadedClip.frames.empty())
                    {
                        hasMapping = false;
                        break;
                    }
                    for (std::size_t modelIndex = 0; modelIndex < monsterNodes.size(); ++modelIndex)
                    {
                        int expectedIndex = -1;
                        for (const ufbx_node* referenceNode : referenceScene->nodes)
                        {
                            if (monsterNodes[modelIndex].name == referenceNode->name.data)
                            {
                                expectedIndex = static_cast<int>(referenceNode->typed_id);
                                break;
                            }
                        }
                        if (loadedClip.sourceNodeIndices[modelIndex] != expectedIndex)
                            hasMapping = false;
                    }
                    for (int requiredIndex : requiredNodeIndices)
                    {
                        if (requiredIndex < 0 || requiredIndex >= static_cast<int>(monsterNodes.size()) ||
                            loadedClip.sourceNodeIndices[requiredIndex] < 0)
                            hasMapping = false;
                    }
                }
            }
            testRunner.Check(hasMapping, mappingLabel.c_str());

            bool matchesWorldPoses = hasMapping;
            if (hasMapping)
            {
                for (std::size_t clipIndex = 0; clipIndex < clips.size(); ++clipIndex)
                {
                    const AnimationClip& loadedClip = clips[clipIndex];
                    const ufbx_anim_stack* sourceClip = referenceScene->anim_stacks.data[clipIndex];
                    const std::vector<std::size_t> sampleIndices = {
                        0, loadedClip.frames.size() / 2, loadedClip.frames.size() - 1
                    };
                    for (std::size_t sampleIndex : sampleIndices)
                    {
                        const double sampleTime = loadedClip.frames[sampleIndex].time;
                        ufbx_scene* evaluatedScene = ufbx_evaluate_scene(
                            referenceScene, sourceClip->anim, sourceClip->time_begin + sampleTime,
                            nullptr, nullptr);
                        if (evaluatedScene == nullptr)
                        {
                            matchesWorldPoses = false;
                            break;
                        }
                        const std::vector<ModelMatrix> actualMatrices =
                            SkeletonPoseCalculator::Calculate(monsterNodes, loadedClip, sampleTime);
                        if (actualMatrices.size() != monsterNodes.size())
                            matchesWorldPoses = false;
                        else
                        {
                            for (std::size_t modelIndex = 0; modelIndex < monsterNodes.size(); ++modelIndex)
                            {
                                const int sourceIndex = loadedClip.sourceNodeIndices[modelIndex];
                                if (sourceIndex == -1)
                                {
                                    for (int row = 0; row < 4; ++row)
                                        for (int column = 0; column < 4; ++column)
                                            if (!(std::abs(actualMatrices[modelIndex].values[row][column] -
                                                defaultMatrices[modelIndex].values[row][column]) <= 0.0001f))
                                                matchesWorldPoses = false;
                                    continue;
                                }
                                if (sourceIndex < 0 || sourceIndex >= static_cast<int>(evaluatedScene->nodes.count))
                                {
                                    matchesWorldPoses = false;
                                    continue;
                                }
                                const ufbx_matrix& expected = evaluatedScene->nodes.data[sourceIndex]->node_to_world;
                                for (int row = 0; row < 3; ++row)
                                    for (int column = 0; column < 4; ++column)
                                        if (!(std::abs(actualMatrices[modelIndex].values[row][column] -
                                            expected.cols[column].v[row]) <= 0.0001))
                                            matchesWorldPoses = false;
                            }
                        }
                        ufbx_free_scene(evaluatedScene);
                    }
                }
            }
            ufbx_free_scene(referenceScene);
            testRunner.Check(matchesWorldPoses, poseLabel.c_str());
        }
    }
	void RunMeshLoaderTests(TestRunner& testRunner)
	{
		RunAnimationSourceTests(testRunner);
		RunAnimationPoseTests(testRunner);

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

		ModelMesh skinMesh;
		// X 중심이 2인 모델의 정렬된 위치와 원본 위치.
		skinMesh.vertices = {
			{ -1.0f, 0.0f, 0.0f, 0.25f, 0.75f },
			{ 1.0f, 0.0f, 0.0f, 0.5f, 0.25f }
		};
		skinMesh.localVertices = {
			{ 1.0f, 0.0f, 0.0f },
			{ 3.0f, 0.0f, 0.0f }
		};
		skinMesh.skinWeights = {
			{ { 0, 0.25f }, { 1, 0.75f } },
			{}
		};

		SkinBinding firstBinding;
		firstBinding.nodeIdx = 0;
		firstBinding.geometryToBone.values[0][3] = -1.0f;

		SkinBinding secondBinding;
		secondBinding.nodeIdx = 1;
		secondBinding.geometryToBone.values[0][3] = -1.0f;

		// 관절 번호와 바인딩 목록 순서가 달라도 올바르게 연결해야 한다.
		skinMesh.skinBindings = { secondBinding, firstBinding };

		std::vector<ModelMatrix> skinNodeMatrices(2);
		skinNodeMatrices[0].values[0][3] = 3.0f;
		skinNodeMatrices[1].values[0][3] = 7.0f;

		ModelMatrix skinAlignment;
		skinAlignment.values[0][3] = -2.0f;

		const std::vector<ModelVertex> skinnedVertices =
			SkinningCalculator::Calculate(skinMesh, skinNodeMatrices, skinAlignment);

		// (1 - 1 + 3) * 0.25 + (1 - 1 + 7) * 0.75 - 2 = 4.
		testRunner.Check(
			skinnedVertices.size() == 2 &&
			std::abs(skinnedVertices[0].x - 4.0f) < 0.0001f &&
			std::abs(skinnedVertices[0].y) < 0.0001f &&
			std::abs(skinnedVertices[0].z) < 0.0001f &&
			skinnedVertices[0].u == 0.25f &&
			skinnedVertices[0].v == 0.75f,
			"SkinningCalculator blends bone movement and preserves UVs"
		);

		testRunner.Check(
			skinnedVertices.size() == 2 &&
			skinnedVertices[1].x == 1.0f &&
			skinnedVertices[1].y == 0.0f &&
			skinnedVertices[1].z == 0.0f &&
			skinnedVertices[1].u == 0.5f &&
			skinnedVertices[1].v == 0.25f,
			"SkinningCalculator preserves vertices without bone influence"
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

		testRunner.Check(
			monsterResult.success && monsterResult.source != nullptr &&
			monsterResult.source->GetAnimationClips().empty(),
			"AnimationSource returns no clips for static monster"
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
			std::make_shared<SkinWeightLoader>(),
			std::make_shared<AnimationLoader>()
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




        // 같은 모델에 Idle과 Move를 차례로 추가한다.
        ModelData animatedModel = importResult.model;
        const bool idleLoaded = importResult.success && importer.ImportAnimation(
            animatedModel,
            "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterIdle.fbx",
            "Idle"
        );

        testRunner.Check(
            idleLoaded && animatedModel.animationClips.size() == 1 &&
            animatedModel.animationClips[0].name == "Idle" &&
            animatedModel.animationClips[0].duration > 0.0 &&
            !animatedModel.animationClips[0].frames.empty() &&
            animatedModel.animationClips[0].sourceNodeIndices.size() ==
                animatedModel.skeltonNodes.size(),
            "ModelImporter loads named Idle animation"
        );

        const std::size_t idleFrameCount =
            animatedModel.animationClips.empty() ? 0 :
            animatedModel.animationClips[0].frames.size();
        const double idleDuration =
            animatedModel.animationClips.empty() ? 0.0 :
            animatedModel.animationClips[0].duration;

        const bool moveLoaded = idleLoaded && importer.ImportAnimation(
            animatedModel,
            "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterWalk.fbx",
            "Move"
        );

        testRunner.Check(
            moveLoaded && animatedModel.animationClips.size() == 2 &&
            animatedModel.animationClips[0].name == "Idle" &&
            animatedModel.animationClips[0].frames.size() == idleFrameCount &&
            animatedModel.animationClips[0].duration == idleDuration &&
            animatedModel.animationClips[1].name == "Move" &&
            animatedModel.animationClips[1].duration > 0.0 &&
            !animatedModel.animationClips[1].frames.empty() &&
            animatedModel.animationClips[1].sourceNodeIndices.size() ==
                animatedModel.skeltonNodes.size(),
            "ModelImporter appends Move without losing Idle"
        );

        const bool rejectsStaticFile = !importer.ImportAnimation(
            animatedModel,
            "C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterLow.fbx",
            "Unused"
        );

        testRunner.Check(
            moveLoaded && rejectsStaticFile &&
            animatedModel.animationClips.size() == 2 &&
            animatedModel.animationClips[0].name == "Idle" &&
            animatedModel.animationClips[0].frames.size() == idleFrameCount &&
            animatedModel.animationClips[0].duration == idleDuration &&
            animatedModel.animationClips[1].name == "Move",
            "ModelImporter rejects animation-free FBX without losing clips"
        );

		// 원본 라이브러리 결과와 비교해 실제 FBX의 관절 행렬을 검사한다.
		ufbx_load_opts referenceOptions{};
		referenceOptions.target_axes = {
			UFBX_COORDINATE_AXIS_POSITIVE_X,
			UFBX_COORDINATE_AXIS_NEGATIVE_Y,
			UFBX_COORDINATE_AXIS_NEGATIVE_Z
		};
		referenceOptions.target_unit_meters = 1.0;
		referenceOptions.space_conversion = UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY;
		ufbx_error referenceError{};
		ufbx_scene* referenceScene = ufbx_load_file(
			"C:/WorkSpace/HiwoongEngine/AsciiDoom/Assets/Model/MonsterLow.fbx",
			&referenceOptions, &referenceError
		);

		bool matchesSourcePose = false;
		bool matchesBindPose = false;
		bool hasBoneInfluence = false;

		if (importResult.success && referenceScene != nullptr &&
			!importResult.model.skeltonNodes.empty() &&
			!importResult.model.meshes.empty())
		{
			const std::vector<ModelMatrix> defaultMatrices =
				SkeletonPoseCalculator::Calculate(importResult.model.skeltonNodes);
			matchesSourcePose =
				defaultMatrices.size() == referenceScene->nodes.count;

			if (matchesSourcePose)
			{
				for (std::size_t index = 0; index < defaultMatrices.size(); ++index)
				{
					const ufbx_matrix& reference = referenceScene->nodes.data[index]->node_to_world;
					for (int row = 0; row < 3; ++row)
					{
						for (int column = 0; column < 4; ++column)
						{
							if (!(std::abs(defaultMatrices[index].values[row][column] -
								reference.cols[column].v[row]) <= 0.0001))
							{
								matchesSourcePose = false;
							}
						}
					}
				}
			}

			matchesBindPose =
				defaultMatrices.size() == referenceScene->nodes.count &&
				importResult.model.meshes.size() == referenceScene->meshes.count;

			if (matchesBindPose)
			{
				for (std::size_t meshIndex = 0; meshIndex < importResult.model.meshes.size(); ++meshIndex)
				{
					const ModelMesh& modelMesh = importResult.model.meshes[meshIndex];
					const ufbx_mesh* sourceMesh = referenceScene->meshes.data[meshIndex];
					if (modelMesh.vertices.empty() ||
						modelMesh.localVertices.size() != modelMesh.vertices.size() ||
						modelMesh.skinWeights.size() != modelMesh.vertices.size())
					{
						matchesBindPose = false;
						break;
					}

					// 이 메시를 관절에 묶었을 때의 자세를 사용한다.
					auto bindMatrices = defaultMatrices;
					if (sourceMesh->skin_deformers.count > 0)
					{
						for (const ufbx_skin_cluster* cluster : sourceMesh->skin_deformers.data[0]->clusters)
						{
							if (cluster->bone_node == nullptr ||
								cluster->bone_node->typed_id >= bindMatrices.size())
							{
								matchesBindPose = false;
								break;
							}
							ModelMatrix& matrix = bindMatrices[cluster->bone_node->typed_id];
							for (int row = 0; row < 3; ++row)
							{
								for (int column = 0; column < 4; ++column)
								{
									matrix.values[row][column] =
										static_cast<float>(cluster->bind_to_world.cols[column].v[row]);
								}
							}
						}
					}
					if (!matchesBindPose)
						break;

					const std::vector<ModelVertex> bindVertices = SkinningCalculator::Calculate(
						modelMesh, bindMatrices, importResult.model.modelAlignment);
					if (bindVertices.size() != modelMesh.vertices.size())
					{
						matchesBindPose = false;
						break;
					}

					for (std::size_t index = 0; index < bindVertices.size(); ++index)
					{
						for (const SkinWeight& weight : modelMesh.skinWeights[index])
						{
							if (weight.weight > 0.0f)
								hasBoneInfluence = true;
						}
						const ModelVertex& actual = bindVertices[index];
						const ModelVertex& expected = modelMesh.vertices[index];
						// 미터 단위에서 위치 오차를 0.1mm까지 허용한다.
						if (!(std::abs(actual.x - expected.x) <= 0.0001f &&
							std::abs(actual.y - expected.y) <= 0.0001f &&
							std::abs(actual.z - expected.z) <= 0.0001f &&
							actual.u == expected.u && actual.v == expected.v))
						{
							matchesBindPose = false;
						}
					}
				}
			}
		}
		ufbx_free_scene(referenceScene);

		testRunner.Check(
			matchesSourcePose,
			"SkeletonPoseCalculator matches monster source node matrices"
		);
		testRunner.Check(
			matchesBindPose && hasBoneInfluence,
			"SkinningCalculator reconstructs monster bind pose"
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