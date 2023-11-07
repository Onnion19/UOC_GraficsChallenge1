#include "LogoScene.h"
#include "Utils/Timers.h"
#include "Utils/TypeConversion.hpp"
#include "Scenes/SceneManager.h"


LogoScene::LogoScene(Core::GameManagers& manager) : SceneBase<LogoScene>(manager), logoTimer(timeInScreen), logo()
{
}

void LogoScene::Activate()
{
	logo = managers.GetManager<ResourceManager>().GetOrLoad<Texture2D>(LogoID, logoPath);
	logoPosition = { 1920 / 2, 1080 / 2 };
	logoPosition.x -= logoSize / 2;
	logoPosition.y -= logoSize / 2;

	auto& sceneManager = managers.GetManager<SceneManager>();
	callbackObject = logoTimer.Start([&sceneManager]() {
		sceneManager.LoadScene(ResourceID{ "MainMenu" });
		});
}

void LogoScene::DeActivate()
{
	logo.reset();
}

void LogoScene::Update(float deltaTime)
{
	logoTimer.Update(deltaTime);
}

void LogoScene::Draw()
{
	static Rectangle textureQuad{ 0, 0, static_cast<float>(logo->width), static_cast<float>(logo->height) };
	static Rectangle renderQuad{
		static_cast<float>(logoPosition.x),
		static_cast<float>(logoPosition.y),
		static_cast<float>(logoSize),
		static_cast<float> (logoSize)
	};

	DrawTexturePro(*logo, textureQuad, renderQuad, {}, 0.f, WHITE);

}

void LogoScene::Finish()
{}
