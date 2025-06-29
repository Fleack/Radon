#include "ConfigManager.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/IO/File.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;
using namespace Radon::Engine::Config;

ConfigManager::ConfigManager(Context* context)
    : Object(context)
{
}

bool ConfigManager::Load(ea::string const& filename)
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* json = cache->GetResource<JSONFile>(filename);
    if (!json)
    {
        RADON_LOGWARN("Config load failed: {}", filename);
        return false;
    }
    root_ = json->GetRoot();
    return true;
}

bool ConfigManager::Save(ea::string const& filename) const
{
    JSONFile file(context_);
    file.GetRoot() = root_;
    File out(context_);
    if (!out.Open(filename, FILE_WRITE))
    {
        RADON_LOGERROR("Config save failed: {}", filename);
        return false;
    }
    file.Save(out, "\t");
    return true;
}

template <typename T>
T ConfigManager::Get(ea::string const& section, ea::string const& key, T const& defaultValue) const
{
    auto const& sec = root_[section];
    if (!sec.IsObject())
        return defaultValue;
    auto& val = sec[key];
    if constexpr (std::is_same_v<T, int>)
        return val.GetInt();
    else if constexpr (std::is_same_v<T, float>)
        return val.GetFloat();
    else if constexpr (std::is_same_v<T, bool>)
        return val.GetBool();
    else
        return val.GetString();
    // TODO Other types
}

template <typename T>
void ConfigManager::Set(ea::string const& section, ea::string const& key, T const& value)
{
    auto& sec = root_[section];
    if constexpr (std::is_same_v<T, int>)
        sec[key] = value;
    else if constexpr (std::is_same_v<T, float>)
        sec[key] = value;
    else if constexpr (std::is_same_v<T, bool>)
        sec[key] = value;
    else
        sec[key] = value;
}
