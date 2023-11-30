#include "Core/WindowManager.h"


WindowManager::WindowManager(std::string_view name, int width, int height)
{
    window = CreateWindow(width, height, name);
}

WindowManager::WindowManager(WindowHandle&& w)
{
    window = std::move(w);
}

Core::Window* WindowManager::GetCurrentWindow() const
{
    return window.get();
}
