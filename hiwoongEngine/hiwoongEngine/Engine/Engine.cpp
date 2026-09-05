#include "Engine/Engine.h"
#include "Scene/Scene.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Util/Util.h"
#include <assert.h>
#include <windows.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Hiwoong
{
	//SingleTon
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		assert(instance == nullptr);
		instance = this;

		//Load Engine Setting Value
		LoadEngineSetting();

		//etting owner and create Input instance
		input = std::make_unique<Input>();

		renderer = std::make_unique<Renderer>(Vector2(setting.width,setting.height));
		Util::SetRandomSeed();

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

		double fpsElapsed = 0.0; //프레임 사이에 흐른 시간 누적
		int fpsFrameCount = 0; //그동안 처리한 프레임 수 


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
			
			
			ShowCurrentFPS(deltaTime, fpsElapsed, fpsFrameCount);

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
	float Engine::GetScreenAspectRatio() const
	{
		const Vector2 screenSize = Renderer::Get().GetScreenSize();
		const Vector2 characterSize = Renderer::Get().GetCharacterSize();

		float characterRatio = setting.characterAspectRatio;

		if (characterSize.x > 0 && characterSize.y > 0)
		{
			characterRatio =
				static_cast<float>(characterSize.x) / characterSize.y;
		}

		return static_cast<float>(screenSize.x) / screenSize.y
			* characterRatio;
	}
	void Engine::ProcessInput()
	{
		assert(input != nullptr);

		input->ProcessInput();
	}
	void Engine::SceneInitialize()
	{
		if (mainScene == nullptr)
		{
			return;
		}

		if (mainScene->HasInitialized()) return;

		mainScene->SceneInitialize();

		Vector2 screenSize = mainScene->GetScreenSize();

		Renderer::Get().Resize(screenSize);

	}
	void Engine::Start()
	{
		if (mainScene == nullptr)
		{
			return;
		}
		mainScene-> Start();
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

		//Scene's GameObjects request to draw to the renderQueue
		mainScene->Draw();
		
		//RenderQueue draws GameObjects in thier storigies
		renderer->Draw();
	}

	void Engine::SavePreviousInputState()
	{
		//Excuse PrecessInput of Input
		assert(input != nullptr);
		
		input->SavePreviousInput();
	}

	void Engine::ShutDown()
	{

	}

	void Engine::LoadEngineSetting()
	{

		// load setting file
		std::ifstream file("../Config/Setting.txt");

		// check
		assert(file.is_open());

		// read line by line
		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#')
			{
				continue;
			}

			// key == value
			const size_t equalPosition = line.find('=');

			// = character check
			assert(equalPosition != std::string::npos);

			//remove empty space
			auto trim = [](std::string& s)
				{
					// ' ' :space
					// \t : tap
					// \n : enter
					const char* whiteSpace = " \t\r\n";

					const size_t begin = s.find_first_not_of(whiteSpace);

					if (begin == std::string::npos)
					{
						s.clear();
						return;
					}
					const size_t end = s.find_last_not_of(whiteSpace);
					
					s = s.substr(begin, end - begin + 1);
				};

			std::string key = line.substr(0, equalPosition);
			std::string value = line.substr(equalPosition + 1);

			trim(key);
			trim(value);

			assert(!key.empty() && !value.empty());

			if (key == "framerate")
			{
				setting.frameRate = static_cast<float>(atof(value.c_str()));
				assert(setting.frameRate > 0.0f);
				continue;
			}

			if (key == "width")
			{
				setting.width = static_cast<int>(atoi(value.c_str()));
				assert(setting.width > 0);
				continue;
			}

			if (key == "height")
			{
				setting.height = static_cast<int>(atoi(value.c_str()));
				assert(setting.height > 0);
			}

			//임시 값 
			if (key == "characterAspectRatio")
			{
				setting.characterAspectRatio =
					static_cast<float>(atof(value.c_str()));

				assert(setting.characterAspectRatio > 0.0f);
				continue;
			}

		}

		//close the file
		file.close();
	}

	void Engine::ShowCurrentFPS(double deltaTime, double& fpsElapsed, int& fpsFrameCount)
	{
		fpsElapsed += deltaTime;
		++fpsFrameCount;

		if (fpsElapsed >= 1.0)
		{
			double averageFps = fpsFrameCount / fpsElapsed; //초당 프레임수
			double averageFrameMs = fpsElapsed * 1000.0 / fpsFrameCount; //프레임 사이의 평균시간.

			std::ostringstream message;
			message << "FPS: " << averageFps
				<< " | Frame: " << averageFrameMs << " ms\n";

			OutputDebugStringA(message.str().c_str()); // 비주얼 스튜디오로 찍는다. 콘솔은 렌더링이라 못찍ㅇ,ㅁ

			fpsElapsed = 0.0;
			fpsFrameCount = 0;
		}
	}


	Engine& Engine::Get()
	{
		assert(instance != nullptr);

		return *instance;
	}
	
	
}