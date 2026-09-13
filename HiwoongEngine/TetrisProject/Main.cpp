#include "Engine/Engine.h"
#include "Scene/TestScene.h"
#include "Render/ConsoleRenderOutput.h"
using namespace Hiwoong;

int main()
{
	ConsoleRenderOptions options;
	options.fontName = L"µ¸¿òÃ¼";
	options.characterSize = Vector2(24, 30);

	Engine engine(
		std::make_unique<ConsoleRenderOutput>(options)
	);

	engine.AddNewScene<TestScene>();
	engine.Run();
}