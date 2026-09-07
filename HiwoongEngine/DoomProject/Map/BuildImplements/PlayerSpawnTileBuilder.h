#pragma once

#include "Map/ITileBuilder.h"

namespace Hiwoong
{
	class PlayerSpawnTileBuilder: public ITileBuilder
	{
	public:
		void Build(DoomMap& map, const Vector3& worldPosition) override;

	};
}

