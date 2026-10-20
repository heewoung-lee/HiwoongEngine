#include "Engine/Engine.h"
#include "Scene/DoomScene.h"
#include "Core/Input.h"
#include "Render/WindowRenderOutput.h"
#include "Bootstrap/DoomInstaller.h"
#include "ModelImport/IModelImporter.h"
#include "Math/MathConstants.h"
#include "Scene/GameOverScene.h"
#include <cassert>
#include <utility>
#include <string>
using namespace Hiwoong;

//9.26일 의존성 주입 추가.
namespace
{
	const std::string targetModel = "Zombie";

	ModelImportResult LoadFBX()
	{
		const auto modelImporter = DoomInstaller::CreateModelImporter();
		const std::string modelFolder = "Assets/Model/" + targetModel + "/";

		ModelImportResult result =
			modelImporter->Import(modelFolder + "MonsterLow.fbx", 135.0f);


		assert(result.success);
		if (!result.success)
			return result;

		const bool idleLoaded = modelImporter->ImportAnimation(
			result.model,
			modelFolder + "MonsterIdle.fbx",
			"Idle"
		);

		const bool moveLoaded = modelImporter->ImportAnimation(
			result.model,
			modelFolder + "MonsterWalk.fbx",
			"Move"
		);

		const bool attackLoaded = modelImporter->ImportAnimation(
			result.model,
			modelFolder + "MonsterAttack.fbx",
			"Attack"
		);

		const bool deadLoaded = modelImporter->ImportAnimation(
			result.model,
			modelFolder + "MonsterDead.fbx",
			"Dead"
		);

		result.success = idleLoaded && moveLoaded && attackLoaded && deadLoaded;
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
