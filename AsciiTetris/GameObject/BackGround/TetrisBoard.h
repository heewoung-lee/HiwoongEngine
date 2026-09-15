#pragma once
#include "Math/Vector2.h"
#include "GameObject/TetrisBlocks/Block.h"
#include <vector>
#include <memory>
namespace Hiwoong
{
	class TetrisBoard
	{
	public:
		TetrisBoard(int width, int height);

		bool IsOccupied(const Vector2& position) const;

		void CheckFillRow();

		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

		void SetWall(const Vector2& position);

		void SetBlock(
			const Vector2& position,
			const std::shared_ptr<Block>& block);

		void ClearRowAndCollapse(int row);

	private :
		int width;
		int height;
		std::vector<bool> cells;
		std::vector<std::weak_ptr<Block>> blockCells;
		void SetOccupied(const Vector2& position, bool occupied);

	};

}