#include "Asteroid.h"
#include "Utils/Geometry.h"
#include "Core/Physics.h"
#include "Utils/TypeConversion.hpp"
#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"

GameObject::Asteroid::Asteroid(Core::GameManagers& manager, const AsteroidTransform& trans) : GameObject::GameObject(manager), transform(trans)
{
	RegisterCollider();
	texture = &gManager.GetManager<ResourceManager>().GetOrLoad<Texture2D>(ResourceID{ "asteroid" }, "resources/asteroid.png");
}

GameObject::Asteroid::~Asteroid()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Asteroid::Update(float deltaTime)
{
	if (!colider.Valid())return;
	transform.position += transform.movement * deltaTime;
	auto bot_right = transform.position + (transform.size * health);
	colider.UpdateColliderBounds<Geometry::Rectangle>({ Geometry::Point{ transform.position }, Geometry::Point{ bot_right } });
	DrawRectangle(transform.position.x, transform.position.y, transform.size.x * health, transform.size.y * health, YELLOW);
}

void GameObject::Asteroid::Draw()
{
	if (!colider.Valid()) return;
	auto renderQuad = GeometryToRaylib::RectangleToRaylib(transform.position, Utils::Vector2f{ transform.size * health });
	Rectangle textureQuad{};
	textureQuad.width = static_cast<float>(texture->width);
	textureQuad.height = static_cast<float>(texture->height);
	DrawTexturePro(*texture, textureQuad, renderQuad, {}, 0, WHITE);
}

void GameObject::Asteroid::OnCollision()
{
	--health;
	if (health <= 0)
		gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Asteroid::RegisterCollider()
{
	auto bot_right = transform.position + transform.size;
	colider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(*this, Geometry::Point{ transform.position }, Geometry::Point{ bot_right });

}
