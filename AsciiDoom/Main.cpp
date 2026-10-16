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
using namespace Hiwoong;

//9.26일 의존성 주입 추가.
namespace
{
	ModelImportResult LoadFBX()
	{
		const auto modelImporter = DoomInstaller::CreateModelImporter();

		//믹사모에서 얻은 FBX모델이 180도 만큼 틀어져 있어서,
		//로드 할때에는 나의 엔진을 기준으로 모델이 정면을 보게 만들어야해서,
		//180도 만큼 보정을 더함.
		ModelImportResult result =
			modelImporter->Import("Assets/Model/MonsterLow.fbx",MathConstants::Pi);



		assert(result.success);
		if (!result.success)
			return result;

		const bool idleLoaded = modelImporter->ImportAnimation(
			result.model,
			"Assets/Model/MonsterIdle.fbx",
			"Idle"
		);

		const bool moveLoaded = modelImporter->ImportAnimation(
			result.model,
			"Assets/Model/MonsterWalk.fbx",
			"Move"
		);

		const bool attackLoaded = modelImporter->ImportAnimation(
			result.model,
			"Assets/Model/MonsterAttack.fbx",
			"Attack"
		);

		result.success = idleLoaded && moveLoaded && attackLoaded;
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
