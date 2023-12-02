#include "Scenes/EndScreen.h"
#include "Scenes/SceneManager.h"
#include "Scenes/BackgroundScene.h"
#include "Utils/GameplayManager.h"	
#include "Core/Physics.h"
EndScene::EndScene(Core::GameManagers& manager) : SceneBase<EndScene>(manager), flickeringEffect(0.f)
{
}

void EndScene::Activate()
{
	font = managers.GetManager<ResourceManager>().GetOrLoad<Font>(fontID, fontPath);
	auto& gameplayManager = managers.GetManager<GameplayManager>();
	auto score = gameplayManager.GetScore();
	message = (gameplayManager.GetHealth() > 0) ? "VICTORY" : "GAME OVER";
}

void EndScene::DeActivate()
{
	font.reset();
}

void EndScene::Update(float deltaTime)
{
	flickeringEffect += deltaTime;

	if (IsKeyPressed(KEY_SPACE))
	{
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "GameScene" });
	}
}

void EndScene::Draw()
{
	DrawTextEx(*font, message.c_str(), {700, 460}, 80, 0, RED);
	DrawTextEx(*font, highscoreText.c_str(), {650, 580}, 40, 0, WHITE);
	int timer = static_cast<int>(flickeringEffect * 1.5f);
	if (timer % 2)
	{
		DrawTextEx(*font, action1, { 700, 620 }, 30, 0, WHITE);
		DrawTextEx(*font, action2, { 720, 690 }, 30, 0, WHITE);
	}
}

void EndScene::Finish()
{
}
