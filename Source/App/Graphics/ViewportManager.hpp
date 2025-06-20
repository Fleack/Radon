#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Scene/Scene.h>

namespace Radon
{
using namespace Urho3D;

class ViewportManager : public Object
{
    URHO3D_OBJECT(ViewportManager, Object);

public:
    explicit ViewportManager(Context* context);
    ~ViewportManager() override = default;

    void SetupViewport(Scene* scene, Node* cameraNode);
};

} // namespace Radon
