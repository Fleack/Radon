#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>

namespace Radon
{
using namespace Urho3D;

class SceneBuilder : public Object
{
    URHO3D_OBJECT(SceneBuilder, Object);

public:
    explicit SceneBuilder(Context* context);
    ~SceneBuilder() override = default;

    void SetupGameplayScene(Scene* scene);
    void SetupMenuScene(Scene* scene);
    Node* CreateCamera(Scene* scene);
    Node* CreateMenuCamera(Scene* scene, const Vector3& position = Vector3(5.0f, 3.0f, 0.0f));

private:
    Node* CreateLight(Scene* scene);
    void CreateFloor(Scene* scene);
    void CreateObjects(Scene* scene);
    void CreateMenuBackgroundObjects(Scene* scene);
};

} // namespace Radon
