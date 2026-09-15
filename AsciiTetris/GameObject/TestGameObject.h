#pragma once

#include "GameObject/GameObject.h"
#include "Manager/TetrisGameState.h"
class TestGameObject : public Hiwoong::GameObject
{
	TYPE_DECALRATIONS(TestGameObject,GameObject)

public:
	TestGameObject();
	~TestGameObject() = default;
	
	virtual void Start();

};

