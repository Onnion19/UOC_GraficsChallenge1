#pragma once
#include "Resources/ResourceManager.h"

namespace Scenes
{
	/*
		IScene interface to enable dynamic polymorphism
	*/
	class IScene {
	public:
		virtual void _Activate() = 0;
		virtual void _DeActivate() = 0;
		virtual void _Update(float deltaTime) = 0;
		virtual void _Draw() = 0;
		virtual void _Finish() = 0;
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
		inline void _Activate() override { static_cast<T*>(this)->Activate(); }
		inline void _DeActivate() override { static_cast<T*>(this)->DeActivate(); }
		inline void _Update(float deltaTime) override { static_cast<T*>(this)->Update(deltaTime); }
		inline void _Draw() override { static_cast<T*>(this)->Draw(); }
		inline void _Finish() override { static_cast<T*>(this)->Finish(); }
		/* ~~~~~~~~~~~~~~~~~~~~~ */	
	protected:
		ResourceManager& resourceManager;
	};

}

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(ResourceManager& manager) :Scenes::SceneBase<BackgroundScene>(manager) {}
	void Activate() {}
	void DeActivate() {}
	inline void Update(float deltaTime) {
		text = "Delta time: " + std::to_string(deltaTime);
	}
	inline void Draw() {
		DrawText(text.c_str(), 50, 50, 30, Color { 255, 255, 255, 255 });
	}
	void Finish() {}
private: 
	std::string text;
};