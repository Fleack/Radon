#pragma once

#include <Urho3D/Container/Str.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Resource/JSONValue.h>

namespace Radon::Engine::Config
{

class ConfigManager : public Urho3D::Object
{
    URHO3D_OBJECT(ConfigManager, Object)
public:
    explicit ConfigManager(Urho3D::Context* context);
    bool Load(ea::string const& filename);
    bool Save(ea::string const& filename) const;

    template <typename T>
    T Get(ea::string const& section, ea::string const& key, T const& defaultValue) const;

    template <typename T>
    void Set(ea::string const& section, ea::string const& key, T const& value);

private:
    Urho3D::JSONValue root_;
};

} // namespace Radon::Engine::Config
