#include <memory>

namespace Hiwoong
{
	//Forward decaration
	class Scene;
	class Engine
	{
		struct EngineSetting
		{
			float frameRate = 120.0f;
		};

	public:
		Engine();
		virtual ~Engine();

		void Run();

		void Quit();


		//request add Level
		template<typename T,
			typename = std::enable_if_t<std::is_base_of<Scene, T> :: value>>

			void AddNewScene()
		{
			nextScene = std::make_shared<T>();
		}


		static Engine& Get();

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
		
	};
}