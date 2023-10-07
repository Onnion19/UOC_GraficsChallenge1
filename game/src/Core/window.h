#pragma once 

#include "raylib.h"
#include <string_view>
#include <memory>
#include "Utils/Handlers.h"
#include <functional>
namespace Core {

	class Window {
	public:

	public:
		Window() = default;

		Window(int width, int height, std::string_view name) noexcept;
		bool Init(int width, int height, std::string_view name) const;
		bool WantsToClose(bool close = true) const;
		// Drawing controllers
		void BeginDraw() const;
		void EndDraw() const;

		// Clear the background of the window
		void Clear(const Color& c = WindowBackground) const;
	private:
		static constexpr int WindowFlags = ConfigFlags::FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE;
		static constexpr Color WindowBackground{ 30,30,30,30 };// dark gray

	};

	struct WindowDispatch {
		void operator()(Core::Window* w)
		{
			CloseWindow();
			delete w;
		}
	};
}

using WindowHandle = Utils::Handle<Core::Window, Core::WindowDispatch>;

WindowHandle CreateWindow(int width, int height, std::string_view name);
