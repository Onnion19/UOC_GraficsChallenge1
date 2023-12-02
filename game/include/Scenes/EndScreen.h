#pragma once 

#include "Scenes/Scene.h"
#include "Utils/Handlers.h"
#include "Resources/font.h"

class EndScene : public Scenes::SceneBase<EndScene> {
public:
	EndScene(Core::GameManagers& manager);
	~EndScene() = default;
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	float flickeringEffect = 0.f;
	std::string highscoreText; 
	std::string message;
	Utils::ResourceHandle<Resources::Font> font;
	static constexpr auto action1 = "Press space to start";
	static constexpr auto action2 = "Developed by Aleix Rius";
	static constexpr auto timePlayed = "Your survived for: ";
	static constexpr auto fontPath = "resources/Agbalumo-Regular.ttf";
	inline static const ResourceID fontID{ "EndScreenFont" };
};

