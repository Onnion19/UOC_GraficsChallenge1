#pragma once 

#include "Scenes/Scene.h"

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(Core::GameManagers& manager) : Scenes::SceneBase<BackgroundScene>(manager) {}
	void Activate() {}
	void DeActivate() {}
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	std::string text;
};

