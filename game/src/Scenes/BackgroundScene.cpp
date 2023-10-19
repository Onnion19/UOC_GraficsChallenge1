#include "BackgroundScene.h"
#include "Scenes/SceneManager.h"

BackgroundScene::BackgroundScene(Core::GameManagers& manager)
	: Scenes::SceneBase<BackgroundScene>(manager)
	, ball(GameObject::GameObjectFactory::MakeGameObject<GameObject::Ball>(GameObject::BallProperties{}))
	, ball2(GameObject::GameObjectFactory::MakeGameObject<GameObject::Ball>(GameObject::BallProperties{})){

}


void BackgroundScene::Activate()
{
	GameObject::BallProperties properties;
	ball.SetPosition({ 100,100 });
	ball2.SetPosition({ 350, 100 });
}

void BackgroundScene::Update(float deltaTime) {
	text = "Delta time: " + std::to_string(deltaTime);

	if (IsKeyDown(KEY_A))
	{
		auto pos = ball.GetPosition();
		pos -= Utils::Vector2i{ 1 , 0 };
		ball.SetPosition(pos);
	}
	else if (IsKeyDown(KEY_D))
	{
		auto pos = ball.GetPosition();
		pos += Utils::Vector2i{ 1 , 0 };
		ball.SetPosition(pos);
	}
}

void BackgroundScene::Draw() {
	DrawText(text.c_str(), 50, 50, 30, Color{ 255, 255, 255, 255 });
	ball.Draw();
	ball2.Draw();
}

void BackgroundScene::Finish() {
	// always visible
}

