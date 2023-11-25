#include "Core/Collider.h"
#include "GameObjects/GameOjbect.h"

void Collider::SetGameObject(GameObject::GameObject& object)
{
    gameObject = &object;
}

GameObject::GameObject* Collider::GetGameObject()
{
    return gameObject;
}
