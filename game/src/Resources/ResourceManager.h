#pragma once
#include <unordered_map>
#include <type_traits>
#include <typeinfo>
#include "Resources/ResourceLoader.h"

// Include known types
#include "Texture.h"
#include "font.h"
#include "music.h"


// Resource ID is just to hash string into id's. 
// It would be nice to have constpexr hashing but this is probably out of the scope.
struct ResourceID
{
private:
	inline static const std::hash<std::string_view>hashFactory{};
	std::size_t id;
public:
	explicit ResourceID(std::string_view name) noexcept : id(hashFactory(name)) {}
	explicit ResourceID(const char* name) noexcept : id(hashFactory(name)) {}

	size_t GetId()const { return id; }
	inline operator size_t() const noexcept { return id; }
};

namespace std
{
	template<>
	struct std::hash<ResourceID> {
		size_t operator()(const ResourceID& resourceID) const noexcept {
			// implicit size_t cast
			return resourceID;
		}
	};
}

/*
	Resource Manager is a class that allows to handle and manage all type of objects, from loading from disk to release them.
	The assets are not tied to any type so you are free to specify any (Font, Image, String, CustomTypeTy).

	The storage is a multimap where:
	  - Key 1: asset type (using type hash which should be free (compile time))
	  - Key 2: Resource ID (std::hash is not compile time, but could use a custom implementation if needed)
	  - Value: void* containing the ResourceHandle. Each resource specifies the handle using type traits. @see ResourceLoader.h for more info
*/
class ResourceManager {

	template<typename T>
	using ResourceHandle = Resources::Resource<T>::HandleTy;
	template<typename T>
	using ResourceHandlePtr = ResourceHandle<T>*;
	using ResourceMap = std::unordered_map < ResourceID, void*>;
	using TypeHash = std::size_t;

public:
	template <typename ResourceTy, typename ... Args>
	ResourceTy& GetOrLoad(ResourceID id, Args&& ... args) {

		if (auto ptr = FetchLoadedResource<ResourceTy>(id))
		{
			assert(ptr && "Holding a handle with null data.");
			return *(ptr->get());
		}

		return Load<ResourceTy>(id, std::forward<Args>(args)...);
	}

	template <typename ResourceTy>
	void Unload(ResourceID id) {
		// TODO: this will do a double look up and can be optimized. 
		if (auto ptr = FetchLoadedResource<ResourceTy>(id))
		{
			// clean the resource --> call the deleter functor
			ptr->reset();
			// remove the resource entry in the manager
			GetContainerByType<ResourceTy>()->erase(id);
		}
	}

	// HEAVY BUG: destructor will clean up all the containers. Due to is storing wiht void* the destructor does not guarantee to call the deleters
	// of the underlying type but just the raw memory. This might cause memory leaking or malfunctions in raylib.
	// Current work around is to manually unload the previously loaded resources.
	// working with shared_ptr this might solvable, but then have to pay the overhead tax.
	~ResourceManager() = default;

private:

	template<typename T, typename ... Args>
	T& Load(ResourceID id, Args&& ... args) {
		auto handle = new ResourceHandle<T>(Resources::Loader::Load<T>(std::forward<Args>(args)...));
		auto& container = GetOrCreateContainerByType<T>();
		auto emplace = container.emplace(id, handle);
		assert(emplace.second && "Failed to emplace the new resource");
		return *(static_cast<ResourceHandlePtr<T>>(emplace.first->second)->get());
	}

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

private:
	std::unordered_map<TypeHash, ResourceMap> resources;
};