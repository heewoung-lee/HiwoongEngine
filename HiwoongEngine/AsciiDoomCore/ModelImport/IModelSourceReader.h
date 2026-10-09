#pragma once

#include <string>

namespace Hiwoong
{
	struct ModelSourceReadResult;

	/// <summary>
	/// FBX파일 경로를 받아 원본을 읽는 계약
	/// </summary>
	class IModelSourceReader
	{
	public:
		virtual ~IModelSourceReader() = default;

		virtual ModelSourceReadResult Read(
			const std::string& filePath
		) const = 0;
	};

}