#include "BackgroundScene.h"
#include "Scenes/SceneManager.h"

BackgroundScene::BackgroundScene(Core::GameManagers& manager)
	: Scenes::SceneBase<BackgroundScene>(manager)
	, asteroid(GameObject::GameObjectFactory::MakeGameObject<GameObject::Asteroid>(GameObject::AsteroidTransform{ {80, 80}, {10, 30}, {1,1} }))
	, spaceship(GameObject::GameObjectFactory::MakeGameObject<GameObject::Spaceship>( Utils::Vector2f {500.f , 500.f})) {

}


void BackgroundScene::Activate()
{

}

void BackgroundScene::Update(float deltaTime) {
	text = "Delta time: " + std::to_string(deltaTime);
	asteroid.Update(deltaTime);
	spaceship.Update(deltaTime);
	
}

void BackgroundScene::Draw() {
	DrawText(text.c_str(), 50, 50, 30, Color{ 255, 255, 255, 255 });
	spaceship.Draw();
	asteroid.Draw();
}

void BackgroundScene::Finish() {
	// always visible
}

