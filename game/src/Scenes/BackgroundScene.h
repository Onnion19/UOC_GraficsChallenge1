#pragma once 

#include "Scenes/Scene.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Block.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Spaceship.h"
#include "GameObjects/HUD.h"


class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(Core::GameManagers& manager);
	void Activate();
	void DeActivate() {}
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	void SpawnAsteroid();
private:
	std::string text;
	std::vector<GameObject::Asteroid> asteroids;
	GameObject::Spaceship spaceship;
	GameObject::HUD hud;
};

