#pragma once

#include "GameObject/GameObject.h"
class TestGameObject : public Hiwoong::GameObject
{
	TYPE_DECALRATIONS(TestGameObject,GameObject)

public:
	TestGameObject();
	virtual void Update(double deltaTime) override;
};

