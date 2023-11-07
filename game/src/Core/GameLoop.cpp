#include "GameLoop.h"
#include <iostream>
#include "Scenes/Scene.h"
void Core::GameLoop::Loop()
{
	[[maybe_unused]] auto timeSinceInitialization = timer(); // Flush all the time from construction to first frame

#if DEBUG
	// Some tracking data is always nice :D
	std::cerr << "Initialization took: " << timeSinceInitialization.count() << std::endl;
#endif

	while (!stopCondition())
	{
		auto deltaTime = timer(); //Frist frame delta time must be 0 or almost 0.

		if (activeScene && isRunning) {
			// 1. Update current scene 
			activeScene->_Update(std::chrono::duration_cast<Internal::Timer::UpdateDuration>(deltaTime).count());
		}


		// 2. Prepare to Draw
		window->BeginDraw();
		window->Clear();

		// 3. Draw Scene
		// TODO 2 ifs checking for the active scene seems to much for a game loop function. Must find a better way.
		if (activeScene) {
			activeScene->_Draw();
		}

		// 4. End drawing (Send all data to gpu)
		window->EndDraw();

#if DEBUG
		if (IsKeyPressed(KEY_P))
		{
			TogglePause();
		}
#endif
	}
}

void Core::GameLoop::OnSceneLoaded(Scenes::IScene* scene)
{
	activeScene = scene;
}

void Core::GameLoop::OnSceneUnloaded(Scenes::IScene* scene)
{
	activeScene = nullptr;
}

void Core::GameLoop::TogglePause()
{
	isRunning = !isRunning;
}
