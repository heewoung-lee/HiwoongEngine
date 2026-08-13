#pragma once

#include "Interface/TetrisModule.h"

namespace Hiwoong
{
    class IModule : public TetrisModule
    {
        TYPE_DECALRATIONS(IModule, TetrisModule)

    public:
        IModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };

    class LModule : public TetrisModule
    {
        TYPE_DECALRATIONS(LModule, TetrisModule)

    public:
        LModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };

    class OModule : public TetrisModule
    {
        TYPE_DECALRATIONS(OModule, TetrisModule)

    public:
        OModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };

    class TModule : public TetrisModule
    {
        TYPE_DECALRATIONS(TModule, TetrisModule)

    public:
        TModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };

    class JModule : public TetrisModule
    {
        TYPE_DECALRATIONS(JModule, TetrisModule)

    public:
        JModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };

    class SModule : public TetrisModule
    {
        TYPE_DECALRATIONS(SModule, TetrisModule)

    public:
        SModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };

    class ZModule : public TetrisModule
    {
        TYPE_DECALRATIONS(ZModule, TetrisModule)

    public:
        ZModule(const Vector2& position);

        const std::array<Vector2, 4>&
            GetBlockPosition() const override;

        Color GetColor() override;

    private:
        std::array<Vector2, 4> blockPositions;
        Color color;
    };
}