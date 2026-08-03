#include "Engine.h"
#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <cassert>

namespace Hiwoong
{
	Engine* Engine::instance = nullptr;
	Engine::Engine()
	{
		assert(instance == nullptr);
		instance = this;
	}
	Engine::~Engine()
	{
		instance = nullptr;
	}
	void Engine::Run()
	{
		//We use the Timer which windows serves.
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		//Check CurrentTime.
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		// Create Int for frame calculrating
		int64_t currentTime = counter.QuadPart;
		int64_t previousTime = currentTime;

		//Fixed Frame
		float oneFrameTime = 1.0f / setting.framerate;

		while (isQuit == false)
		{
			// Input state
			ProcessInput();

			// Check currentTime.
			QueryPerformanceCounter(&counter);

			// Save CurrentTime
			currentTime = counter.QuadPart;

			//Calculating frame time
			float deltaTime = static_cast<float>(currentTime - previousTime)
				/ static_cast<float>(frequency.QuadPart);

			// fixed frame
			// Update checks deltaTime which Diffetent Time Between currentTime and previousTime
			// deltatime exceeds oneFrameTime excuces the loop and updates previousTime;
			if (deltaTime < oneFrameTime) continue;

			//Scene Initialize
			SceneInitialize();

			//Object Initialize
			ObjectInitialize();

			//Update
			Update(deltaTime);

			//Draw
			Draw();

			// Save Previous Input state
			SavePreviousInput();

			// Save Previous FrameTime
			previousTime = currentTime;
		}

		ShutDown();
	}
	void Engine::Quit()
	{
		//Setting Quit Flag
		isQuit = true;

	}
	Engine& Engine::Get()
	{
		assert(instance != nullptr);

		return *instance;
	}
	void Engine::ProcessInput()
	{
	}
	void Engine::SceneInitialize()
	{
	}

	void Engine::ObjectInitialize()
	{
	}

	void Engine::Update(float deltaTime)
	{
		std::cout << "DeltaTime : " << (1.0f / deltaTime) << std::endl;

	}
	void Engine::Draw()
	{
	}
	void Engine::SavePreviousInput()
	{
	}
	void Engine::ShutDown()
	{
	}
}