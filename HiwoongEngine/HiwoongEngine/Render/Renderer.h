#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <iostream>
#include <vector>
#include <memory>

namespace Hiwoong
{
	class ScreenBuffer;

	//Draw to Screen 
	class Hiwoong_API Renderer
	{
		//The struct has data which draw in screen
		struct  RenderCommand
		{
			//drawing value of screen
			std::string image;
			
			//RenderPosition
			Vector2 position;

			//Color
			Color color = Color::White;

			//SortingOrder
			int sortingorder = -1;
		};
		

		//Screen Frame(2 dimention Array)
		struct Frame
		{
			// Create Size width X horizontal
			Frame(const Vector2& screenSize);
			~Frame();

			//Initialize Frame
			void Clear();

			//2d charater Array
			std::unique_ptr<CHAR_INFO[]> charInfoArray;

			// 2d Array to sort to draw
			std::unique_ptr<int[]> sortingOrderArray;


			void SetCharacter(
				const Vector2& position,
				char character,
				Color color,
				int sortingOrder
			);


		private:
			Vector2 screenSize;
		};

		//Do not draw immediately it can save line info until they rendered in screen
		struct LineRenderCommand
		{
			Vector2 start;
			Vector2 end;

			char character = '@';
			Color color = Color::White;
			int sortingOrder = 0;
		};

	public:
		Renderer(const Vector2& screenSize);
		~Renderer();

		//GameObject request rendercommand to Renderer with SummitMethod

		void Submit(
			const std::string& value,
			const Vector2& position,
			Color color,
			int sortingorder);

		// Draw Event Method (it will be called by engine)
		void Draw();

		void Resize(const Vector2& screenSize);

		//Requrie the draw the line then softwareRasterizer mathod calculated the line
		void SubmitLine(
			const Vector2& start,
			const Vector2& end,
			char character,
			Color color,
			int sortingOrder
		);

		// static access method
		static Renderer& Get();


		


	private:
		// when engine starts the frame, it starts removing the screen
		void Clear();

		//DrawRenderQueue
		void DrawRenderQueue();

		//when we use the Twice Buffer Backfuffer draws next frame;
		void Present();

		// Buffer can draw this frame
		const ScreenBuffer* const GetCurrentBuffer() const;

	private:

		// Singletn
		static Renderer* instance;

		// Colleting Datas which drawing Datas in this frame
		std::vector<RenderCommand> renderQueue;

		//Screen Size
		Vector2 screenSize;

		// organizing 2 dimention Array of drawing the characters 
		std::unique_ptr<Frame> frame;

		// screen consle buffer
		std::unique_ptr<ScreenBuffer> screenBufferArray[2] = {};

		// Back bufferIdx(draw buffer)
		int currentBufferIndex = 0;

		//LineRenderer List
		std::vector<LineRenderCommand> lineRenderQueue;

		//Create twice ScreenBuffer
		void CreateSceenBuffer(const Vector2& newScreenSize);
	};
}

