#include "Resources/ResourceManager.h"
// Include known types
#include "Resources/Texture.h"
#include "Resources/font.h"
#include "Resources/music.h"

int ResourceManager::ResourceData::GetUsageCount() const
{
	return _usage();
}

bool ResourceManager::ResourceData::Valid() const
{
	return _valid();
}
