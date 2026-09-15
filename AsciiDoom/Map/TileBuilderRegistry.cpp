#include "TileBuilderRegistry.h"
#include <cassert>
#include <utility>
namespace Hiwoong
{
	//맵에 오브젝트 생성기 등록.
	void TileBuilderRegistry::RegisterBuilder(char tile, std::unique_ptr<ITileBuilder> builder)
	{
		assert(builder && "Tile builder is null");


		//조회해서 소유권을 전환.
		//댕글링 참조를 방지하기 위해.
		//소유권을 등록기로 옮겨서 안전하게 등록.
		const auto [iterator, inserted] =
			builders.emplace(tile, std::move(builder));

		assert(inserted && "Tile builder is already registered");

	}
	ITileBuilder& TileBuilderRegistry::FindBuilder(char tile) const
	{
		//등록이 안되면 에러를 뱉게 해야함
		return *builders.at(tile);
	}
}