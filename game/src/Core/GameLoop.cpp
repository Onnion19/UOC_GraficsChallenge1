#include "GameLoop.h"
#include <iostream>

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
		if (!activeScene || !isRunning) continue;

		// 1. Update current scene
		activeScene->_Update(deltaTime.count());

		// 2. Prepare to Draw
		window->BeginDraw();
		window->Clear();

		// 3. Draw Scene
		activeScene->_Draw();

		// 4. End drawing (Send all data to gpu)
		window->EndDraw();
	}
}

void Core::GameLoop::SetActiveScene(Scenes::IScene* scene)
{
	activeScene = scene;
}

void Core::GameLoop::TogglePause()
{
	isRunning = !isRunning;
}
