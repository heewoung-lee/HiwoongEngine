#include "Engine/Engine.h"
#include "Scene/TestScene.h"
using namespace Hiwoong;

int main()
{
	Engine engine;
	engine.AddNewScene<TestScene>();
	engine.Run();
}