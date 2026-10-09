#pragma once

#include "IModelSource.h"
#include <memory>
#include <string>


namespace Hiwoong 
{
	/// <summary>
	/// 파일 읽기 결과 성공하면 원본을 저장
	/// </summary>
	struct ModelSourceReadResult
	{
		bool success = false;

		// 읽어 둔 원본 데이터
		std::shared_ptr<const IModelSource> source;

		// 읽기에 실패한 이유
		std::string errorMessage;

	};
	
}
