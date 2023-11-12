#pragma once

#include <memory>
#include <tuple>
namespace Utils
{

	/**
	* Safe callbacks objects are a guarantee that a listener is alive when the callback is called.
	* This is useful because the listeners does not require to explicit unregister or remove the callback
	* and it will automatically do it when they are out of scope.
	*/

	// Observer observes if the listener is still alive.
	struct ObserverCallbackObject
	{
		ObserverCallbackObject() = default;
		operator bool() const { return !objectAlive.expired(); }
	private:
		ObserverCallbackObject(std::shared_ptr<bool> object) : objectAlive(object) {}
		std::weak_ptr<bool> objectAlive;
		friend struct CallbackObjectsFactory;
	};


	// SafeCallback is attached to the listener life cycle. It will be gone when listener goes out of scope.
	struct SafeCallbackObject
	{
		SafeCallbackObject() = default;
		void Release() { objectAlive.reset(); }
	private:
		SafeCallbackObject(std::shared_ptr<bool>&& object) : objectAlive(object) {}
		std::shared_ptr<bool> objectAlive;
		friend struct CallbackObjectsFactory;
	};


	// Helper factory to generate the safe callback objects
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