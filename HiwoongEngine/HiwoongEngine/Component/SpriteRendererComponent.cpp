#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "Render/Renderer.h"
#include "GameObject/GameObject.h"
#include <sstream>

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

		std::shared_ptr<TransformComponent> transform = gameObject->GetComponent<TransformComponent>();
		if (transform == nullptr) return;


		//9.10일 월드 포지션에서 로컬 포지션 으로 수정함. 
		Vector3 position = transform->GetLocalPosition();

		std::istringstream stream(image);
		std::string line;
		int row = 0;


		//9,10 일 여러줄을 읽을 수 있게 수정
		//그리고 ' '빈 공백의 문자가 있을때는 렌더링하지 않고 인덱스는 유지하는방향으로 수정
		while (std::getline(stream, line))
		{
			//공백이 아닌 문자의 위치를 찾는 변수.
			std::size_t start = line.find_first_not_of(' ');

			while (start != std::string::npos)
			{
				std::size_t end = line.find(' ', start);

				if (end == std::string::npos)
					end = line.size();

				Renderer::Get().Submit(
					line.substr(start, end - start),
					Vector2(
						position.x + static_cast<float>(start),
						position.y + row
					),
					color,
					sortingOrder
				);
				start = line.find_first_not_of(' ', end);
			}
			++row;
		}



		//Renderer::Get().Submit()
	}
}