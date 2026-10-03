#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "Render/Renderer.h"
#include "GameObject/GameObject.h"
#include <sstream>
#include <vector>
#include <string>
namespace Hiwoong
{
	SpriteRendererComponent::SpriteRendererComponent(
		const SpriteFrame& frame,
		int sortingOrder)
		: image(frame),
		sortingOrder(sortingOrder)
	{
	}

	//가로중 가장 큰길이 사용
	int SpriteRendererComponent::GetMaxWidth() const
	{
		std::size_t width = 0;

		for (const std::vector<SpriteCell>& row : image.cells)
		{
			if (row.size() > width)
			{
				width = row.size();
			}
		}

		return static_cast<int>(width);
	}


	void SpriteRendererComponent::Draw()
	{
		super::Draw();

		std::shared_ptr<GameObject> gameObject = GetOwner();

		if (gameObject == nullptr || gameObject->IsActive() == false) return;

		std::shared_ptr<TransformComponent> transform = gameObject->GetComponent<TransformComponent>();
		if (transform == nullptr) return;


		Vector3 position = transform->GetWorldPosition();

		//9,10 일 여러줄을 읽을 수 있게 수정
		//그리고 ' '빈 공백의 문자가 있을때는 렌더링하지 않고 인덱스는 유지하는방향으로 수정
		for (std::size_t row = 0; row < image.cells.size(); ++row)
		{
			const std::vector<SpriteCell>& cellRow = image.cells[row];

			for (std::size_t column = 0; column < cellRow.size(); ++column)
			{
				const SpriteCell& cell = cellRow[column];

				if (cell.character == ' ') continue;

				Renderer::Get().Submit(
					std::string(1, cell.character),
					Vector2(
						position.x + static_cast<float>(column),
						position.y + static_cast<float>(row)
					),
					cell.color,
					sortingOrder
				);
			}
		}



		//Renderer::Get().Submit()
	}
}