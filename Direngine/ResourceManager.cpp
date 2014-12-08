#include "ResourceManager.h"

static ResourceManager* instance;

ResourceManager* ResourceManager::GetInstance() {
  if (instance == nullptr)
    instance = new ResourceManager();

  return instance;
}

void ResourceManager::DestroyResourceManager(ResourceManager*& resourceManagerPointer) {
  if (instance) {
    delete instance;
    instance = nullptr;
    resourceManagerPointer = nullptr;
  }
}