#include "Bullet.h"
#include "Utils/Geometry.h"
#include "Core/Physics.h"
#include "Resources/ResourceManager.h"
#include "Resources/Texture.h"

namespace {
	const ResourceID bulletTextureID{ "bulletTexture" };
}

GameObject::Bullet::Bullet(Core::GameManagers& manager, const BulletTransform& trans) : GameObject::GameObject(manager), transform(trans), physicsManager(gManager.GetManager < Core::PhysicsManager>())
{
	RegisterCollider();
	texture = &gManager.GetManager<ResourceManager>().GetOrLoad<Texture2D>(bulletTextureID, "resources/bullet.png");
}

GameObject::Bullet::~Bullet()
{
	UnRegisterCollider();
}

GameObject::Bullet::Bullet(const Bullet& b) : GameObject::GameObject(b.gManager), transform(b.transform), physicsManager(gManager.GetManager < Core::PhysicsManager>())
{
	RegisterCollider();
	texture = b.texture;
}

GameObject::Bullet& GameObject::Bullet::operator=(const Bullet& b)
{
	transform = b.transform;
	lifespanTimer = b.lifespanTimer;
	texture = b.texture;
	// invalidate old collider
	UnRegisterCollider();
	// Generate new collider
	RegisterCollider();
	return *this;
}

void GameObject::Bullet::Update(float deltaTime)
{
	if (!colider.Valid())return;

	transform.position += transform.movement * deltaTime;
	auto postion =transform.position + Geometry::ForwardVector(transform.rotation) * (transform.size.x/2.f);
	colider.UpdateColliderBounds<Geometry::Circle>({ Geometry::Point{ postion }, static_cast<float>(transform.size.x)/2.f });

	physicsManager.CheckCollisionOnCollider(colider);
	lifespanTimer -= deltaTime;
	if (lifespanTimer < 0)
		Invalidate();
}

bool GameObject::Bullet::Active() const
{
	return colider.Valid();
}

void GameObject::Bullet::Draw()const
{
	if (!colider.Valid()) return;

	const Rectangle textureQuad{ 0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height) };
	const Rectangle renderQuad{ transform.position.x, transform.position.y, static_cast<float>(transform.size.x), static_cast<float>(transform.size.y) };
	DrawTexturePro(*texture, textureQuad, renderQuad, { static_cast<float>(transform.size.x), static_cast<float>(transform.size.y) }, transform.rotation, WHITE);
}

void GameObject::Bullet::OnCollision()
{
	UnRegisterCollider();
}

void GameObject::Bullet::Invalidate()
{
	UnRegisterCollider();
}

void GameObject::Bullet::RegisterCollider()
{
	colider = gManager.GetManager<Core::PhysicsManager>().RegisterCollider<Geometry::Circle>(*this, Geometry::Point{ transform.position }, transform.size.x/2.f);

}

void GameObject::Bullet::UnRegisterCollider()
{
	if (!colider.Valid())return;
	gManager.GetManager<Core::PhysicsManager>().UnregisterCollider(colider);
}
