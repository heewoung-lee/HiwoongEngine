#include "Engine.h"
#include "Scene/Scene.h"
#include <assert.h>
#include <windows.h>
#include <stdint.h>
#include <iostream>

namespace Hiwoong
{
	//SingleTon
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
		//We need a count which can count frequency until a second
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		//We need a count which Total Windows time Count
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		int64_t currentTime = counter.QuadPart;
		int64_t previousTime = currentTime;


		float oneFrameTime = 1.0f / setting.frameRate;

		while (isQuit == false)
		{
			//Precess Input
			ProcessInput();

			//Check Current CounterTime
			QueryPerformanceCounter(&counter);

			//Save Current Time
			currentTime = counter.QuadPart;

			//calculate deltaTime;
			double deltaTime = static_cast<double>(currentTime - previousTime) / static_cast<double>(frequency.QuadPart);
			if (deltaTime < oneFrameTime) continue;
			
			//level initialize if you request add Obejct the lastframe adds requested a object and nextframe it will be spawned in Level.
			SceneInitialize();

			//Initialze Actor
			Start();

			//Actor update
			Update(deltaTime);

			// Save updated result
			Draw();

			//Transition Level
			if (nextScene != nullptr)
			{
				if (mainScene != nullptr)
				{
					mainScene.reset();
				}

				// changine owner ship next to main.
				mainScene = std::move(nextScene);

				//organizing 
				nextScene.reset();
			}

			//Orginazing Scene If you add any object or delete it starts this frame
			if (mainScene != nullptr)
			{
				mainScene->ProcessAddAndDestoryActors();
			}

			//Save calculated Input to the previous frame
			SavePreviousInputState();

			previousTime = currentTime;
		}

	}
	void Engine::Quit()
	{
		isQuit = true;
	}
	void Engine::ProcessInput()
	{
	}
	void Engine::SceneInitialize()
	{
		if (mainScene == nullptr)
		{
			return;
		}
		mainScene->SceneInitialize();

	}
	void Engine::Start()
	{
		if (mainScene == nullptr)
		{
			return;
		}
		mainScene->SceneInitialize();
	}

	void Engine::Update(double deltaTime)
	{
		//std::cout << 1/deltaTime << std::endl;
		if (mainScene == nullptr)
		{
			return;
		}
		mainScene->Update(deltaTime);

	}

	void Engine::Draw()
	{
		if (mainScene == nullptr)
		{
			return;
		}
		mainScene->Draw();

	}

	void Engine::SavePreviousInputState()
	{
	}

	void Engine::ShutDown()
	{
	}


	Engine& Engine::Get()
	{
		assert(instance != nullptr);

		return *instance;
	}
	
	
}