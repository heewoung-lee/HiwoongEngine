#include "Renderer.h"
#include "ScreenBuffer.h"
#include <cassert>
#include <Windows.h>
#include <iostream>

namespace Hiwoong
{

	Renderer::Frame::Frame(int bufferCount)
	{

		//Create 2d Character Object Array and sorting order objects 
		charInfoArray = std::make_unique<CHAR_INFO[]>(bufferCount);
		sortingOrderArray = std::make_unique<int[]>(bufferCount);
	}

	Renderer::Frame::~Frame()
	{
	}

	void Renderer::Frame::Clear(const Vector2& screenSize)
	{
		// Organizing Value 
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height;++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// trnslate 2d array index(x,y) -> 1d 
				const int index = (y * width) + x;

				// initialize chrater value
				CHAR_INFO& info = charInfoArray[index];
				info.Char.AsciiChar = ' ';
				info.Attributes = 0;

				// initialize draw soringorder
				sortingOrderArray[index] = -1;
			}
		}

	}

	// ----------------------FRMAE-------------------------//

	// static valuable initlize
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2& screenSize) : screenSize(screenSize)
	{
		assert(instance == nullptr);
		instance = this;

		CreateSceenBuffer(screenSize);
	}


	Renderer::~Renderer()
	{
		instance = nullptr;

		//recorver default console
		SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	void Renderer::Submit(const std::string& image, const Vector2& position, Color color, int sortingOrder)
	{
		// create commend objects
		RenderCommand command;
		command.image = image;
		command.position = position;
		command.color = color;
		command.sortingorder = sortingOrder;

		renderQueue.emplace_back(command);
	}

	void Renderer::Draw()
	{
		Clear();
		DrawRenderQueue();
		Present();
	}
	void Renderer::Resize(const Vector2& screenSize)
	{
		CreateSceenBuffer(screenSize);
	}
	Renderer& Renderer::Get()
	{
		assert(instance);
		return *instance;
	}

	void Renderer::Clear()
	{
		// frame init
		frame->Clear(screenSize);

		// console buffer init
		GetCurrentBuffer()->Clear();

	}
	void Renderer::DrawRenderQueue()
	{
		// cycle renderqueue

		for (const RenderCommand& command : renderQueue)
		{
			if (command.image.empty()) continue;

			if (command.position.y < 0 || command.position.y >= screenSize.y)continue;

			//Check character length
			const int length = static_cast<int>(command.image.length());

			// start position
			const int startX = command.position.x;

			// end position
			const int endX = startX + length - 1; 

			// continiue if x escape the screen
			if (endX < 0 || startX >= screenSize.x) continue;

			//calculate character's start position and end position to show
			const int visibleStart = startX < 0 ? 0 : startX;
			const int visibleEnd = endX >= screenSize.x ? screenSize.x - 1 : endX;


			//recode into frame
			for (int x = visibleStart; x <= visibleEnd; ++x)
			{
				//letter idx
				const int sourceIndex = x - startX;

				// 2d index to recode
				const int index = (command.position.y * screenSize.x) + x;

				// sorting order
				if (frame->sortingOrderArray[index] > command.sortingorder)
				{
					continue;
				}

				// recode 2d Array
				frame->charInfoArray[index].Char.AsciiChar = command.image[sourceIndex];
				frame->charInfoArray[index].Attributes = static_cast<WORD>(command.color);

				// recode sortingOrder
				frame->sortingOrderArray[index] = command.sortingorder;
			}
		}

		//send current Backbuffer
		GetCurrentBuffer()->Draw(frame->charInfoArray.get());

		//Clear RenderQueue
		renderQueue.clear();

		SetConsoleTextAttribute(
			GetCurrentBuffer()->GetScreenBuffer(),
			static_cast<WORD>(Color::White)
		);
	}

	void Renderer::Present()
	{
		// Current BackBuffer Enable
		SetConsoleActiveScreenBuffer(GetCurrentBuffer()->GetScreenBuffer());

		//Cycle Buffers
		currentBufferIndex = 1 - currentBufferIndex;
	}
	
	const ScreenBuffer* const Renderer::GetCurrentBuffer() const
	{
		return screenBufferArray[currentBufferIndex].get();
	}
	void Renderer::CreateSceenBuffer(const Vector2& newScreenSize)
	{
		std::unique_ptr<ScreenBuffer> newBuffer0 = std::make_unique<ScreenBuffer>(newScreenSize);
		std::unique_ptr<ScreenBuffer> newBuffer1 = std::make_unique<ScreenBuffer>(newScreenSize);

		newBuffer0->Clear();
		newBuffer1->Clear();

		const int bufferCount = newScreenSize.x * newScreenSize.y;
		std::unique_ptr<Frame> newframe = std::make_unique<Frame>(bufferCount);
		newframe->Clear(newScreenSize);


		SetConsoleActiveScreenBuffer(
			newBuffer0->GetScreenBuffer()
		);

		// move owner
		screenBufferArray[0] = std::move(newBuffer0);
		screenBufferArray[1] = std::move(newBuffer1);

		// Setting 0 consoleBuffer to show
		SetConsoleActiveScreenBuffer(screenBufferArray[0]->GetScreenBuffer());

		frame = std::move(newframe);

		screenSize = newScreenSize;
		currentBufferIndex = 0;
	}
}