#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "Render/Renderer.h"
#include "GameObject/GameObject.h"
namespace Hiwoong
{
	SpriteRendererComponent::SpriteRendererComponent(
		const std::string& image, 
		Color color, 
		int sortingOrder) : image(image),color(color),sortingOrder(sortingOrder)
	{

	}
	void SpriteRendererComponent::Draw()
	{
		super::Draw();

		std::shared_ptr<GameObject> gameObject = GetOwner();

		if (gameObject == nullptr || gameObject->IsActive() == false) return;

		std::shared_ptr<TransformComponent> transform = gameObject->GetTransform();
		if (transform == nullptr) return;

		Renderer::Get().Submit(image, transform->GetWorldPosition(), color, sortingOrder);


		//Renderer::Get().Submit()
	}
}