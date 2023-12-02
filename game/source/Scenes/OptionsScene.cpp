#include "Scenes/OptionsScene.h"
#include "Utils/Timers.h"
#include "Utils/TypeConversion.hpp"
#include "Scenes/SceneManager.h"


OptionsScene::OptionsScene(Core::GameManagers& manager) : SceneBase<OptionsScene>(manager)
{
}

void OptionsScene::Activate()
{}

void OptionsScene::DeActivate()
{}

void OptionsScene::Update(float deltaTime)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "MainMenu" });
	}
}

void OptionsScene::Draw()
{
	DrawText("Move with W A S D", static_cast<int>(1920 / 2.4), static_cast<int>(1080 / 2.1), 30, WHITE);
	DrawText("Jump with space", static_cast<int>(1920 / 2.4), static_cast<int>(1080 / 2), 30, WHITE);
	DrawText("Press space to go back", static_cast<int>(1920 / 2.4), static_cast<int>(1080 / 1.9), 30, WHITE);
}

void OptionsScene::Finish()
{}
