#pragma once

#include "Engine/Input/CameraController/BaseCameraController.hpp"

namespace Radon::Engine::Input
{

class FreeCameraController final : public BaseCameraController
{
    URHO3D_OBJECT(FreeCameraController, BaseCameraController);

public:
    explicit FreeCameraController(Urho3D::Context* context);
    ~FreeCameraController() override;

    void Initialize(Urho3D::Node& cameraNode) override;

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};

} // namespace Radon::Engine::Input
