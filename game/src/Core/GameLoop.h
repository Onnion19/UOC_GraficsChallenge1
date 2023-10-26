#pragma once

#include <functional>
#include <chrono>
#include "Core/window.h"

namespace Scenes {
	class IScene;
}

namespace Core {
	namespace Internal {
		struct Timer {
			using Duration = std::chrono::duration<float, std::milli>;
			using UpdateDuration = std::chrono::duration<float>; // seconds
			using Clock = std::chrono::steady_clock;
			using Timepoint = Clock::time_point;

			Timer() : start(Clock::now()), lastCheck(start) {}

			Duration operator()() {
				Timepoint current = Clock::now();
				Duration duration = current - lastCheck;
				lastCheck = current;
				return duration;
			}
		private:
			Timepoint start;
			Timepoint lastCheck;
		};
	}

	class GameLoop {
		using StopFunctor = std::function<bool()>;
	public:
		GameLoop(StopFunctor&& func, Core::Window* win) : stopCondition(std::move(func)), window(win), activeScene(nullptr), isRunning(true) {}
		void Loop();
		void OnSceneLoaded(Scenes::IScene* scene);
		void OnSceneUnloaded(Scenes::IScene* scene);
		void TogglePause();
	private:
		Internal::Timer timer;
		StopFunctor stopCondition;
		Core::Window* window;
		Scenes::IScene* activeScene;
		bool isRunning;
	};
}