#pragma once

#include "Core/GameLoop.h"
#include "Resources/ResourceManager.h"
#include "Scenes/SceneManager.h"
namespace Core {
	class Game {
	public: 
		Game(std::string_view name);
		void Start();
	private: 
		void Initialize();
		void DeInitialize();
	private:
		// Window will be auto closed and released (RAII).
		WindowHandle mainWindow;
		GameLoop gameLoop;
		ResourceManager resourceManager;
		SceneManager sceneManager;
	};
}