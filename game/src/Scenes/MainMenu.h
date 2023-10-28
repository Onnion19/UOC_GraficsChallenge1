#pragma once 
#include "Scenes/Scene.h"


class MainMenu : public Scenes::SceneBase<MainMenu> {
public:
	MainMenu(Core::GameManagers& manager);
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
private: 
	float lerp = 0.f;
	Font mainMenuFont;
	static constexpr auto title = "Not Arkanoid";
	static constexpr auto author = "Developed by Aleix Rius";
	static constexpr auto action = "Press space to start";
	static constexpr auto fontPath = "resources/Overpass-SemiBold.ttf";
	inline static const ResourceID fontID{ "MainMenuFont" };

};

