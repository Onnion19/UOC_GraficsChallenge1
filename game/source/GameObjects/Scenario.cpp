#include "GameObjects/Scenario.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"


namespace {
	Collider RegisterCollider(Utils::Vector2f pos, Utils::Vector2f size, Core::PhysicsManager& pm, GameObject::GameObject* owner)
	{
		Utils::Vector2f Size{ 1920 * size.x, 1080 * size.y };
		Utils::Vector2f Center{ 1920 * pos.x, 1080 * pos.y };
		Geometry::Point topLeft = Center - (Size* 0.5f);
		Geometry::Point botRight = Center + (Size* 0.5f);

		Geometry::Rectangle shape{topLeft, botRight};
		return pm.RegisterCollider<Geometry::Rectangle>(owner, topLeft, botRight);
	}
}


GameObject::Scenario::Scenario(Core::GameManagers& managers, const Utils::ResourceHandle<Resources::Texture>& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(texture);
	SetupTransform();
}

GameObject::Scenario::Scenario(Core::GameManagers& managers, Utils::ResourceHandle<Resources::Texture>&& texture) : GameObject(managers)
{
	spriteComponent = &GetOrAddComponent<Components::Sprite>(std::move(texture));
	SetupTransform();
}

void GameObject::Scenario::Draw()
{
	spriteComponent->Render(*transformComponent);
}

void GameObject::Scenario::SetupTransform()
{
	transformComponent = &GetOrAddComponent<Components::Transform>();
	transformComponent->position = { 1920 / 2.f , 1080 / 2.f };
	transformComponent->rotation = 0.f;
	transformComponent->size = { 1920.f, 1080.f };


	auto& pm = gManager.GetManager<Core::PhysicsManager>();
	colliders.push_back(RegisterCollider({ 0.5f,0.9844f }, { 1.f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.5f,0.8281 }, { .9286f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.125f,0.6719f }, { .1786f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.4821f,0.6719f }, { .3929f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.8571f,0.6719f }, { .2143f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.2143f,0.5156f}, { .4286f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.7857f,0.5156f}, { .4286f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.5f,0.3594f }, { .9286f,0.0313f }, pm, this));
	colliders.push_back(RegisterCollider({ 0.5f,0.2344f }, { .2143f,0.0313f }, pm, this));

}
