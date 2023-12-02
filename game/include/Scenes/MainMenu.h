#pragma once 
#include "Scenes/Scene.h"
#include "Utils/Handlers.h"
#include "Utils/Vector2.hpp"
#include "Resources/font.h"
#include "Resources/Texture.h"

class MainMenu : public Scenes::SceneBase<MainMenu> {

public:
	MainMenu(Core::GameManagers& manager);
	~MainMenu() = default;
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
private: 
	float lerp = 0.f;
	Utils::ResourceHandle<Resources::Font> mainMenuFont;
	Utils::ResourceHandle<Resources::Texture> backgroundImage;

	static constexpr auto author = "Developed by Aleix Rius";
	static constexpr auto action = "Press space to start";
	static constexpr auto action2 = "Press O for options";
	static constexpr auto fontPath = "resources/Overpass-SemiBold.ttf";
	inline static const ResourceID fontID{ "MainMenuFont" };
	inline const static ResourceID bgID{ "Menu Background" };
	static constexpr auto bgPath{ "resources/Menu/DonkeyKongMainLogo.png" };
	static constexpr Utils::Vector2f bgSize{ 1000, 750 };

};

