#pragma once

#include "Math/Color.h"
#include "Render/SpriteColorPalette.h"
#include <vector>
#include <string>
#include <cassert>
#include <cstddef>
//10.3일 기존에는 픽셀 정보가 글자 밖에 없었는데
//색상까지 담을 수 있는 정보를 표현하기 위해
//SpriteCell 제작
namespace Hiwoong
{
	struct SpriteCell
	{
		char character = ' ';
		Color color = Color::White;
	};

	struct SpriteFrame
	{
		SpriteFrame(
			const std::vector<std::string>& characterRows,
			const std::vector<std::string>& colorRows)
		{
			assert(characterRows.size() == colorRows.size());

			for (std::size_t row = 0; row < characterRows.size();++row)
			{

				//가시성을 위해서 문자 렌러링과 색상 렌러링 정보를 분리 해 놓음.
				//두개를 합치기 위해서는 각자의 인덱스 정보가 같아야 하므로
				//행수와 칸수를 비교해서 같으면 합치고 둘중 하나라도 다르면 에러 나와야함.
				assert(characterRows[row].size() == colorRows[row].size());

				std::vector<SpriteCell> cellRow;

				for (std::size_t column = 0; column < characterRows[row].size(); ++column)
				{
					const char colorCode = colorRows[row][column];
					const auto colorEntry = SpriteColorPalette.find(colorCode);

					assert(colorEntry != SpriteColorPalette.end());

					cellRow.push_back(SpriteCell{
						characterRows[row][column],
						colorEntry->second // 색상.
						});
				}
				cells.push_back(cellRow);
			}
		}


		std::vector<std::vector<SpriteCell>> cells;
	};
}