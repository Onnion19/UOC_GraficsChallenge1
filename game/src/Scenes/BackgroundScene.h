#pragma once 

#include "Scenes/Scene.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Block.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Spaceship.h"

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
	GameObject::Asteroid asteroid;
	GameObject::Spaceship spaceship;
};

