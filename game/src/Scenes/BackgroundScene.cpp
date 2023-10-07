#include "BackgroundScene.h"
#include "Scenes/SceneManager.h"

void BackgroundScene::Update(float deltaTime) {
	text = "Delta time: " + std::to_string(deltaTime);
}

void BackgroundScene::Draw() {
	DrawText(text.c_str(), 50, 50, 30, Color{ 255, 255, 255, 255 });
}

void BackgroundScene::Finish() {
	// always visible
}

