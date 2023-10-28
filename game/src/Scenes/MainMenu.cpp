#include "MainMenu.h"
#include "Scenes/SceneManager.h"

namespace {
	Color ColorLerp(const Color& a, const Color& b, float l) {
		// This is probably wrong, this conversion should be done in hsl for better results.
		const float decimal = l - static_cast<int>(l);
		return
		{
			static_cast<unsigned char>(a.r * decimal + (b.r * 1 - decimal)),
			static_cast<unsigned char>(a.g * decimal + (b.g * 1 - decimal)),
			static_cast<unsigned char>(a.b * decimal + (b.b * 1 - decimal)),
			255
		};
	}
}


MainMenu::MainMenu(Core::GameManagers& manager) : SceneBase<MainMenu>(manager)
{}

void MainMenu::Activate()
{
	mainMenuFont = managers.GetManager<ResourceManager>().GetOrLoad<Font>(fontID, fontPath);
}

void MainMenu::DeActivate()
{
	managers.GetManager<ResourceManager>().Unload<Font>(fontID);
}

void MainMenu::Update(float deltaTime)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "GameScene" });
	}
	lerp += deltaTime / 2.f;
}

void MainMenu::Draw()
{
	const auto color = ColorLerp(BLUE, RED, lerp);
	DrawTextEx(mainMenuFont, title, { 700, 460 }, 80, 0, color);
	DrawTextEx(mainMenuFont, author, { 770, 620 }, 30, 0, color);
	DrawTextEx(mainMenuFont, action, { 790, 690 }, 30, 0, color);
}

void MainMenu::Finish()
{
}
