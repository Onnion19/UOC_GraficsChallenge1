#pragma once 

#include "Scenes/Scene.h"
#include "GameObjects/Asteroid.h"
#include "GameObjects/Spaceship.h"
#include "GameObjects/HUD.h"
#include "Utils/Timers.h"
#include "Resources/music.h"

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(Core::GameManagers& manager);
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
	
	void OnHealthUpdate(unsigned int newHealth);
private:
	void SpawnAsteroid();

private: 
	static constexpr auto backgroundMusicPath{ "resources/gameMusic.wav" };
	inline static const ResourceID backgroundMusicId{ "BackgroundMusic" };
private:
	std::string text;

	std::vector<GameObject::Asteroid> asteroids;

	Utils::Handle<GameObject::Spaceship> spaceship;
	Utils::Handle<GameObject::HUD> hud;

	Utils::SafeCallbackObject asteroidsSpawnerObject; 
	Utils::SafeCallbackObject healthCallback;

	Utils::RepeatingTimerWithVariation asteroidsSpawnerTimer;

	Music* backgroundMusic;
};

