#pragma once

#include "Engine/Input/CameraMode/ICamera.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

class BaseCameraController : public ICamera
{
    URHO3D_OBJECT(BaseCameraController, ICamera);

public:
    explicit BaseCameraController(Urho3D::Context* context);
    ~BaseCameraController() override;

    void Initialize(Urho3D::Node& cameraNode) override;
    void Shutdown() override;

    void SetLookSensitivity(float sensitivity) override { lookSensitivity_ = sensitivity; }
    [[nodiscard]] float GetLookSensitivity() const override { return lookSensitivity_; }

    void SetMoveSpeed(float speed) override { moveSpeed_ = speed; }
    [[nodiscard]] float GetMoveSpeed() const override { return moveSpeed_; }

    void SetPitchLimits(float min, float max)
    {
        pitchMin_ = min;
        pitchMax_ = max;
    }

protected:
    void UpdateOrientation(float mouseXMove, float mouseYMove);

protected:
    Urho3D::WeakPtr<Urho3D::Node> cameraNode_;

    float pitchMin_{-89.0f};
    float pitchMax_{89.0f};
    float yaw_{0.0f};
    float pitch_{0.0f};

    float lookSensitivity_{0.1f};
    float moveSpeed_{5.0f};

    bool initialized_{false};
};

} // namespace Radon::Engine::Input
