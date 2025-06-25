#include "App/Scene/SceneManager.hpp"

#include "App/Logger/Logger.hpp"

#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>

using namespace Urho3D;
using namespace Radon::Scene;

SceneManager::SceneManager(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("SceneManager: constructor called");
}

WeakPtr<Scene> SceneManager::LoadScene(ea::string const& sceneName)
{
    RADON_LOGDEBUG("SceneManager: LoadScene called for '{}'", sceneName);
    if (ExistsLocally(sceneName))
    {
        RADON_LOGINFO("SceneManager: Scene '{}' already loaded", sceneName);
        return scenes_.at(sceneName);
    }

    auto* cache = GetSubsystem<ResourceCache>();
    if (auto* xml = cache->GetResource<XMLFile>("Scenes/" + sceneName + ".xml"))
    {
        auto scene = MakeShared<Urho3D::Scene>(context_);
        if (!scene->LoadXML(xml->GetRoot()))
        {
            RADON_LOGERROR("Failed to load scene: {}", sceneName);
            return nullptr;
        }
        scenes_.emplace(sceneName, scene);
        RADON_LOGINFO("SceneManager: Scene '{}' loaded successfully", sceneName);
        return scene;
    }
    RADON_LOGERROR("Scene file not found: {}", sceneName);
    return nullptr;
}

bool SceneManager::UnloadScene(ea::string const& sceneName)
{
    RADON_LOGDEBUG("SceneManager: UnloadScene called for '{}'", sceneName);
    bool erased = static_cast<bool>(scenes_.erase(sceneName));
    if (erased)
        RADON_LOGINFO("SceneManager: Scene '{}' unloaded", sceneName);
    else
        RADON_LOGWARN("SceneManager: Tried to unload non-existent scene '{}'", sceneName);
    return erased;
}

bool SceneManager::ExistsLocally(ea::string const& sceneName) const
{
    bool exists = scenes_.contains(sceneName);
    RADON_LOGDEBUG("SceneManager: ExistsLocally('{}') = {}", sceneName, exists);
    return exists;
}
