#include "Engine/Engine.h"
#include "Scene/DoomScene.h"
#include "Core/Input.h"
#include "Render/WindowRenderOutput.h"
#include "Bootstrap/DoomInstaller.h"
#include "ModelImport/IModelImporter.h"
#include <cassert>
#include <utility>
using namespace Hiwoong;

//9.26일 의존성 주입 추가.
namespace
{
	ModelImportResult LoadFBX()
	{
		//10.9일 몬스터 FBX읽기
		const auto modelImporter = DoomInstaller::CreateModelImporter();

		const ModelImportResult result =
			modelImporter->Import("Assets/Model/MonsterLow.fbx");

		assert(result.success);

		return result;
	}

	void DependencyInjection(Engine& engine)
	{
		ModelImportResult monsterModelResult = LoadFBX();

		const std::shared_ptr<const ModelData> monsterModel =
			std::make_shared<ModelData>(
				std::move(monsterModelResult.model)
			);

		const auto pathFinder = DoomInstaller::CreatePathFinder();

		
		engine.AddNewScene<DoomScene>(pathFinder, monsterModel);
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
