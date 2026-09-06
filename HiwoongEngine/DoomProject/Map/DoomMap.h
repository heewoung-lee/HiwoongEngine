#pragma once
#include "GameObject/GameObject.h"
namespace Hiwoong
{
    class DoomMap : public GameObject
    {
        TYPE_DECALRATIONS(DoomMap, GameObject)

    public:
        DoomMap(const std::string& mapPath);
        ~DoomMap() override = default;

        void Start() override;

    private:
        std::string mapPath;
        std::vector<std::string> rows;
    };
}
