#include "Engine/Engine.h"
#include "Scene/DoomScene.h"
using namespace Hiwoong;

int main()
{
	Engine engine;
	engine.AddNewScene<DoomScene>();
	engine.Run();

	return 0;
}
