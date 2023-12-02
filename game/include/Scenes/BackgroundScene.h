#pragma once 

#include "Scenes/Scene.h"
#include "Utils/Timers.h"
#include "Utils/Handlers.h"
#include "Utils/Vector2.hpp"
#include "Resources/music.h"

namespace GameObject {
	class Mario;
	class DK;
	class Scenario;
	class HUD;
	class EnemiesPool;
	class InteractiveItem;
};

class GameplayManager;

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(Core::GameManagers& manager);
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();

	~BackgroundScene();
private:

	void PrepareEnemiesSpawner();
	void RegisterPowerUps(const Utils::Vector2i& screenSize);

private: 
	static constexpr auto backgroundMusicPath{ "resources/gameMusic.wav" };
	static constexpr auto mapTexturePath{ "resources/Maps/L2.png" };
	inline static const ResourceID backgroundMusicId{ "BackgroundMusic" };
	inline static const ResourceID mapTextureId{ "Map" };
private:
	GameplayManager* gameplayManager;
	std::string text;
	Utils::Handle<GameObject::HUD> hud;

	Utils::Handle<GameObject::Mario> mario;
	Utils::Handle<GameObject::DK> dk;
	Utils::Handle <GameObject::Scenario> map;
	Utils::Handle <GameObject::EnemiesPool> enemiesSpawner;
	std::vector<Utils::Handle<GameObject::InteractiveItem>> powerUps;

	std::vector<Utils::SafeCallbackObject> enemiesCallbackObjects;
	Utils::ResourceHandle<Music> backgroundMusic;
	bool gameOver = false;
};

