#pragma once

#include "Core/GameLoop.h"
#include "Resources/ResourceManager.h"
namespace Core {
	class Game {
	public: 
		Game(std::string_view name) 
			: mainWindow(CreateWindow(800, 400, name))
			, gameLoop([window = mainWindow.get()]() {return window->WantsToClose(); }, mainWindow.get()) {}

		void Start();
	private: 
		void Initialize();
		void DeInitialize();
	private:
		WindowHandle mainWindow;
		GameLoop gameLoop;
		ResourceManager resourceManager;
	};
}