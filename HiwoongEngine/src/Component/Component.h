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


		//GetComponent를 기존에 쓰려면 오너를 가져와서 그 오너의 컴포넌트를 뒤져야했는데
		//이게 너무 불편하니. 차라리 줄여쓰는게 좋을꺼라 생각해서 추가.
		template<typename T>
		std::shared_ptr<T> GetComponent() const;


	protected:
		bool hasStarted = false;

		std::weak_ptr<GameObject> owner;
	
	};

}
