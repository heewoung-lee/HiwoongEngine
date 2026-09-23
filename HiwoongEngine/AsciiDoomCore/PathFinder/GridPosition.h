#pragma once

namespace Hiwoong
{
	struct GridPosition
	{
		int row = 0;
		int column = 0;

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