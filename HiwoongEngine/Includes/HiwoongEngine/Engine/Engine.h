#pragma once
#include "Core/Core.h"
#include <memory>

namespace Hiwoong
{
	//Forward decaration
	class Scene;
	class Input;
	class Renderer;

	class Hiwoong_API Engine
	{
		struct EngineSetting
		{
			float frameRate = 0.0f;

			//Screen Width
			int width = 0;

			//Screen height
			int height = 0;

			//콘솔마다 문자들의 높이와 너비가 다르므로 그 비율을 맞추기위한 비율
			float characterAspectRatio = 0.54f;
		};

	public:
		Engine();
		virtual ~Engine();

		void Run();

		void Quit();


		//request add Level
		template<
			typename T,
			typename... Args,
			typename = std::enable_if_t<
			std::is_base_of<Scene, T>::value>>
			void AddNewScene(Args&&... args)
		{
			nextScene = std::make_shared<T>(
				std::forward<Args>(args)...
			);
		}


		static Engine& Get();

		//Getter
		inline int GetWidth() const { return setting.width; }
		inline int GetHeight() const { return setting.height; }

		//문자에 따른 콘솔화면 비율
		float GetScreenAspectRatio() const;
	protected:
		// Process input
		void ProcessInput();

		//Initilize Scene
		void SceneInitialize();

		//Intialize Object
		void Start();
		 //Update frame
		void Update(double deltaTime);
		 //Renderfing
		void Draw();
		//Save Previous input state and compare to current
		void SavePreviousInputState();

		//ShotDown
		void ShutDown();

		

		//Load EngineSetting Method
		void LoadEngineSetting();

	protected:
		//Setting Engine
		EngineSetting setting;


		//Flag for Engine quit
		bool isQuit = false;

		//Singleton Engine
		static Engine* instance;


		// Main Level
		std::shared_ptr<Scene> mainScene;

		std::shared_ptr<Scene> nextScene;
		
		std::unique_ptr<Input> input;

		std::unique_ptr<Renderer> renderer;

	
	private:
		void ShowCurrentFPS(double deltaTime, double& fpsElapsed, int& fpsFrameCount);

	};
}