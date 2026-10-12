#include "ModelImporter.h"
#include "IModelSourceReader.h"
#include "IMeshLoader.h"
#include "ModelSourceReadResult.h"
#include "ModelImportContext.h"
#include "MeshLoadResult.h"
#include "PngTextureLoader.h"
#include "ISkeletonLoader.h"
#include "ISkinWeightLoader.h"
#include "IAnimationLoader.h"
#include <algorithm>
#include <limits>
#include <utility>
#include <cassert>

namespace Hiwoong
{
	ModelImporter::ModelImporter(
		std::shared_ptr<const IModelSourceReader> sourceReader,
		std::shared_ptr<const IMeshLoader> meshLoader,
		std::shared_ptr<const ISkeletonLoader> skeletonLoader,
		std::shared_ptr<const ISkinWeightLoader> skinWeightLoader,
		std::shared_ptr<const IAnimationLoader> animationLoader
	)
		: sourceReader(sourceReader),
		meshLoader(meshLoader),
		skeletonLoader(skeletonLoader),
		skinWeightLoader(skinWeightLoader),
		animationLoader(animationLoader)
	{
		assert(this->sourceReader != nullptr);
		assert(this->meshLoader != nullptr);
		assert(this->skeletonLoader != nullptr);
		assert(this->skinWeightLoader != nullptr);
		assert(this->animationLoader != nullptr);
	}

	ModelImportResult ModelImporter::Import(
		const std::string& filePath
	) const
	{
		ModelImportResult result;

		const ModelSourceReadResult readResult =
			sourceReader->Read(filePath);

		if (!readResult.success || readResult.source == nullptr)
		{
			result.errorMessage = readResult.errorMessage;
			return result;
		}

		ModelImportContext context;
		context.source = readResult.source;

		MeshLoadResult meshResult = meshLoader->Load(context);

		if (!meshResult.success)
		{
			result.errorMessage = meshResult.errorMessage;
			return result;
		}

		for (ModelMesh& mesh : meshResult.meshes)
		{
			mesh.diffuseTextures.resize(mesh.diffuseTexturePaths.size());

			for (std::size_t i = 0;
				i < mesh.diffuseTexturePaths.size();
				++i)
			{
				const std::string& texturePath = mesh.diffuseTexturePaths[i];

				if (texturePath.empty())
					continue;

				auto texture = std::make_shared<ModelTexture>();

				if (!LoadPngTexture(texturePath, *texture))
				{
					result.errorMessage =
						"Failed to load texture: " + texturePath;

					return result;
				}

				mesh.diffuseTextures[i] = texture;
			}
		}
		//메시와 스켈레톤 모관
		result.model.meshes = std::move(meshResult.meshes);
		result.model.skeltonNodes = skeletonLoader->Load(context);

		// 대응표를 전달해서 출력메시의 가중치를 채움
		context.meshMappings = meshResult.sourceMappings;

		if (skinWeightLoader->Load(context, result.model.meshes) == false)
		{
			result.errorMessage = "Skin weight loading failed";
			return result;
		}

		//모델 정렬
		CenterModel(result.model);

		result.success = true;

		return result;
	}
	bool ModelImporter::ImportAnimation(
		ModelData& model,
		const std::string& filePath,
		const std::string& name
	) const
	{
		const ModelSourceReadResult readResult =
			sourceReader->Read(filePath);

		if (!readResult.success || readResult.source == nullptr)
			return false;

		ModelImportContext context;
		context.source = readResult.source;

		std::vector<AnimationClip> clips;

		if (!animationLoader->Load(context, model.skeltonNodes, clips))
			return false;

		// 지금 사용하는 Mixamo 파일은 파일당 애니메이션 하나.
		if (clips.size() != 1)
			return false;

		clips[0].name = name;
		model.animationClips.push_back(std::move(clips[0]));

		return true;
	}
	void ModelImporter::CenterModel(ModelData& model) const
	{
		const float largest = std::numeric_limits<float>::max();

		ModelVertex minPosition{ largest, largest, largest };
		ModelVertex maxPosition{ -largest, -largest, -largest };

		for (const ModelMesh& mesh : model.meshes)
		{
			for (const ModelVertex& vertex : mesh.vertices)
			{
				minPosition.x = std::min(minPosition.x, vertex.x);
				minPosition.y = std::min(minPosition.y, vertex.y);
				minPosition.z = std::min(minPosition.z, vertex.z);

				maxPosition.x = std::max(maxPosition.x, vertex.x);
				maxPosition.y = std::max(maxPosition.y, vertex.y);
				maxPosition.z = std::max(maxPosition.z, vertex.z);
			}
		}

		const ModelVertex center{
			(minPosition.x + maxPosition.x) * 0.5f,
			(minPosition.y + maxPosition.y) * 0.5f,
			(minPosition.z + maxPosition.z) * 0.5f
		};

		//기존 정점에서 빼던 중심값을 행렬에도 보관.
		model.modelAlignment.values[0][3] = -center.x;
		model.modelAlignment.values[1][3] = -center.y;
		model.modelAlignment.values[2][3] = -center.z;


		for (ModelMesh& mesh : model.meshes)
		{
			for (ModelVertex& vertex : mesh.vertices)
			{
				vertex.x -= center.x;
				vertex.y -= center.y;
				vertex.z -= center.z;
			}
		}


	}
}