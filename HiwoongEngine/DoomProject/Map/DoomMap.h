#pragma once
#include "GameObject/GameObject.h"
#include "TileBuilderRegistry.h"
#include "Render/Mesh.h"
#include <string>
#include <vector>
namespace Hiwoong
{
    class DoomMap : public GameObject
    {
        TYPE_DECALRATIONS(DoomMap, GameObject)

    public:
        DoomMap(const std::string& mapPath);
        ~DoomMap() override = default;


        inline bool HasPlayerSpawnPosition() const { return hasPlayerSpawnPosition; }

        //Mesh에 접근 할수 있는 함수 이걸로, 형상 추가를 할 수 있음.
        Mesh& GetMapMesh();

        //플레이어 스폰 위치 설정
        void SetPlayerSpawnPosition(const Vector3& position);
        const Vector3& GetPlayerSpawnPosition() const;

        //몬스터 스폰위치 설정
        void AddMonsterSpawnPosition(const Vector3& position);

        inline const std::vector<Vector3>& GetMonsterSpawnPositions() const
        {
            return monsterSpawnPositions;
        }

        void Start() override;

    private:
        std::string mapPath;
        std::vector<std::string> rows;
        //값으로 두는 이유는 DoomMap이 사라질때 Builder도 같이 사라져야함.
        TileBuilderRegistry tileBuilderRegistry;
        
        Vector3 playerSpawnPosition = Vector3::Zero;
        bool hasPlayerSpawnPosition = false;
        
        std::vector<Vector3> monsterSpawnPositions;

        Mesh mapMesh;

        void LoadMap();
        void BuildMap();
        void RegisterTileBuilders();
    };
}
