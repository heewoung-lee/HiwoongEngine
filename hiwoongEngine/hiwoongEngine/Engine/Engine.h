#pragma once

namespace Hiwoong
{
	class Engine
	{
		//Setting Engine Structure
		struct EngineSetting
		{
			float framerate = 120.0f;
		};

	public:
		Engine();
		virtual ~Engine();

		// Game Loop Method
		void Run();

		// Game Quit Method
		void Quit();

		// acesses singleton
		static Engine& Get();

	protected:
		// Processing Input Method.
		void ProcessInput();

		// Level Initialize Method.
		void SceneInitialize();

		// Object initialize Method.
		void ObjectInitialize();

		// Update Method.
		void Update(float deltaTime);

		// Draw Method.
		void Draw();

		// Saving previous Input Method
		void SavePreviousInput();

		// Organizing Method after Engine Quit
		void ShutDown();


	protected:
		//EngineSetting
		EngineSetting setting;

		//Engine Quit Flag
		bool isQuit = false;

		//Singleton 
		static Engine* instance;
	};



}
