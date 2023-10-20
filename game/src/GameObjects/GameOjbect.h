#pragma once

#include "Core/Game.h"
namespace GameObject {

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

	private:
		inline static Core::GameManagers* gManager = nullptr;
		// Only the game class can initialize it. It's a little of protection here.
		friend Core::Game;
	};
}