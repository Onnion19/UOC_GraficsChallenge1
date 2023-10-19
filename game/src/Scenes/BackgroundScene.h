#pragma once 

#include "Scenes/Scene.h"
#include "GameObjects/Ball.h"

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(Core::GameManagers& manager);
	void Activate();
	void DeActivate() {}
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	std::string text;
	GameObject::Ball ball;
	GameObject::Ball ball2;
};

