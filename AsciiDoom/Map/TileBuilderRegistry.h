#pragma once
#include "ITileBuilder.h"
#include <memory>
#include <unordered_map>


namespace Hiwoong
{
	class TileBuilderRegistry
	{
	public:
		//문자 등록기
		void RegisterBuilder(char tile, std::unique_ptr<ITileBuilder> builder);
		
		//등록된 
		ITileBuilder& FindBuilder(char tile) const;


	private:

		//각 문자들의 오브젝트 생성기를 담고있는 맵ㄴ
		std::unordered_map<char, std::unique_ptr<ITileBuilder>> builders;
	};
}

