#pragma once 

#include "Scenes/Scene.h"
#include "Utils/Vector2.hpp"
#include "Utils/Timers.h"
#include "Utils/Handlers.h"

class LogoScene : public Scenes::SceneBase<LogoScene> {
public:
	LogoScene(Core::GameManagers& manager);
	~LogoScene() = default;
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	inline static const ResourceID LogoID{ "logo" };
	static constexpr auto logoPath{ "resources/logo.png" };
	static constexpr int logoSize = 512;
	static constexpr float timeInScreen = 3.f;
private:
	Utils::ResourceHandle<Texture2D> logo;
	Utils::Vector2i logoPosition;
	Utils::SingleTimer logoTimer;
	Utils::SafeCallbackObject callbackObject;
};

