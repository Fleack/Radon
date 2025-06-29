#pragma once

#include "CameraMode/FreeCameraController.hpp"
#include "CameraMode/ICamera.hpp"
#include "Engine/Input/CameraMode/FPSCameraController.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

enum class CameraMode
{
    FREE_CAMERA,
    FPS_CAMERA,
};

class CameraManager final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraManager, Object);

public:
    explicit CameraManager(Urho3D::Context* context);
    ~CameraManager() override;

    void Initialize(Urho3D::Node& cameraNode, float lookSensitivity = 0.1f, float moveSpeed = 5.0f);
    void Shutdown();

    void SetCameraMode(CameraMode mode);
    [[nodiscard]] CameraMode GetCurrentCameraMode() const { return currentMode_; }

    [[nodiscard]] ICamera* GetCurrentCamera() const { return currentCamera_; }

    [[nodiscard]] FreeCameraController* GetFreeCamera() const { return freeCamera_; }
    [[nodiscard]] FPSCameraController* GetFPSCamera() const { return fpsCamera_; }

    void SetLookSensitivity(float sensitivity) const;
    [[nodiscard]] float GetLookSensitivity() const;

    void SetMoveSpeed(float speed) const;
    [[nodiscard]] float GetMoveSpeed() const;

private:
    Urho3D::WeakPtr<Urho3D::Node> cameraNode_;

    Urho3D::SharedPtr<FreeCameraController> freeCamera_{nullptr};
    Urho3D::SharedPtr<FPSCameraController> fpsCamera_{nullptr};

    CameraMode currentMode_{CameraMode::FREE_CAMERA};
    ICamera* currentCamera_{nullptr};

    bool initialized_{false};
};

} // namespace Radon::Engine::Input
