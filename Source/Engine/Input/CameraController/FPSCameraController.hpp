#pragma once

#include "Engine/Input/CameraController/BaseCameraController.hpp"

namespace Radon::Engine::Input
{

class FPSCameraController final : public BaseCameraController
{
    URHO3D_OBJECT(FPSCameraController, BaseCameraController);

public:
    explicit FPSCameraController(Urho3D::Context* context);
    ~FPSCameraController() override;

    void Initialize(Urho3D::Node& cameraNode) override;

    void SetPlayerNode(Urho3D::Node* playerNode);

    void SetCameraOffset(Urho3D::Vector3 const& offset) { cameraOffset_ = offset; }
    [[nodiscard]] Urho3D::Vector3 GetCameraOffset() const { return cameraOffset_; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::WeakPtr<Urho3D::Node> playerNode_;
    Urho3D::Vector3 cameraOffset_{0.0f, 1.8f, 0.0f};
};

} // namespace Radon::Engine::Input
