#include "Scenes/SceneManager.h"
#include <assert.h>
#include <algorithm>
#include "Scenes/Scene.h"

SceneManager::~SceneManager()
{
	Clear();
}

void SceneManager::RegisterListener(Scenes::SceneManagerListenerTE listener)
{
	auto iter = std::find(listeners.begin(), listeners.end(), listener);
	if (iter != listeners.end()) return;

	listeners.push_back(listener);
}

void SceneManager::UnregisterListener(Scenes::SceneManagerListenerTE listener)
{
	auto iter = std::find(listeners.begin(), listeners.end(), listener);
	if (iter == listeners.end()) return;
	listeners.erase(iter);
}

void SceneManager::RemoveScene(ResourceID id)
{
	auto scene = FetchScene(id);
	if (scene == currentScene)	UnloadScene();
	scenes.erase(id);
}

void SceneManager::LoadScene(ResourceID id)
{
	if (auto scene = FetchScene(id))
	{
		LoadScene_Internal(scene);
	}
}

void SceneManager::UnloadScene()
{
	if (!currentScene)return;
	currentScene->_DeActivate();
	std::for_each(listeners.begin(), listeners.end(), [&](const Scenes::SceneManagerListenerTE& listener) {listener.SceneUnloaded(currentScene); });
	currentScene = nullptr;
}

void SceneManager::Clear()
{
	UnloadScene();
	for (auto&& [id, scene] : scenes)
	{
		scene.reset();
	}
	scenes.clear();
}

void SceneManager::LoadScene_Internal(Scenes::IScene* scene)
{
	assert(scene);
	UnloadScene();
	scene->_Activate();
	std::for_each(listeners.begin(), listeners.end(), [&](const Scenes::SceneManagerListenerTE& listener) {listener.SceneLoaded(scene); });
	currentScene = scene;
}

Scenes::IScene* SceneManager::FetchScene(ResourceID id)
{
	auto iter = scenes.find(id);
	return (iter != scenes.end()) ? iter->second.get() : nullptr;
}
