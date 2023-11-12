#pragma once
#include <unordered_map>
#include <type_traits>
#include <typeinfo>
#include <optional>
#include <cassert>
#include "Resources/ResourceLoader.h"
#include "Utils/Handlers.h"



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

	// Struct holding the ref counting of the resoruce.
	struct ResourceData {
		template<typename shared_ptr>
		ResourceData(const shared_ptr& ptr);
		int GetUsageCount()const;
		bool Valid()const;
		template<typename T>
		std::shared_ptr<T> GetDataAs() const;
	private:
		std::function<bool()> _valid;
		std::function<void* ()> _data;
		std::function<int()> _usage;
	};


private:
	template<typename T>
	using ResourceHandle = Utils::ResourceHandle<T>;
	using ResourceMap = std::unordered_map < ResourceID, ResourceData>;
	using TypeHash = std::size_t;

public:
	template <typename ResourceTy, typename ... Args>
	ResourceHandle< ResourceTy> GetOrLoad(ResourceID id, Args&& ... args) {

		const std::optional<ResourceHandle<ResourceTy>> resource = FetchLoadedResource<ResourceTy>(id);
		if (resource.has_value())
		{
			return resource.value();
		}

		return Load<ResourceTy>(id, std::forward<Args>(args)...);
	}

	template <typename ResourceTy>
	unsigned int ResourceUsage(ResourceID id) {
		unsigned int usage = 0;
		const std::optional<ResourceHandle<ResourceTy>> resource = FetchLoadedResource<ResourceTy>(id);
		if (resource.has_value())
		{
			usage = resource.value().use_count() - 1; //Withdraw self usage
		}
		else
		{
			// remove unused resource
			GetContainerByType<ResourceTy>()->erase(id);
		}

		return usage;
	}

	~ResourceManager() = default;

private:

	template<typename T, typename ... Args>
	auto Load(ResourceID id, Args&& ... args) {
		ResourceHandle<T> shared_ptr = Resources::Loader::Load<T>(std::forward<Args>(args)...);
		auto& container = GetOrCreateContainerByType<T>();
		auto emplace = container.emplace(id, shared_ptr);
		assert(emplace.second && "Failed to emplace the new resource");
		return shared_ptr;
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
	std::optional<ResourceHandle<T>> FetchLoadedResource(ResourceID id) {
		const auto container = GetContainerByType<T>();
		if (!container) return std::nullopt;
		const auto it = container->find(id);

		if (it == container->end()) return std::nullopt;

		ResourceData data = it->second;
		if (data.Valid())
			return data.GetDataAs<T>();
		else
			return std::nullopt;
	}

private:
	std::unordered_map<TypeHash, ResourceMap> resources;
};

template<typename T>
struct functor {
	std::weak_ptr<T> ptr;

	void* operator()() {
		return &ptr;
	}
};

template<typename shared_ptr>
inline ResourceManager::ResourceData::ResourceData(const shared_ptr& ptr)
{
	std::weak_ptr <typename shared_ptr::element_type> weak{ ptr };
	_data = functor<typename shared_ptr::element_type>{ weak };
	_valid = [=]() {return !weak.expired(); };
	_usage = [=]() {return weak.use_count(); };
}

template<typename T>
inline std::shared_ptr<T> ResourceManager::ResourceData::GetDataAs() const
{
	std::weak_ptr<T> weak_ptr = *(static_cast<std::weak_ptr<T>*>(_data()));
	std::shared_ptr<T> shared_ptr = weak_ptr.lock();
	return std::move(shared_ptr);
}
