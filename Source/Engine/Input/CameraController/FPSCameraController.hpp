#pragma once

#include "BaseCameraController.hpp"

#include <Urho3D/Math/Vector3.h>

namespace Radon::Engine::Input
{

/// FPS camera controller that manages camera movement and rotation
/// Simplified version without camera offset logic
class FPSCameraController final : public BaseCameraController
{
    URHO3D_OBJECT(FPSCameraController, BaseCameraController);

public:
    explicit FPSCameraController(Urho3D::Context* context);
    ~FPSCameraController() override;

    void Initialize(Urho3D::Node& cameraNode) override;
    void Shutdown() override;

    /// Get current camera direction for movement calculations
    [[nodiscard]] Urho3D::Vector3 GetForwardDirection() const;
    [[nodiscard]] Urho3D::Vector3 GetRightDirection() const;

    /// Get current pitch and yaw values
    [[nodiscard]] float GetPitch() const { return pitch_; }
    [[nodiscard]] float GetYaw() const { return yaw_; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void NotifyDirectionChanged();

private:
    Urho3D::Quaternion lastRotation_;
    bool directionChanged_{false};
};

} // namespace Radon::Engine::Input
