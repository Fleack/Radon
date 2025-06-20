#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/Node.h>

namespace Radon
{
using namespace Urho3D;

class MenuSceneAnimator : public Object
{
    URHO3D_OBJECT(MenuSceneAnimator, Object);

public:
    explicit MenuSceneAnimator(Context* context);
    ~MenuSceneAnimator() override = default;

    void Setup(Scene* scene, Node* cameraNode);
    void Update(float timeStep);

private:
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void AnimateCamera(float timeStep);
    void AnimateObjects(float timeStep);

private:
    Scene* scene_{nullptr};
    Node* cameraNode_{nullptr};
    float cameraYaw_{0.0f};
    float rotationSpeed_{5.0f};
    float time_{0.0f};
};

} // namespace Radon
