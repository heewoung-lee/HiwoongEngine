#include "PlayerSpawnTileBuilder.h"
#include "Map/DoomMap.h"
namespace Hiwoong
{
	void PlayerSpawnTileBuilder::Build(DoomMap& map, const Vector3& worldPosition)
	{
		map.SetPlayerSpawnPosition(worldPosition);
	}
}

