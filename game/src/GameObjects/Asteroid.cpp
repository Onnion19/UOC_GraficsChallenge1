#include "Asteroid.h"
#include "Utils/Geometry.h"
#include "Core/Physics.h"
#include "Utils/TypeConversion.hpp"
#include "Resources/Texture.h"
#include "Utils/GameplayManager.h"


namespace {
	void EnsurePositionInScreen(Utils::Vector2f& pos)
	{
		if (pos.x > 1920.f) pos.x = pos.x - 1920.f;
		else if (pos.x < 0.f) pos.x = 1920.f + pos.x;

		if (pos.y > 1080.f) pos.y = pos.y - 1080.f;
		else if (pos.y < 0.f) pos.y = 1080.f + pos.y;
	}
}

GameObject::Asteroid::Asteroid(Core::GameManagers& manager, const AsteroidTransform& trans) : GameObject::GameObject(manager), transform(trans)
{
	RegisterCollider();
	texture = gManager.GetManager<ResourceManager>().GetOrLoad<Texture2D>(asteroidTextureID, "resources/asteroid.png");
}

GameObject::Asteroid::Asteroid(const Asteroid& other) : GameObject::GameObject(other.gManager), transform(other.transform)
{
	RegisterCollider();
	texture = other.texture;
}

GameObject::Asteroid& GameObject::Asteroid::operator=(const Asteroid& other)
{
	transform = other.transform;
	UnregisterCollider();
	RegisterCollider();
	return *this;
}

GameObject::Asteroid::~Asteroid()
{
 	UnregisterCollider();
}

void GameObject::Asteroid::Update(float deltaTime)
{
	if (!colider.Valid())return;
	transform.position += transform.movement * deltaTime;
	EnsurePositionInScreen(transform.position);

	auto bot_right = transform.position + (transform.size * health);
	colider.UpdateColliderBounds<Geometry::Rectangle>({ Geometry::Point{ transform.position }, Geometry::Point{ bot_right } });
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

bool GameObject::Asteroid::Valid() const
{
	return colider.Valid();
}

void GameObject::Asteroid::OnCollision()
{
	--health;
	// Send the event to update the score
	gManager.GetManager<GameplayManager>().UpdateScore(10 * (InitialHP - health));

	// Invalidate if the asteroid is destroyed
	if (health <= 0)
		gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}

void GameObject::Asteroid::RegisterCollider()
{
	auto bot_right = transform.position + transform.size;
	colider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Rectangle>(*this, Geometry::Point{ transform.position }, Geometry::Point{ bot_right });

}

void GameObject::Asteroid::UnregisterCollider()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}
