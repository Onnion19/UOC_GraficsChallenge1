#include "raylib.h"
#include "Resources/Texture.h"
#include "Components/Transform.h"
namespace RenderUtils {

	void RenderImage(Resources::Texture* texture, const Rectangle& textureQuad, const Components::Transform& transform , const Color& color = WHITE);

}