#pragma once
#include "Resources/ResourceManager.h"

namespace Scenes
{
	/*
		IScene interface to enable dynamic polymorphism
	*/
	class IScene {
	public:
		virtual void _Activate();
		virtual void _DeActivate();
		virtual void _Update(float deltaTime);
		virtual void _Draw();
		virtual void _Finish();
	};

	/*
		CRTP pattern to flatt the overhead of virtual cost to only 1 level of indirection.

		SceneBase should be inherited by all the scenes.
	*/
	template<typename T>
	class SceneBase : public IScene {
	public:
		SceneBase(ResourceManager& manager) : resourceManager(manager) {}
		virtual ~SceneBase() {}
		/* IScene implementation */
		void _Activate() override { static_cast<T*>(this)->Activate(); }
		void _DeActivate() override { static_cast<T*>(this)->DeActivate(); }
		void _Update(float deltaTime) override { static_cast<T*>(this)->Update(deltaTime); }
		void _Draw() override { static_cast<T*>(this)->Draw(); }
		void _Finish() override { static_cast<T*>(this)->Finish(); }
		/* ~~~~~~~~~~~~~~~~~~~~~ */	
	protected:
		ResourceManager& resourceManager;
	};

}