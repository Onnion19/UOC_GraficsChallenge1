#pragma once

#include "Core/GameLoop.h"
#include "Core/GameManagers.h"



namespace Core {

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