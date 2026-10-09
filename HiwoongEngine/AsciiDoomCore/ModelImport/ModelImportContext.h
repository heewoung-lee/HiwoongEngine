#pragma once

#include "MeshSourceMapping.h"
#include <memory>
#include <vector>

/// <summary>
/// 로더들이 사용할 원본과 대응표를 전달하는 역할
/// </summary>
namespace Hiwoong
{
	class IModelSource;


	struct ModelImportContext
	{
		//이미 읽어둔 FBX원본데이터
		std::shared_ptr<const IModelSource> source;

		// 메시 로더가 만든 정점 대응표
		std::vector<MeshSourceMapping> meshMappings;

	};


}