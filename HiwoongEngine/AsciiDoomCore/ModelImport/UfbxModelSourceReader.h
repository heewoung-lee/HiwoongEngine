#pragma once

#include "IModelSourceReader.h"
#include "ModelSourceReadResult.h"

namespace Hiwoong
{
	/// <summary>
	/// 모델 소스 읽는 방식을 Ufbx리더로 하는 구현체
	/// </summary>
	class UfbxModelSourceReader final : public IModelSourceReader
	{
	public:
		ModelSourceReadResult Read(
			const std::string& filePath
		) const override;
	};
}