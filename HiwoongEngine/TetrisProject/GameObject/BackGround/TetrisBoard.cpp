#include "TetrisBoard.h"
#include "Manager/TetrisGameState.h"
#include <cassert>


namespace Hiwoong
{
	TetrisBoard::TetrisBoard(int width, int height) : width(width), height(height), cells(width* height, false), blockCells(width*height)
	{
		assert(width > 0);
		assert(height > 0);
	}
	bool TetrisBoard::IsOccupied(const Vector2& position) const
	{

		//wall is true
		if (position.x < 0 || position.x >= width) return true;

		if (position.y < 0 || position.y >= height) return true;

		const int idx = position.y * width + position.x;

		return cells[idx];
	}


	void TetrisBoard::SetOccupied(
		const Vector2& position,
		bool occupied)
	{
		if (position.x < 0 || position.x >= width)
			return;

		if (position.y < 0 || position.y >= height)
			return;

		const int idx = position.y * width + position.x;

		cells[idx] = occupied;
	}


	void TetrisBoard::CheckFillRow()
	{
		//except top and bottom
		for (int r = 1; r < height-1; ++r)
		{
			bool isbingo = true;
			//except left and right
			for (int c = 1; c < width-1;++c)
			{
				if (cells[r * width + c] == false)
				{
					isbingo = false;
					break;
				}
			}
			if (isbingo == true)
			{
				ClearRowAndCollapse(r);
				TetrisGameState::Get().AddScore(10);
			}

		}
	}
	void TetrisBoard::SetWall(const Vector2& position)
	{
		SetOccupied(position, true);
	}
	void TetrisBoard::SetBlock(const Vector2& position, const std::shared_ptr<Block>& block)
	{
		int idx = position.y * width + position.x;

		cells[idx] = true;
		blockCells[idx] = block;

		SetOccupied(block->GetWorldPosition(),true);

	}

	void TetrisBoard::ClearRowAndCollapse(int row)
	{

		//except left and right which are walls
		for (int c = 1; c < width-1; ++c)
		{
			int idx = row * width + c;

			std::shared_ptr<Block> block = blockCells[idx].lock();

			if (block != nullptr)
			{
				block->Destroy();
			}
			cells[row * width + c] = false;
			blockCells[row * width + c].reset();
		}


		//Collapse from upRow

		//Iteratr bottom-up to prevent moved blocks from being processed again.
		for (int r = row-1; r >= 1 ; --r)
		{
			for (int c = 1; c < width - 1; ++c)
			{
				int idx = r * width + c;

				std::shared_ptr<Block> block = blockCells[idx].lock();

				if (block != nullptr)
				{
					cells[idx + width] = true;
					blockCells[idx + width] = block;
					block->SetPosition(
						block->GetPosition() + Vector2(0, 1)
					);

					cells[idx] = false;
					blockCells[idx].reset();
				}
			}
		}



	}
}