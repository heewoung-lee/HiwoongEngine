#include "UfbxModelSource.h"
#include "../ThirdParty/ufbx.h"
#include <algorithm>
#include <filesystem>
#include <cstdint>
#include <cassert>
#include <utility>
namespace Hiwoong
{
	UfbxModelSource::UfbxModelSource(ufbx_scene* scene)
		: scene(scene)
	{
	}

	UfbxModelSource::~UfbxModelSource()
	{
		ufbx_free_scene(scene);
	}
	std::size_t UfbxModelSource::GetMeshCount() const
	{
		return scene->meshes.count;
	}
	std::string UfbxModelSource::GetMeshName(
		std::size_t meshIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

		return std::string(mesh->name.data, mesh->name.length);
	}
	ModelVertex UfbxModelSource::GetVertex(
		std::size_t meshIndex,
		std::size_t vertexIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
		const ufbx_vec3 position = mesh->vertices.data[vertexIndex];

		return ModelVertex{
			static_cast<float>(position.x),
			static_cast<float>(position.y),
			static_cast<float>(position.z)
		};
	}
	std::size_t UfbxModelSource::GetFaceCount(
		std::size_t meshIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

		return mesh->num_faces;
	}
	//다면체 분할
	std::vector<ModelTriangle> UfbxModelSource::TriangulateFace(
		std::size_t meshIndex,
		std::size_t faceIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
		const ufbx_face& face = mesh->faces.data[faceIndex];

		if (face.num_indices < 3)
			return {};

		std::vector<std::uint32_t> indices(
			(static_cast<std::size_t>(face.num_indices) - 2) * 3
		);

		const std::size_t triangleCount = ufbx_triangulate_face(
			indices.data(),
			indices.size(),
			mesh,
			face
		);

		std::vector<ModelTriangle> triangles;

		std::size_t materialIndex = 0;

		if (faceIndex < mesh->face_material.count)
		{
			materialIndex = mesh->face_material.data[faceIndex];
		}

		for (std::size_t i = 0; i < triangleCount; ++i)
		{
			const std::size_t offset = i * 3;
			triangles.push_back({
			indices[offset],
			indices[offset + 1],
			indices[offset + 2],
			materialIndex });
		}

		return triangles;
	}
	std::size_t UfbxModelSource::GetCornerCount(std::size_t meshIndex) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
		return mesh->num_indices;
	}
	ModelVertex UfbxModelSource::GetCornerVertex(
		std::size_t meshIndex,
		std::size_t cornerIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

		assert(cornerIndex < mesh->num_indices);

		// 이 코너가 사용하는 원본 정점 번호.
		const std::size_t vertexIndex =
			mesh->vertex_indices.data[cornerIndex];

		assert(mesh->instances.count == 1);
		const ufbx_node* node = mesh->instances.data[0];

		const ufbx_vec3 position = ufbx_transform_position(
			&node->geometry_to_world,
			mesh->vertices.data[vertexIndex]
		);

		ModelVertex vertex{
			static_cast<float>(position.x),
			static_cast<float>(position.y),
			static_cast<float>(position.z)
		};

		if (mesh->vertex_uv.exists)
		{
			const ufbx_vec2 uv = ufbx_get_vertex_vec2(
				&mesh->vertex_uv,
				cornerIndex
			);

			vertex.u = static_cast<float>(uv.x);
			vertex.v = static_cast<float>(uv.y);
		}

		return vertex;
	}
	std::size_t UfbxModelSource::GetCornerSourceVertexIndex(
		std::size_t meshIndex,
		std::size_t cornerIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

		assert(cornerIndex < mesh->num_indices);

		return mesh->vertex_indices.data[cornerIndex];
	}
	std::vector<std::string> UfbxModelSource::GetDiffuseTexturePaths(
		std::size_t meshIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

		assert(mesh->instances.count == 1);
		const ufbx_node* node = mesh->instances.data[0];

		// 재질 개수만큼 빈 문자열을 만들어 번호를 유지한다.
		std::vector<std::string> paths(node->materials.count);

		for (std::size_t i = 0; i < node->materials.count; ++i)
		{
			const ufbx_material* material = node->materials.data[i];
			const ufbx_texture* texture =
				material->fbx.diffuse_color.texture;

			if (texture == nullptr)
			{
				texture = material->pbr.base_color.texture;
			}
			if (texture != nullptr && texture->filename.length > 0)
			{
				// fbxPath 생성
				const std::filesystem::path fbxPath(
					std::string(
						scene->metadata.filename.data,
						scene->metadata.filename.length
					)
				);
				// imagePath 생성
				const std::filesystem::path imagePath(
					std::string(
						texture->filename.data,
						texture->filename.length
					)
				);
				// paths[i] 대입
				paths[i] = (
					fbxPath.parent_path() / "textures" / imagePath.filename()
					).string();

			}

		}

		return paths;
	}
	std::vector<SkeletonNode> UfbxModelSource::GetSkeletonNodes() const
	{
		std::vector<SkeletonNode> nodes;

		for (const ufbx_node* node : scene->nodes)
		{
			SkeletonNode skeletonNode;

			skeletonNode.name = node->name.data;

			//관절 위치
			const ufbx_vec3 position = node->local_transform.translation;

			skeletonNode.localPositionX = static_cast<float>(position.x);
			skeletonNode.localPositionY = static_cast<float>(position.y);
			skeletonNode.localPositionZ = static_cast<float>(position.z);

			//관절 회전
			const ufbx_quat rotation = node->local_transform.rotation;

			skeletonNode.localRotationX = static_cast<float>(rotation.x);
			skeletonNode.localRotationY = static_cast<float>(rotation.y);
			skeletonNode.localRotationZ = static_cast<float>(rotation.z);
			skeletonNode.localRotationW = static_cast<float>(rotation.w);

			// 관절 크기
			const ufbx_vec3 scale = node->local_transform.scale;

			skeletonNode.localScaleX = static_cast<float>(scale.x);
			skeletonNode.localScaleY = static_cast<float>(scale.y);
			skeletonNode.localScaleZ = static_cast<float>(scale.z);

			if (node->parent != nullptr)
			{
				skeletonNode.parentIdx =
					static_cast<int>(node->parent->typed_id);
			}
			nodes.push_back(std::move(skeletonNode));
		}
		return nodes;
	}
	std::vector<std::vector<SkinWeight>> UfbxModelSource::GetSkinWeights(
		std::size_t meshIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

		// 원본 정점마다 빈 가중치 목록을 하나씩 만든다.
		std::vector<std::vector<SkinWeight>> weights(mesh->num_vertices);

		// 관절 연결 정보가 없는 메시라면 빈 목록을 반환한다.
		if (mesh->skin_deformers.count == 0)
		{
			return weights;
		}

		// 정점과 관절의 연결 정보인 스킨 데이터를 가져옴
		// 스킨안에는 각 정점이 어떤 관절을 얼마나 따라가는지 들어있기 때문에
		const ufbx_skin_deformer* skin = mesh->skin_deformers.data[0];

		//정점을 순회
		for (std::size_t vertexIdx = 0;
			vertexIdx < mesh->num_vertices;
			++vertexIdx)
		{
			const ufbx_skin_vertex& vertex = skin->vertices.data[vertexIdx];

			//정점만의 관절을 순회
			for (std::size_t i = 0; i < vertex.num_weights; ++i)
			{
				const ufbx_skin_weight& sourceWeight =
					skin->weights.data[vertex.weight_begin + i];

				const ufbx_skin_cluster* cluster =
					skin->clusters.data[sourceWeight.cluster_index];

				SkinWeight weight;
				weight.nodeIdx = static_cast<int>(cluster->bone_node->typed_id);
				weight.weight = static_cast<float>(sourceWeight.weight);

				weights[vertexIdx].push_back(weight);
			}
		}

		return weights;
	}
	std::vector<SkinBinding> UfbxModelSource::GetSkinBindings(
		std::size_t meshIndex
	) const
	{
		const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
		std::vector<SkinBinding> bindings;

		if (mesh->skin_deformers.count == 0)
			return bindings;

		const ufbx_skin_deformer* skin = mesh->skin_deformers.data[0];

		//메시와 관절 하나를 연결하는 정보 각메시별 관절번호를 매칭
		for (const ufbx_skin_cluster* cluster : skin->clusters)
		{
			SkinBinding binding;
			binding.nodeIdx =
				static_cast<int>(cluster->bone_node->typed_id);

			//FBX 데이터의 행렬 값 복사
			const ufbx_matrix& matrix = cluster->geometry_to_bone;

			for (int row = 0; row < 3; ++row)
			{
				for (int column = 0; column < 4; ++column)
				{
					binding.geometryToBone.SetValue(
						row,
						column,
						static_cast<float>(matrix.cols[column].v[row])
					);
				}
			}


			bindings.push_back(binding);
		}

		return bindings;
	}
	std::vector<AnimationClip> UfbxModelSource::GetAnimationClips() const
	{
		std::vector<AnimationClip> clips;

		for (const ufbx_anim_stack* sourceClip : scene->anim_stacks)
		{
			AnimationClip clip;

			clip.name = sourceClip->name.data;
			clip.duration = sourceClip->time_end - sourceClip->time_begin;

			int frameIndex = 0;

			while (true)
			{
				const double time = std::min(
					frameIndex / 30.0,
					clip.duration
				);

				ufbx_scene* evaluatedScene = ufbx_evaluate_scene(
					scene,
					sourceClip->anim,
					sourceClip->time_begin + time,
					nullptr,
					nullptr
				);

				assert(evaluatedScene != nullptr);
				if (evaluatedScene == nullptr)
					return {};

				UfbxModelSource frameSource(evaluatedScene);

				AnimationFrame frame;
				frame.time = time;
				frame.nodes = frameSource.GetSkeletonNodes();

				clip.frames.push_back(std::move(frame));

				if (time >= clip.duration)
					break;

				++frameIndex;
			}

			clips.push_back(std::move(clip));
		}

		return clips;
	}
}

