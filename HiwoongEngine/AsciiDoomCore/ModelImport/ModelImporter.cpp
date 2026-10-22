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
#include "Math/Vector4.h"
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
		const std::string& filePath,
		float frontRotationY
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
			mesh.diffuseTextures.resize(mesh.diffuseTextureInfos.size());

			for (std::size_t i = 0;
				i < mesh.diffuseTextureInfos.size();
				++i)
			{
				const ModelTextureInfo& textureInfo =
					mesh.diffuseTextureInfos[i];

				if (textureInfo.filePath.empty())
					continue;

				auto texture = std::make_shared<ModelTexture>();

				texture->info = textureInfo;

				if (!LoadPngTexture(textureInfo.filePath, *texture))
				{
					result.errorMessage =
						"Failed to load texture: " + textureInfo.filePath;

					return result;
				}

				mesh.diffuseTextures[i] = texture;
			}
		}
		//메시와 스켈레톤 모관
		result.model.meshes = std::move(meshResult.meshes);
		result.model.skeltonNodes = skeletonLoader->Load(context);

		// 대응표를 전달해서 출력메시의 가중치를 채움
		context.meshMappings = std::move(meshResult.sourceMappings);

		if (skinWeightLoader->Load(context, result.model.meshes) == false)
		{
			result.errorMessage = "Skin weight loading failed";
			return result;
		}

		//모델 정렬
		AlignModel(result.model, frontRotationY);
		
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
	void ModelImporter::AlignModel(ModelData& model, float frontRotationY) const
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

		// 중심을 원점으로 옮긴 뒤, 정면 방향을 맞춘다.
		model.modelAlignment =
			Matrix4x4::RotationY(frontRotationY) *
			Matrix4x4::Translation(
				Vector3(-center.x, -center.y, -center.z)
			);

		for (ModelMesh& mesh : model.meshes)
		{
			for (ModelVertex& vertex : mesh.vertices)
			{
				const Vector4 position =
					model.modelAlignment *
					Vector4(vertex.x, vertex.y, vertex.z, 1.0f);

				vertex.x = position.x;
				vertex.y = position.y;
				vertex.z = position.z;
			}
		}


	}
}