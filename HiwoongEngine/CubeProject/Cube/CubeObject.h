#pragma once

#include "GameObject/GameObject.h"
#include "Render/Mesh.h"

namespace Hiwoong
{
	class CubeObject : public GameObject
	{
		TYPE_DECALRATIONS(CubeObject,GameObject)


	public:
		CubeObject();
		void Draw() override;

	private:
		Mesh mesh;
	};
}

