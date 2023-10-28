#pragma once

#include <memory>
#include <tuple>
namespace Utils
{

	struct ObserverCallbackObject
	{
		ObserverCallbackObject() = default;
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

		static std::tuple<SafeCallbackObject, ObserverCallbackObject> MakeCallbacks()
		{
			auto safeCallback = MakeSafeCallbackObject();
			auto observerCallback = MakeObserverCallback(safeCallback);
			return { std::move(safeCallback) , std::move(observerCallback) };
		}

	};
}