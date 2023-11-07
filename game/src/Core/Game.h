#pragma once

#include "Core/GameLoop.h"
#include "Core/GameManagers.h"

namespace Core {

	/**
	* Game class is the top most clas owning the low level systems and coordinating the managers.
	* Also owns the game loop which will execute the whole game logic.
	*/
	class  Game final {
	public:
		Game(std::string_view name);
		~Game();
		Game(const Game&) = delete;
		Game(Game&&) = delete;
		void Start();
	private:
		void Initialize();
		void DeInitialize();
	private:
		GameManagers managers;
		// Window will be auto closed and released (RAII).
		WindowHandle mainWindow;
		GameLoop gameLoop;
	};
}