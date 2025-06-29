#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

class ICamera : public Urho3D::Object
{
    URHO3D_OBJECT(ICamera, Object);

public:
    explicit ICamera(Urho3D::Context* context)
        : Object(context) {}
    ~ICamera() override = default;

    virtual void Initialize(Urho3D::Node& cameraNode) = 0;
    virtual void Shutdown() = 0;

    virtual void SetLookSensitivity(float sensitivity) = 0;
    [[nodiscard]] virtual float GetLookSensitivity() const = 0;

    virtual void SetMoveSpeed(float speed) = 0;
    [[nodiscard]] virtual float GetMoveSpeed() const = 0;
};

} // namespace Radon::Engine::Input
