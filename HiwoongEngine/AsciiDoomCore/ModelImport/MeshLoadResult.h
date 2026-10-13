#pragma once

#include "ModelMesh.h"
#include "MeshSourceMapping.h"
#include <string>
#include <vector>


namespace Hiwoong
{
	/// <summary>
	/// 메시 변환 결과
	/// </summary>
	struct MeshLoadResult
	{
		bool success = false;
		std::vector<ModelMesh> meshes;
		// sourceMapping[i]는 meshes[i]에 대응함.
		std::vector<MeshSourceMapping> sourceMappings;


		std::string errorMessage;

	};

}
