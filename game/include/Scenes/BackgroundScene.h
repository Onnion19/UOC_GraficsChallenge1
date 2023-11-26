#pragma once 

#include "Scenes/Scene.h"
#include "GameObjects/HUD.h"
#include "Utils/Timers.h"
#include "Utils/Handlers.h"
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

	~BackgroundScene();
private:


private: 
	static constexpr auto backgroundMusicPath{ "resources/gameMusic.wav" };
	inline static const ResourceID backgroundMusicId{ "BackgroundMusic" };
private:
	std::string text;
	Utils::Handle<GameObject::HUD> hud;

	Utils::SafeCallbackObject asteroidsSpawnerObject; 
	Utils::SafeCallbackObject healthCallback;

	Utils::RepeatingTimerWithVariation asteroidsSpawnerTimer;

	Utils::ResourceHandle<Music> backgroundMusic;
};

