#include "Scenes/MainMenu.h"
#include "Scenes/SceneManager.h"
#include "Resources/Color.h"
#include "Resources/Font.h"
#include "Utils/GameplayManager.h"
namespace {
	using namespace Resources;
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
	backgroundImage = managers.GetManager<ResourceManager>().GetOrLoad<Texture2D>(bgID, bgPath);
	mainMenuFont = managers.GetManager<ResourceManager>().GetOrLoad <Resources::Font>(fontID, fontPath);
}

void MainMenu::DeActivate()
{
	mainMenuFont.reset();
}

void MainMenu::Update(float deltaTime)
{
	if (IsKeyPressed(KEY_SPACE))
	{
		managers.GetManager<GameplayManager>().StartGame();
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "GameScene" });
	}
	lerp += deltaTime / 2.f;
}

void MainMenu::Draw()
{
	static Rectangle textureQuad{ 0, 0, static_cast<float>(backgroundImage->width), static_cast<float>(backgroundImage->height) };
	static Rectangle renderQuad{
		1920/2.f - bgSize.x/2.f,
		1080/2.f - bgSize.y/2.f,
		bgSize.x,
		bgSize.y
	};
	DrawTexturePro(*backgroundImage, textureQuad, renderQuad, {}, 0.f, WHITE);
	const auto color = ColorLerp(BLUE, RED, lerp);
	DrawTextEx(*mainMenuFont, author, { 770, 950 }, 30, 0, color);
	DrawTextEx(*mainMenuFont, action, { 790, 1000 }, 30, 0, color);
	DrawTextEx(*mainMenuFont, action2, { 790, 1050 }, 30, 0, color);
}

void MainMenu::Finish()
{
}
