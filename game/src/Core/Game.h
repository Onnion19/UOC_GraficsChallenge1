#pragma once

#include "Core/GameLoop.h"
#include "Resources/ResourceManager.h"
namespace Core {
	class Game {
	public: 
		// TODO This closing condition lambda looks awful. Fix if there is time.
		Game(std::string_view name) 
			: mainWindow(CreateWindow(800, 400, name))
			, gameLoop([window = mainWindow.get()]() {return window->WantsToClose(); }, mainWindow.get()), scene(resourceManager) {}

		void Start();
	private: 
		void Initialize();
		void DeInitialize();
	private:
		// Window will be auto closed and released (RAII).
		WindowHandle mainWindow;
		GameLoop gameLoop;
		ResourceManager resourceManager;
		// TODO: This must be a SceneManager but for testing I'm fine with a dummy scene to preview things
		BackgroundScene scene;
	};
}