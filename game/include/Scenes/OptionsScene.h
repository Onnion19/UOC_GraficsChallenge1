#pragma once 

#include "Scenes/Scene.h"
#include "Utils/Vector2.hpp"
#include "Utils/Timers.h"
#include "Utils/Handlers.h"
#include "Resources/Font.h"

class OptionsScene : public Scenes::SceneBase<OptionsScene> {
public:
	OptionsScene(Core::GameManagers& manager);
	~OptionsScene() = default;
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:

private:
	Utils::ResourceHandle<Resources::Font> font;
};

