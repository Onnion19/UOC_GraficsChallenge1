#pragma once

#include "Core/GameManagers.h"
#include "Resources/ResourceManager.h"
#include "Utils/Handlers.h"
namespace GameObject {

	/**
	* Base class for game objects. The inheritance is not enforced but encouraged.
	* When a an Object inheriting from GameObject will get the game managers injected when construced via 
	* the GameObjectFactory.
	*/
	class GameObject {
	public:
		explicit GameObject(Core::GameManagers& manager) noexcept : gManager(manager) {}
		virtual ~GameObject() {}
	protected:
		Core::GameManagers& gManager;
	};

	// I'd like to have global access to this functionalities but singletons meh.
	// So far I'll do it static but this must be improved.
	struct GameObjectFactory
	{
		/**
		* @brief constructs a game object and injects the manager if possible
		*/
		template<typename T, typename ... Args>
		static T MakeGameObject(Args&& ... args)
		{
			assert(gManager != nullptr && "GameObjectFactory has not initialized");
			if constexpr (std::is_constructible_v<T, Core::GameManagers&, Args...>)
			{
				return T(*gManager, std::forward<Args>(args)...);
			}
			else
			{
				return T(std::forward<Args>(args)...);
			}
		}

		/**
		* @brief constructs a game object handle and injects the manager if possible
		*/
		template<typename T, typename ... Args>
		static Utils::Handle<T> MakeGameObjectHandle(Args&& ... args)
		{
			assert(gManager != nullptr && "GameObjectFactory has not initialized");
			if constexpr (std::is_constructible_v<T, Core::GameManagers&, Args...>)
			{
				return std::make_unique<T>(*gManager, std::forward<Args>(args)...);
			}
			else
			{
				return std::make_unique<T>(std::forward<Args>(args)...);
			}
		}

		static void RegisterGameManagers(Core::GameManagers& managers) {
			gManager = &managers;
		}

		static void InvalidateGameManagers() { gManager = nullptr; }
	private:
		inline static Core::GameManagers* gManager = nullptr;
	};
}