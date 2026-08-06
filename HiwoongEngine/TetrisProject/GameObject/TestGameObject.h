#pragma once

#include "GameObject/GameObject.h"
class TestGameObject : public Hiwoong::GameObject
{
public:
	TestGameObject();
	virtual void Update(double deltaTime) override;
};

