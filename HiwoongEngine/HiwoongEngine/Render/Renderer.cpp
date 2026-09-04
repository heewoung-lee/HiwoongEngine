#include "Renderer.h"
#include "ScreenBuffer.h"
#include "Math/Vector2.h"
#include <cassert>
#include <Windows.h>
#include <iostream>
#include <limits>
#include "SoftwareRasterizer.h"


namespace Hiwoong
{

	Renderer::Frame::Frame(const Vector2& screenSize) : screenSize(screenSize)
	{

		//Create 2d Character Object Array and sorting order objects 
		
		const int bufferCount = screenSize.x * screenSize.y;

		charInfoArray = std::make_unique<CHAR_INFO[]>(bufferCount);
		sortingOrderArray = std::make_unique<int[]>(bufferCount);
		depthBufferArray = std::make_unique<float[]>(bufferCount);
	}

	Renderer::Frame::~Frame()
	{
	}

	void Renderer::Frame::Clear()
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

				// Find Minimum so initized infinite or largeNumber
				depthBufferArray[index] = std::numeric_limits<float>::infinity();
			}
		}

	}
	void Renderer::Frame::SetCharacter(const Vector2& position, char character, Color color, int sortingOrder)
	{

		if (position.x < 0 || position.x >= screenSize.x ||
			position.y < 0 || position.y >= screenSize.y)
		{
			return;
		}

		//Check the positions range in the screen
		const int index =
			position.y * screenSize.x + position.x;


		//if the object which top other object
		//below objects do not draw  
		if (sortingOrderArray[index] > sortingOrder)
		{
			return;
		}

		//Save data
		CHAR_INFO& info = charInfoArray[index];

		info.Char.AsciiChar = character;
		info.Attributes = static_cast<WORD>(color);

		sortingOrderArray[index] = sortingOrder;

	}

	void Renderer::Frame::SetCharacter3D(const Vector2& position, char character, Color color, float depth, int sortingOrder)
	{
		if (position.x < 0 || position.x >= screenSize.x ||
			position.y < 0 || position.y >= screenSize.y)
		{
			return;
		}

		//Check the positions range in the screen
		const int index =
			position.y * screenSize.x + position.x;


		if (sortingOrderArray[index] > sortingOrder)
		{
			return;
		}

		//if the equal layer each other comparer who is the deeper
		if (sortingOrderArray[index] == sortingOrder &&
			depthBufferArray[index] <= depth)
		{
			return;
		}

		//Save data
		CHAR_INFO& info = charInfoArray[index];

		info.Char.AsciiChar = character;
		info.Attributes = static_cast<WORD>(color);

		sortingOrderArray[index] = sortingOrder;
		depthBufferArray[index] = depth;

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

	void Renderer::SubmitPoint3D(const Vector2& position, float depth, char character, Color color, int sortingOrder)
	{
		PointRenderCommand command;

		command.position = position;
		command.depth = depth;
		command.character = character;
		command.color = color;
		command.sortingOrder = sortingOrder;

		pointRenderQueue.emplace_back(command);
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
	//Save lineRenderer to draw 
	void Renderer::SubmitLine(
		const Vector2& start,
		const Vector2& end,
		char character,
		Color color,
		int sortingOrder)
	{
		LineRenderCommand command;

		command.start = start;
		command.end = end;
		command.character = character;
		command.color = color;
		command.sortingOrder = sortingOrder;

		lineRenderQueue.emplace_back(command);
	}
	Renderer& Renderer::Get()
	{
		assert(instance);
		return *instance;
	}

	void Renderer::Clear()
	{
		// frame init
		frame->Clear();

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

				frame->SetCharacter(
					Vector2(x, command.position.y),
					command.image[sourceIndex],
					command.color,
					command.sortingorder
				);
			}
		}

		//Processing LineRenderer
		for (const LineRenderCommand& command : lineRenderQueue)
		{
			const std::vector<Vector2> points =
				SoftwareRasterizer::RasterizeLine(
					command.start,
					command.end
				);

			for (const Vector2& point : points)
			{
				frame->SetCharacter(
					point,
					command.character,
					command.color,
					command.sortingOrder
				);
			}
		}

		//Processing PointRenderer
		for (const PointRenderCommand& command : pointRenderQueue)
		{
			frame->SetCharacter3D(
				command.position,
				command.character,
				command.color,
				command.depth,
				command.sortingOrder
			);
		}

		//send current Backbuffer
		GetCurrentBuffer()->Draw(frame->charInfoArray.get());

		//Clear RenderQueue
		renderQueue.clear();
		lineRenderQueue.clear();
		pointRenderQueue.clear();

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

		std::unique_ptr<Frame> newframe = std::make_unique<Frame>(newScreenSize);
		newframe->Clear();


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
