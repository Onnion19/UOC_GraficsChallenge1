#pragma once

#include "Core/window.h"

class WindowManager {
	static constexpr int SCREEN_WIDTH{ 1920 };
	static constexpr int SCREEN_HEIGHT{ 1080 };

public: 
	WindowManager(std::string_view name, int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT);
	WindowManager(WindowHandle&& w);
	Core::Window* GetCurrentWindow() const;

private: 
	WindowHandle window;
};