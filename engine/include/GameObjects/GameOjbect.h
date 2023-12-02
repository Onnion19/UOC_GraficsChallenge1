#pragma once

#include "Core/GameManagers.h"
#include "Components/ComponentManager.h"
#include "Core/Tag.h"
#include "Utils/Handlers.h"
#include "Resources/ResourceManager.h"
#include "GameObjects/GameObjectID.h"
namespace GameObject {

	/**
	* Base class for game objects. The inheritance is not enforced but encouraged.
	* When a an Object inheriting from GameObject will get the game managers injected when construced via
	* the GameObjectFactory.
	*/
	class GameObject : public IdentifiedObject, public Core::TaggedObject {
	public:
		explicit GameObject(Core::GameManagers& manager) noexcept : IdentifiedObject(), gManager(manager), componentManager(manager.GetManager<Components::ComponentManager>()) {}

		template<Core::StringLike T>
		explicit GameObject(Core::GameManagers& manager, T tag) noexcept : IdentifiedObject(), Core::TaggedObject(tag), gManager(manager) {}
		
		GameObject(const GameObject& other) : IdentifiedObject(), gManager(other.gManager) , componentManager(other.componentManager) {}
		virtual ~GameObject() {}

		GameObject(GameObject&& other) noexcept: IdentifiedObject(), gManager(other.gManager) , componentManager(other.componentManager){}

		template<typename T, typename ... Args>
		T& GetOrAddComponent(Args&& ... args) {
			return componentManager.GetOrAddComponent<T>(GetId(), std::forward<Args>(args)...);
		}

		template<typename T>
		T* GetComponent()
		{
			return componentManager.GetComponent<T>(GetId());
		}

		template<typename T>
		bool RemoveComponent() {
			return componentManager.RemoveComponent<T>(GetId());
		}

	protected:
		Core::GameManagers& gManager;
		Components::ComponentManager& componentManager;
	};




	template<typename T>
	concept DerivesFromGameObject = std::is_base_of_v<GameObject, T>;


	// I'd like to have global access to this functionalities but singletons meh.
	// So far I'll do it static but this must be improved.
	struct GameObjectFactory
	{
		/**
		* @brief constructs a game object and injects the manager if possible
		*/
		template<DerivesFromGameObject T, typename ... Args>
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
		template<DerivesFromGameObject T, typename ... Args>
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

		template<DerivesFromGameObject T, typename ... Args>
		static Utils::Handle<GameObject> MakePureGameObjectHandle(Args&& ... args)
		{
			assert(gManager != nullptr && "GameObjectFactory has not initialized");
			T* gameObject = nullptr;
			if constexpr (std::is_constructible_v<T, Core::GameManagers&, Args...>)
			{
				gameObject = new T{ *gManager, std::forward<Args>(args)... };
			}
			else
			{
				gameObject = new T{ std::forward<Args>(args)... };
			}

			return Utils::Handle<GameObject>{ gameObject };
		}

		static void RegisterGameManagers(Core::GameManagers& managers) {
			gManager = &managers;
		}

		static void InvalidateGameManagers() { gManager = nullptr; }
	private:
		inline static Core::GameManagers* gManager = nullptr;
	};
}