#pragma once
#include "Resources/ResourceManager.h"
#include <string>
#include "Core/GameManagers.h"

class SceneManager;

namespace Scenes
{
	/*
		IScene interface to enable dynamic polymorphism
	*/
	class IScene {
	public:
		// Function to set up all the requirements for the scene to work (load resources if necessary).
		virtual void _Activate() = 0;
		// Function to free the scene memory before going out
		virtual void _DeActivate() = 0;
		// Update function of the scene
		virtual void _Update(float deltaTime) = 0;
		// Drawing function, populate buffers before sending to gpu.
		virtual void _Draw() = 0;
	protected:
		// Finish will unload the scene and make it inactive
		virtual void _Finish() = 0;
	};

	/*
		CRTP pattern to flatt the overhead of virtual cost to only 1 level of indirection.

		SceneBase should be inherited by all the scenes.
	*/
	template<typename T>
	class SceneBase : public IScene {
	public:
		SceneBase(Core::GameManagers& manager) : managers(manager) {}
		virtual ~SceneBase() {}
		/* IScene implementation */
		void _Activate() override { static_cast<T*>(this)->Activate(); }
		void _DeActivate() override { static_cast<T*>(this)->DeActivate(); }
		void _Update(float deltaTime) override { static_cast<T*>(this)->Update(deltaTime); }
		void _Draw() override { static_cast<T*>(this)->Draw(); }
		void _Finish() override { static_cast<T*>(this)->Finish(); }
		/* ~~~~~~~~~~~~~~~~~~~~~ */
	protected:
		Core::GameManagers& managers;
	};

}

using SceneHandle = Utils::Handle<Scenes::IScene>;

template<typename T>
SceneHandle MakeSceneHandle(Core::GameManagers& managers) {
	return std::make_unique<T>(managers);
}
