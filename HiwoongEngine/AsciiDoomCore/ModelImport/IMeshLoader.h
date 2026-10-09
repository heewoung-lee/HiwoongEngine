#pragma once


namespace Hiwoong
{
	struct ModelImportContext;
	struct MeshLoadResult;
	/// <summary>
	/// 외부에서 가져온 FBX정보를 우리의 ModelMesh 형태로 변환
	/// </summary>
	class IMeshLoader
	{
	public:
		virtual ~IMeshLoader() = default;

		virtual MeshLoadResult Load(
			const ModelImportContext& context
		) const = 0;
	};
}