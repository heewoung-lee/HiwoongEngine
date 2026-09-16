#pragma once

#include "GameObject/GameObject.h"
#include "Math/Vector2.h"
#include <vector>
namespace Hiwoong
{
	class BigTextDisplayComponent : public Component
	{
		TYPE_DECALRATIONS(BigTextDisplayComponent,Component)

	public:
		explicit BigTextDisplayComponent(
			const std::string& text, int scale);

		void Start() override;
		void SetText(const std::string& text);
		
		//텍스트가 얼마나 크기를 차지하는지 구하는함수
		//이걸로 크기를 계산해 간격을 띄울 수 있음.
		Vector2 GetTextSize() const;

	private:
		std::vector<std::string> BuildImage() const;


	private:
		std::string text;
		int scale;
	};


}

