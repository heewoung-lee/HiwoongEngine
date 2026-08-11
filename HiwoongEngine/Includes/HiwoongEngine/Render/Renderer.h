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
			Frame(int bufferCount);
			~Frame();

			//Initialize Frame
			void Clear(const Vector2& screenSize);

			//2d charater Array
			std::unique_ptr<CHAR_INFO[]> charInfoArray;

			// 2d Array to sort to draw
			std::unique_ptr<int[]> sortingOrderArray;

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


		// static access method
		static Renderer& Get();


		


	private:
		// when engine starts the frame, it starts removing the screen
		void Clear();

		//DrawRenderQueue
		void DrawRenderQueue();

		//TODO: when we use the Twice Buffer Backfuffer draws next frame;
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


		//Create twice ScreenBuffer
		void CreateSceenBuffer(const Vector2& newScreenSize);
	};
}

