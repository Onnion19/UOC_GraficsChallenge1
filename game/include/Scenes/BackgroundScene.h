#pragma once 

#include "Scenes/Scene.h"
#include "Utils/Timers.h"
#include "Utils/Handlers.h"
#include "Resources/music.h"

namespace GameObject {
	class Mario;
	class Scenario;
	class HUD;
};

class BackgroundScene : public Scenes::SceneBase<BackgroundScene> {
public:
	BackgroundScene(Core::GameManagers& manager);
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
	void OnHealthUpdate(unsigned int newHealth);

	~BackgroundScene();
private:


private: 
	static constexpr auto backgroundMusicPath{ "resources/gameMusic.wav" };
	static constexpr auto mapTexturePath{ "resources/Maps/L2.png" };
	inline static const ResourceID backgroundMusicId{ "BackgroundMusic" };
	inline static const ResourceID mapTextureId{ "Map" };
private:
	std::string text;
	Utils::Handle<GameObject::HUD> hud;

	Utils::Handle<GameObject::Mario> mario;
	Utils::Handle <GameObject::Scenario> map;

	Utils::SafeCallbackObject healthCallback;
	Utils::RepeatingTimerWithVariation asteroidsSpawnerTimer;
	Utils::ResourceHandle<Music> backgroundMusic;
};

