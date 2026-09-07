#include "DoomMap.h"
#include "BuildImplements/WallTileBuilder.h"
#include "BuildImplements/EmptyTileBuilder.h"
#include "BuildImplements/PlayerSpawnTileBuilder.h"
#include "BuildImplements/MonsterSpawnTileBuilder.h"
#include <memory>
#include <fstream>
#include <cassert>
#include <string>

namespace Hiwoong
{
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

		auto verCnt = mapMesh.vertices.size();
		auto triCnt = mapMesh.triangles.size();

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

}

