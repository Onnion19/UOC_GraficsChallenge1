#pragma once 

#include "Scenes/Scene.h"

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(ResourceManager& manager, SceneManager* smanager) : Scenes::SceneBase<BackgroundScene>(manager, smanager) {}
	void Activate() {}
	void DeActivate() {}
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	std::string text;
};

