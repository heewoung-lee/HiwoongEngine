#pragma once
#include "Core/Core.h"
#include <memory>

namespace Hiwoong
{
	// This class preovides any Type of GameObject 
	// Can check Type in Runtime
	class Hiwoong_API HiwoongObject
	{
	public:
		// ID reutrn
		virtual size_t GetType() const = 0;

		//Compare current Type and any Type
		virtual bool Is(size_t id) const
		{
			return false;
		}

		// question type
		template<typename T>
		bool IsTypeOf() const
		{
			return Is(T::TypeId());
		}
		
		//changing
		template<typename T, typename U>
		std::shared_ptr<T> Cast(const std::shared_ptr<U>& object)
		{
			//exception
			if (!object)
			{
				return nullptr;
			}
			if (object->Is(T::TypeId()))
			{
				return std::static_pointer_cast<T>(object);
			}
			return nullptr;
		}
	};
}

// Macro
#define TYPE_DECALRATIONS(Type, ParentType)								\
using super = ParentType;												\
protected:																\
	static size_t TypeIdClass()											\
	{																	\
		static int runtimeTypeId = 0;									\
		return reinterpret_cast<size_t>(&runtimeTypeId);				\
	}																	\
public:																	\
	static size_t TypeId()												\
	{																	\
		return Type::TypeIdClass();										\
	}																	\
	virtual size_t GetType() const override								\
	{																	\
		return Type::TypeIdClass();										\
	}																	\
	virtual bool Is(size_t id) const override							\
	{																	\
		return (id == TypeIdClass()) ? true : ParentType::Is(id);		\
	}																