#pragma once

#include "Core/HiwoongObject.h"

namespace Hiwoong
{

	//front decalration
	class GameObject;
	class Scene;
	
	class Hiwoong_API Component : public HiwoongObject
	{
		TYPE_DECALRATIONS(Component,HiwoongObject)


	public:
		Component();
		virtual ~Component();

		virtual void Start();
		virtual void Update(double deltaTime);
		virtual void Draw();

		virtual void OnCollision(const std::shared_ptr<GameObject>& other);

	
		inline bool HasStared() const { return hasStarted; }
		inline std::shared_ptr<GameObject> GetOwner() const { return owner.lock(); }
		inline void SetOwner(std::weak_ptr<GameObject> newOwner) { owner = newOwner; }

		std::shared_ptr<Scene> GetScene() const;


	protected:
		bool hasStarted = false;

		std::weak_ptr<GameObject> owner;
	
	};

}
