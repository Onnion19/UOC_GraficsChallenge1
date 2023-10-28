#pragma once

#include <memory>
#include <tuple>
namespace Utils
{

	struct ObserverCallbackObject
	{

		operator bool() const { return !objectAlive.expired(); }
	private:
		ObserverCallbackObject(std::shared_ptr<bool> object) : objectAlive(object) {}
		std::weak_ptr<bool> objectAlive;
		friend struct CallbackObjectsFactory;
	};


	struct SafeCallbackObject
	{
		SafeCallbackObject() = default;
		void Release() { objectAlive.reset(); }
	private:
		SafeCallbackObject(std::shared_ptr<bool>&& object) : objectAlive(object) {}
		std::shared_ptr<bool> objectAlive;
		friend struct CallbackObjectsFactory;
	};

	struct CallbackObjectsFactory {

		static SafeCallbackObject MakeSafeCallbackObject()
		{
			SafeCallbackObject object{ std::make_shared<bool>(new bool()) };
			return object;
		}

		static ObserverCallbackObject MakeObserverCallback(const SafeCallbackObject& safeObject)
		{
			return { safeObject.objectAlive };
		}

	};
}