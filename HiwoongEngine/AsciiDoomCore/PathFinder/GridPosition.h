#pragma once

namespace Hiwoong
{
	struct GridPosition
	{
		int row = 0;
		int column = 0;

		GridPosition(int row, int column) :row(row), column(column)
		{
		}


		bool operator==(const GridPosition& other) const
		{
			return row == other.row &&
				column == other.column;
		}

		bool operator!=(const GridPosition& other) const
		{
			return row != other.row ||
				column != other.column;
		}
	};


}