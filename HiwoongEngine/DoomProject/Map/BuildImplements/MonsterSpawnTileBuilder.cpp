#include "MonsterSpawnTileBuilder.h"
#include "Map/DoomMap.h"

namespace Hiwoong
{
	void MonsterSpawnTileBuilder::Build(DoomMap& map, const Vector3& worldPosition)
	{
		map.AddMonsterSpawnPosition(worldPosition);
	}
}

