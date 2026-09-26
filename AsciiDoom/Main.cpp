#include "Engine/Engine.h"
#include "Scene/DoomScene.h"
#include "Core/Input.h"
#include "Render/WindowRenderOutput.h"
#include "Bootstrap/DoomInstaller.h"
#include <utility>
using namespace Hiwoong;

//9.26일 의존성 주입 추가.
namespace
{
	void DependencyInjection(Engine& engine)
	{
		const std::shared_ptr<const IPathFinder> pathFinder =
			DoomInstaller::CreatePathFinder();

		engine.AddNewScene<DoomScene>(pathFinder);
	}
}

int main()
{
	auto output =
		std::make_unique<WindowRenderOutput>(Vector2(4, 8));

	const HWND gameWindow = output->GetWindowHandle();

	Engine engine(std::move(output));

	Input::Get().SetMouseWindow(gameWindow);
	Input::Get().SetMouseLocked(true);
	DependencyInjection(engine);
	engine.Run();

	return 0;
}
