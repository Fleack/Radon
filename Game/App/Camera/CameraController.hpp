#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon
{
using namespace Urho3D;

class CameraController : public Object
{
    URHO3D_OBJECT(CameraController, Object);

public:
    explicit CameraController(Context* context);
    ~CameraController() override = default;

    void Setup(Node* cameraNode);

    void HandleUpdate(StringHash eventType, VariantMap& eventData);

private:
    Node* cameraNode_{nullptr};
    float yaw_{0.0f}, pitch_{0.0f};
    float moveSpeed_{10.0f};
    float lookSensitivity_{0.1f};
};

} // namespace Radon
