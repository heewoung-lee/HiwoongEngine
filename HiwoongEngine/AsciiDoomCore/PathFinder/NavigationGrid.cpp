#include "NavigationGrid.h"

namespace Hiwoong
{
    NavigationGrid::NavigationGrid(
        const std::vector<std::vector<bool>>& walkableTiles) : walkableTiles(walkableTiles)
    {
        if (walkableTiles.empty()) return;

        const std::size_t width = walkableTiles.front().size();

        if (width == 0) return;

        for (const std::vector<bool>& row : walkableTiles)
        {
            if (row.size() != width)
            {
                return;
            }
        }
        isValid = true;
    }
    std::size_t NavigationGrid::GetWidth() const
    {
        if (isValid == false)
        {
            return 0;
        }

        return walkableTiles.front().size();
    }

    std::size_t NavigationGrid::GetHeight() const
    {
        if (isValid == false)
        {
            return 0;
        }

        return walkableTiles.size();
    }
    bool NavigationGrid::IsWalkable(const GridPosition& position) const
    {
        if (IsInside(position) == false) return false;

        bool checkWalkable = walkableTiles[position.row][position.column];

        return checkWalkable;
    }

    bool NavigationGrid::IsInside(const GridPosition& position) const
    {
        if (isValid == false) return false;

        if (position.column < 0 || position.row < 0) return false;

        bool checkInside =
            static_cast<std::size_t>(position.column) < GetWidth() &&
            static_cast<std::size_t>(position.row) < GetHeight();

        return checkInside;
    }

}

