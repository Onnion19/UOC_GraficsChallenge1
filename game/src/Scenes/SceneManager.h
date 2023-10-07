#pragma once
#include "Resources/ResourceManager.h"
#include "Scenes/Scene.h"

namespace Scenes {
	// Concept verifying if a type can listen from scene manager callbacks
	template<typename T>
	concept SceneManagerListener = requires (T t) {
		t.OnSceneLoaded(nullptr);
		t.OnSceneUnloaded(nullptr);
	};

	// A non owning Scene manager listener Type erasure. 
	// A Listener is any type that has OnSceneLoaded and OnSceneUnloaded methods.
	struct SceneManagerListenerTE {
	public:
		template<SceneManagerListener Ty>
		SceneManagerListenerTE(Ty* t) {
			ptr = t;
			_OnSceneLoaded = [](void* ptr, IScene* scene) { return static_cast<Ty*>(ptr)->OnSceneLoaded(scene); };
			_OnSceneUnloaded = [](void* ptr, IScene* scene) { return static_cast<Ty*>(ptr)->OnSceneUnloaded(scene); };
		}

		bool operator==(const SceneManagerListenerTE& other) const { return ptr == other.ptr; }
		void SceneLoaded(IScene* scene) const { _OnSceneLoaded(ptr, scene); }
		void SceneUnloaded(IScene* scene) const { _OnSceneUnloaded(ptr, scene); }

	private:
		void* ptr;
		void (*_OnSceneLoaded)(void* ptr, IScene* scene);
		void (*_OnSceneUnloaded)(void* ptr, IScene* scene);
	};
}

// Scene Manager handling and storing all the scenes.
class SceneManager {
public:
	SceneManager(ResourceManager& manager) : resourceManager(manager) {};

	void RegisterListener(Scenes::SceneManagerListenerTE listener);
	void UnregisterListener(Scenes::SceneManagerListenerTE listener);

	template<typename T>
	void AddScene(ResourceID id, bool overrideScene);
	void RemoveScene(ResourceID id);

	void LoadScene(ResourceID id);
	void UnloadScene();

	template<typename T>
	void AddAndLoadScene(ResourceID id, bool overrideScene);
	void Clear();
private:
	template<typename T>
	Scenes::IScene* AddScene_Internal(ResourceID id, bool overrideScene);
	void LoadScene_Internal(Scenes::IScene* scene);
	Scenes::IScene* FetchScene(ResourceID id);
private:
	ResourceManager& resourceManager;
	std::unordered_map<ResourceID, SceneHandle> scenes;
	std::vector<Scenes::SceneManagerListenerTE> listeners;
	Scenes::IScene* currentScene = nullptr;
};


template<typename T>
inline void SceneManager::AddScene(ResourceID id, bool overrideScene)
{
	AddScene_Internal<T>(id, overrideScene);
}

template<typename T>
inline void SceneManager::AddAndLoadScene(ResourceID id, bool overrideScene)
{
	auto scene = AddScene_Internal<T>(id, overrideScene);
	LoadScene_Internal(scene);
}

template<typename T>
inline Scenes::IScene* SceneManager::AddScene_Internal(ResourceID id, bool overrideScene)
{
	Scenes::IScene* scene = FetchScene(id);
	if (!scene || overrideScene)
	{
		auto handle = MakeSceneHandle<T>(resourceManager, this);
		scene = handle.get();
		scenes[id] = std::move(handle);
	}
	return scene;
}
