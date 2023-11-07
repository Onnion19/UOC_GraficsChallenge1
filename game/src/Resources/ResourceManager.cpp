#include "ResourceManager.h"

int ResourceManager::ResourceData::GetUsageCount() const
{
    return _usage();
}

bool ResourceManager::ResourceData::Valid() const
{
    return _valid();
}
