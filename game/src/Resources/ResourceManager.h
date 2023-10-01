#pragma once
#include <unordered_map>
#include <type_traits>
#include <typeinfo>
#include "Resources/ResourceLoader.h"
#include "Resources/font.h"
#include "Resources/music.h"




class ResourceManager {
	// Resources will be referenced by name --> "Main font", "Background music"...
	using ResourceName = std::string_view;
	// Functor to convert name to hash, in this case using std::hash (good enough).
	using ResourceIDFactory = std::hash<ResourceName>;
	// Hash type or resource ID will be the result of hashing the name: std::hash("Main Font").
	using ResourceID = std::invoke_result<ResourceIDFactory, ResourceName>::type;
	template<typename T>
	using ResourceHandle = Resources::Resource<T>::HandleTy;
	template<typename T>
	using ResourceHandlePtr = ResourceHandle<T>*;
	using ResourceMap = std::unordered_map < ResourceID, void*>;
	using TypeHash = std::size_t;

public:
	template <typename T, typename ... Args>
	T& Load(ResourceName name, Args&& ... args) {
		ResourceID id = GetResoruceID(name);
		if (auto ptr = FetchLoadedResource<T>(id))
		{
			assert(ptr, "Holding a handle with null data.");
			return *(ptr->get());
		}

		auto handle = new ResourceHandle<T>(resourceLoader.Load<T>(std::forward<Args>(args)...));
		auto& container = GetOrCreateContainerByType<T>();
		auto h = container.emplace(id, handle).first->second;
		return *(static_cast<ResourceHandlePtr<T>>(h)->get());
	}

	template <typename T>
	void Unload(ResourceName name) {
		ResourceID id = GetResoruceID(name);
		// TODO: this will do a double look up and can be optimized. 
		if (auto ptr = FetchLoadedResource<T>(id))
		{
			// clean the resource --> call the deleter functor
			ptr->reset();
			// remove the resource entry in the manager
			GetContainerByType<T>()->erase(id);
		}
	}

	// HEAVY BUG: destructor will clean up all the containers. Due to is storing wiht void* the destructor does not guarantee to call the deleters
	// of the underlying type but just the raw memory. This might cause memory leaking or malfunctions in raylib.
	// Current work around is to manually unload the previously loaded resources.
	~ResourceManager() = default;

private:

	template<typename T>
	inline ResourceMap* GetContainerByType()
	{
		TypeHash hash = typeid(T).hash_code();
		return GetContainerByHash(hash);
	}

	inline ResourceMap* GetContainerByHash(TypeHash hash)
	{
		auto it = resources.find(hash);
		return (it != resources.end()) ? &it->second : nullptr;
	}

	template<typename T>
	inline ResourceMap& GetOrCreateContainerByType()
	{
		TypeHash hash = typeid(T).hash_code();
		return resources[hash];
	}

	template<typename T>
	ResourceHandlePtr<T> FetchLoadedResource(ResourceID id) {
		const auto container = GetContainerByType<T>();
		if (!container) return nullptr;
		const auto it = container->find(id);
		// Getting the void* and casting back to the handle
		return (it != container->end()) ? static_cast<ResourceHandlePtr<T>>(it->second) : nullptr;

	}

	inline ResourceID GetResoruceID(ResourceName name)
	{
		return IdFactory(name);
	}

private:
	Resources::Loader	resourceLoader;
	ResourceIDFactory	IdFactory;
	std::unordered_map<TypeHash, ResourceMap> resources;
};