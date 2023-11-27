#include "Scenes/BackgroundScene.h"
#include "Scenes/SceneManager.h"
#include "Utils/GameObjectEmitter.h"
#include "Utils/GameplayManager.h"
#include "Utils/RandomGenerator.h"
#include "Components/Sprite.h"

#include "GameObjects/Mario.h"
#include "GameObjects/HUD.h"
#include "GameObjects/Scenario.h"

#if DEBUG
#include "Core/Physics.h"
#endif


namespace {
	struct AsteroidsTimerFunctor
	{
		float operator()(unsigned timesTriggered, float initialTime)
		{
			return std::max(1.f, initialTime * (1 - (timesTriggered * 0.1f)));
		}
	};
}


BackgroundScene::BackgroundScene(Core::GameManagers& manager)
	: Scenes::SceneBase<BackgroundScene>(manager)
	, asteroidsSpawnerTimer(5.f, AsteroidsTimerFunctor{}) {
}


void BackgroundScene::Activate()
{
	mario = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Mario>(Utils::Vector2f{ 30	,1020 });

	auto handle = managers.GetManager<ResourceManager>().GetOrLoad<Resources::Texture>(mapTextureId, mapTexturePath);
	map = GameObject::GameObjectFactory::MakeGameObjectHandle<GameObject::Scenario>(std::move(handle));
}

void BackgroundScene::DeActivate()
{
	mario.reset();
}

void BackgroundScene::Update(float deltaTime) {
	mario->Update(deltaTime);
#if DEBUG
	managers.GetManager<Core::PhysicsManager>().DrawDebugColliders();
#endif
}

void BackgroundScene::Draw() {
	map->Draw();
	mario->Draw();
}

void BackgroundScene::Finish() {

}

void BackgroundScene::OnHealthUpdate(unsigned int newHealth)
{
	if (newHealth == 0)
	{
		managers.GetManager<GameplayManager>().EndGame();
		managers.GetManager<SceneManager>().LoadScene(ResourceID{ "EndScene" });
	}
}

BackgroundScene::~BackgroundScene()
{
}


