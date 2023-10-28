#include "EndScreen.h"
#include "raylib.h"
#include "Scenes/SceneManager.h"

EndScene::EndScene(Core::GameManagers& manager) : SceneBase<EndScene>(manager), flickeringEffect(0.f)
{
}

void EndScene::Activate()
{
}

void EndScene::DeActivate()
{
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
	DrawText("GAME OVER", 700, 460, 80, RED);
	int timer = static_cast<int>(flickeringEffect * 1.5f);
	if (timer % 2)
	{
		DrawText("Press Space to restart", 780, 620, 30, WHITE);
		DrawText("Press ESC to Exit", 800, 690, 30, WHITE);
	}

}

void EndScene::Finish()
{
}
