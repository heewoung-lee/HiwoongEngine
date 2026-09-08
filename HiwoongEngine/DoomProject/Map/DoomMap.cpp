#include "DoomMap.h"
#include "BuildImplements/WallTileBuilder.h"
#include "BuildImplements/EmptyTileBuilder.h"
#include "BuildImplements/PlayerSpawnTileBuilder.h"
#include "BuildImplements/MonsterSpawnTileBuilder.h"
#include <memory>
#include <fstream>
#include <cassert>
#include <string>
#include <utility>
#include <cmath>
namespace Hiwoong
{
	void DoomMap::AddOnMapBuilt(std::function<void()> callback)
	{
		if (callback == nullptr) return;

		//불필요한 복사 비용을 줄이고 소유권을 건네줌.
		onMapBuiltCallbacks.emplace_back(std::move(callback));

	}
	DoomMap::DoomMap(const std::string& mapPath) : mapPath(mapPath)
	{
		
	}
	Mesh& DoomMap::GetMapMesh()
	{
		return mapMesh;
	}
	//플레이어 스폰포지션 설정.
	void DoomMap::SetPlayerSpawnPosition(const Vector3& position)
	{
		assert(
			!hasPlayerSpawnPosition &&
			"Map contains multiple player spawn positions"
		);

		playerSpawnPosition = position;
		hasPlayerSpawnPosition = true;
	}

	//플레이어 포지션 가져오기.
	const Vector3& DoomMap::GetPlayerSpawnPosition() const
	{
		assert(
			hasPlayerSpawnPosition &&
			"Player spawn position has not been set"
		);

		return playerSpawnPosition;
	}

	void DoomMap::AddMonsterSpawnPosition(const Vector3& position)
	{
		monsterSpawnPositions.emplace_back(position);
	}

	void DoomMap::Start()
	{
		super::Start();

		LoadMap();
		RegisterTileBuilders();
		BuildMap();

		BroadcastOnMapBuilt();//콜백 호출
	}
	//인접칸이 벽인지 확인하는 메서드.
	//
	bool DoomMap::IsWallTile(int x, int z) const
	{
		if (z < 0 || z >= static_cast<int>(rows.size()))
			return false;

		if (x < 0 || x >= static_cast<int>(rows[z].size()))
			return false;

		return rows[z][x] == '#';
	}

	bool DoomMap::CanMoveTo(const Vector3& worldPosition, float halfSize) const
	{

		//맵과 플레이어를 독립적으로 둔 상태에서, 이동예정 좌표를 전달하고,
		//이후 맵의 로컬관점에서 다시 수정.
		const Vector3 mapRelativePosition =
			worldPosition - GetWorldPosition();

		const float minX = mapRelativePosition.x - halfSize;
		const float maxX = mapRelativePosition.x + halfSize;
		const float minZ = mapRelativePosition.z - halfSize;
		const float maxZ = mapRelativePosition.z + halfSize;

		//이 범위의 타일번호 구하기.
		const int minTileX = static_cast<int>(std::floor(minX + 0.5f));
		const int maxTileX = static_cast<int>(std::floor(maxX + 0.5f));
		const int minTileZ = static_cast<int>(std::floor(minZ + 0.5f));
		const int maxTileZ = static_cast<int>(std::floor(maxZ + 0.5f));


		// 맵 데이터가 없거나 플레이어 몸의 일부가 맵 범위를 벗어나면 이동 불가
		if (rows.empty() ||
			minTileX < 0 || minTileZ < 0 ||
			maxTileZ >= static_cast<int>(rows.size()) ||
			maxTileX >= static_cast<int>(rows[0].size()))
		{
			return false;
		}


		//몸이 차지하는 칸들만 검사.
		for (int z = minTileZ; z <= maxTileZ; ++z)
		{
			for (int x = minTileX; x <= maxTileX;++x)
			{
				if (IsWallTile(x, z)) return false;
			}
		}

		//검사한 모든 칸에 벽이 없다면 ㅎ토과.
		return true;
	}

	//맵 읽기. 여기에서는 직사각형의 크키만 받을 것.
	void DoomMap::LoadMap()
	{
		std::ifstream file(mapPath);

		assert(file.is_open() && "Failed to open map file");
		
		//한줄씩 저장.
		std::string row;
		while (std::getline(file,row))
		{
			rows.emplace_back(row);
		}
		assert(!rows.empty() && "Map file is empty");

		//행 크기 검사
		const size_t mapWidth = rows[0].size();
		assert(mapWidth > 0 && "Map width is zero");

		for (const std::string& row : rows)
		{
			assert(
				row.size() == mapWidth &&
				"Map rows have different widths"
			);
		}
	}
	//모든칸을 순회 하면서 현재 칸에 맞는 문자에 맞는 오브젝트로 치환
	void DoomMap::BuildMap()
	{
		for (size_t z = 0; z < rows.size(); ++z)
		{
			for (size_t x = 0; x < rows[z].size(); ++x)
			{
				const char tile = rows[z][x];

				//맵을 기준으로 월드 좌표 생성
				//이렇게 안하면 만약에 맵을 다른 좌표로 생성했을때
				//생성되는 오브젝트들이 다른 곳에 생성될 수 있음.
				const Vector3 worldPosition =
					this->GetWorldPosition() +
					Vector3(static_cast<float>(x), 0.0f, static_cast<float>(z));


				//순회 하면서 등록된 딕셔너리의 키를 호출해 빌드한다.
				tileBuilderRegistry.FindBuilder(tile).Build(*this, worldPosition);

			}
		}
	}

	//TODO: 9.7일 지금은 내가 직접 등록하는 방식이지만, 이후로는
	// RTTI를 통해 클래스의 인스턴스가 생성될때 자기 자신이 직접 RegisterBuilder
	//를 통해 자신을 등록하게 만들것.
	void DoomMap::RegisterTileBuilders()
	{
		tileBuilderRegistry.RegisterBuilder('#', std::make_unique<WallTileBuilder>());
		tileBuilderRegistry.RegisterBuilder('.',std::make_unique<EmptyTileBuilder>());
		tileBuilderRegistry.RegisterBuilder('P', std::make_unique<PlayerSpawnTileBuilder>());
		tileBuilderRegistry.RegisterBuilder('M', std::make_unique<MonsterSpawnTileBuilder>());
	}

	void DoomMap::BroadcastOnMapBuilt() //콜백 호출
	{
		for (const auto& callback : onMapBuiltCallbacks)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}

	}

}

