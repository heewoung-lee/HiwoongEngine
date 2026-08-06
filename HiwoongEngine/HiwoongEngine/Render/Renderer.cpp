#include "Renderer.h"
#include <cassert>
#include <iostream>
namespace Hiwoong
{
	// static valuable initlize
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer()
	{
		assert(instance == nullptr);
		instance = this;

		 // Cousor in conole sets blinking disable
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 1;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);

	}
	Renderer::~Renderer()
	{
		instance = nullptr;

		// Cousor in conole sets blinking enable
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 1;
		info.bVisible = TRUE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

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
	Renderer& Renderer::Get()
	{
		assert(instance);
		return *instance;
	}

	void Renderer::Clear()
	{
		system("cls");

	}
	void Renderer::DrawRenderQueue()
	{
		// cycle renderqueue

		for (const RenderCommand& command : renderQueue)
		{
			//Move Position
			COORD coord;
			coord.X = static_cast<short>(command.position.x);
			coord.Y = static_cast<short>(command.position.y);


			SetConsoleCursorPosition
			(
				GetStdHandle(STD_OUTPUT_HANDLE),
				coord
			);

			//Setting Color
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				static_cast<WORD>(command.color)
			);

			//Draw
			std::cout << command.image;

			//revert color
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				static_cast<WORD>(Color::White)
			);
		}

		//Clear RenderQueue
		renderQueue.clear();
	}

	void Renderer::Present()
	{
		//TODO: twice buffer 
	}
	

}