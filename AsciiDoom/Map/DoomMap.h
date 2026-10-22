#pragma once
#include "GameObject/GameObject.h"
#include "TileBuilderRegistry.h"
#include "Render/Mesh.h"
#include "Math/Vector3.h"
#include "Navigation/INavigationMap.h"
#include "Render/SpotLight.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>


namespace Hiwoong
{
    struct ModelData;

    class DoomMap : public GameObject, public INavigationMap
    {

        TYPE_DECALRATIONS(DoomMap, GameObject)

    public:
        void AddOnMapBuilt(std::function<void()> callback);

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

        bool IsWallTile(int x, int z) const;

        //벽이나 장애물이 있다면 플레이어에게 이동하지말라고 말해야함.
        bool CanMoveTo(
            const Vector3& worldPosition,
            float halfSize
        ) const;

        const NavigationGrid& GetNavigationGrid() const override;

        GridPosition WorldToGrid(
            const Vector3& worldPosition
        ) const override;

        Vector3 GridToWorld(
            const GridPosition& gridPosition
        ) const override;

        int GetWallLayers() const { return wallLayers; }
        float GetFloorHeight() const { return floorHeight; }

        void AddSpotLight(const SpotLight& light);

        const std::vector<SpotLight>& GetSpotLights() const
        {
            return spotLights;
        }

    private:
        void LoadMap();
        void RegisterTileBuilders();
        void BroadcastOnMapBuilt();
        void InitNavigationGrid();

        void BuildMap();
        void BuildFloor();

        bool InitTexturedMeshRenderer(
            const Mesh& mesh,
            const std::string& texturePath
        );

        std::shared_ptr<const ModelData> LoadPropModel(
            const std::string& filePath
        );
        
        //오브젝트를 등록하는 메서드
        void RegisterCrate();
        void RegisterBarrel();
        void RegisterColumn();
        void RegisterBeam();
        void RegisterCeilingLight();


    private:
        std::string mapPath;
        std::vector<std::string> rows;
        //값으로 두는 이유는 DoomMap이 사라질때 Builder도 같이 사라져야함.
        TileBuilderRegistry tileBuilderRegistry;

        Vector3 playerSpawnPosition = Vector3::Zero;
        bool hasPlayerSpawnPosition = false;

        std::vector<Vector3> monsterSpawnPositions;

        Mesh mapMesh;
        Mesh floorMesh;

        //맵의 콜백함수들을 모은다.
        std::vector<std::function<void()>> onMapBuiltCallbacks;

        //각 칸에 이동 가능 여부를 보관
        std::unique_ptr<NavigationGrid> navigationGrid;

        std::string floorTexturePath;
        std::string wallTexturePath;


        int wallLayers = 1;
        float floorHeight = 0.5f;
        std::vector<SpotLight> spotLights;
    };
}
