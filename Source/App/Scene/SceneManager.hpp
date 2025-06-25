#pragma once

#include <EASTL/hash_map.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>

namespace Radon::Scene
{

class SceneManager : public Urho3D::Object
{
    URHO3D_OBJECT(SceneManager, Object)
public:
    explicit SceneManager(Urho3D::Context* context);
    Urho3D::WeakPtr<Urho3D::Scene> LoadScene(ea::string const& sceneName);
    bool UnloadScene(ea::string const& sceneName);

private:
    [[nodiscard]] bool ExistsLocally(ea::string const& sceneName) const;

private:
    ea::hash_map<ea::string, Urho3D::SharedPtr<Urho3D::Scene>> scenes_;
};

} // namespace Radon::Scene
