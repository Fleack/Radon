#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

class ICameraController : public Urho3D::Object
{
    URHO3D_OBJECT(ICameraController, Object);

public:
    explicit ICameraController(Urho3D::Context* context)
        : Object(context) {}
    ~ICameraController() override = default;

    virtual void Initialize(Urho3D::Node& cameraNode) = 0;
    virtual void Shutdown() = 0;

    virtual void SetLookSensitivity(float sensitivity) = 0;
    [[nodiscard]] virtual float GetLookSensitivity() const = 0;
};

} // namespace Radon::Engine::Input
