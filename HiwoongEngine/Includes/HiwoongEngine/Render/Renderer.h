#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <iostream>
#include <vector>
namespace Hiwoong
{
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
		
	public:
		Renderer();
		~Renderer();

		//GameObject request rendercommand to Renderer with SummitMethod

		void Submit(
			const std::string& value,
			const Vector2& position,
			Color color,
			int sortingorder);

		// Draw Event Method (it will be called by engine)
		void Draw();

		// static access method
		static Renderer& Get();


	private:
		// when engine starts the frame, it starts remove the screen
		void Clear();

		//DrawRenderQueue
		void DrawRenderQueue();

		//TODO: when we use the Twice Buffer Backfuffer draws next frame;
		void Present();


	private:

		// Singletn
		static Renderer* instance;

		// Colleting Datas which drawing Datas in this frame
		std::vector<RenderCommand> renderQueue;

	};
}

