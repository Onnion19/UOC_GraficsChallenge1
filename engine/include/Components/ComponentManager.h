#pragma once

#include "GameObjects/GameObjectID.h"
#include <typeinfo>
#include <assert.h>

namespace Components {

	namespace _internal {

		/*
		* Type erasure component type
		*/
		struct ComponentTE {
			template<typename T>
			ComponentTE(T* t) noexcept {
				_data = t;
				Deleter = [](void* ptr) {delete static_cast<T*>(ptr); };
			}

			ComponentTE(ComponentTE&& other) noexcept {
				_data = other._data;
				Deleter = other.Deleter;
				other._data = nullptr;
				other.Deleter = nullptr;
			}

			~ComponentTE() { if(Deleter) Deleter(_data); }
			template<typename T>
			T* GetDataAs() const { return static_cast<T*>(_data); }

		private:
			void* _data = nullptr;
			void (*Deleter)(void*);

		};
	}


	class ComponentManager {
	public:
		using TypeId = std::size_t;
		using GameObjectID = GameObject::ID::rep;
		using GameObjectMap = std::unordered_map<GameObjectID, _internal::ComponentTE>;
		using ComponentMap = std::unordered_map<TypeId, GameObjectMap>;
	public:
		ComponentManager() noexcept = default;

		template<typename ComponentTy, typename ... Args>
		ComponentTy& GetOrAddComponent(const GameObjectID& gameObjectId, Args&& ... args) {
			const TypeId id = typeid(ComponentTy).hash_code();
			return GetOrAddComponentInternal<ComponentTy>(gameObjectId, componentMap[id], std::forward<Args>(args)...);
		}

		template<typename ComponentTy>
		ComponentTy* GetComponent(const GameObjectID& gameObjectId)
		{
			auto map = GetGameObjectMap<ComponentTy>();
			if (!map) return nullptr;
			return GetComponentInternal<ComponentTy>(gameObjectId, *map);
		}

		template<typename ComponentTy>
		bool RemoveComponent(const GameObjectID& gameObjectId)
		{
			auto map = GetGameObjectMap<ComponentTy>();
			if (!map) return;
			return RemoveComponentInternal(gameObjectId, *map);
		}


	private:

		template<typename T>
		GameObjectMap* GetGameObjectMap()
		{
			const TypeId id = typeid(T).hash_code();
			auto iter = componentMap.find(id);
			return (iter != componentMap.end()) ? &iter->second : nullptr;
		}

		template<typename T, typename ...Args>
		T& GetOrAddComponentInternal(const GameObjectID& id, GameObjectMap& map, Args&& ... args) {
			GameObjectMap::iterator iter = map.find(id);
			if (iter != map.end())
			{
				return *iter->second.GetDataAs<T>();
			}
			else
			{
				return AddComponent<T>(id, map, std::forward<Args>(args)...);
			}
		}

		template<typename T, typename ... Args>
		T& AddComponent(GameObjectID id, GameObjectMap& map, Args&& ... args)
		{
			assert(map.find(id) == map.end());
			T* t = new T(std::forward<Args>(args)...);
			auto result = map.emplace(id, _internal::ComponentTE{ t });
			assert(result.second);
			return *result.first->second.GetDataAs<T>();
		}

		template<typename T>
		T* GetComponentInternal(const GameObjectID& id, GameObjectMap& map)
		{
			auto iter = map.find(id);
			return (iter != map.end()) ? iter->second.GetDataAs<T>() : nullptr;
		}

		bool RemoveComponentInternal(const GameObjectID& id, GameObjectMap& map)
		{
			return map.erase(id) > 0;
		}

	private:
		ComponentMap componentMap;
	};
}