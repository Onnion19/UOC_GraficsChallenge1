#include "window.h"
#include <assert.h>

Core::Window::Window(int width, int height, std::string_view name) noexcept
{
	if (!Init(width, height, name))
	{
		assert(false && "Window could not be initialized");
	}
}

bool Core::Window::Init(int width, int height, std::string_view name) const
{
	InitWindow(width, height, name.data());
	InitAudioDevice();
	// Some what I'd expect a callback when the window is ready rather than polling it.
	if (!IsWindowReady()) {
		return false;
	}
	SetWindowState(WindowFlags);
	Clear();
	return true;
}

bool Core::Window::WantsToClose(bool close) const
{
	return WindowShouldClose();
}


void Core::Window::BeginDraw() const
{
	BeginDrawing();
}

void Core::Window::EndDraw() const
{
	EndDrawing();
}

Utils::Vector2i Core::Window::GetWindowSize() const
{
	return { GetRenderWidth(), GetRenderHeight() };
}

void Core::Window::Clear(const Color& c) const
{
	// Clear background is doing a copy of color...
	ClearBackground(c);
}

WindowHandle CreateWindow(int width, int height, std::string_view name)
{
	Core::Window* window = new Core::Window(width, height, name);
	return { window, Core::WindowDispatch{} };
}
