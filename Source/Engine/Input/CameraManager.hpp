#pragma once

#include "CameraController/CameraController.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Urho3D
{
class Camera;
}

namespace Radon::Engine::Input
{

/// Simplified camera manager that handles FPS and debug camera modes
class CameraManager final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraManager, Object);

public:
    explicit CameraManager(Urho3D::Context* context);
    ~CameraManager() override;

    /// Initialize with player node - creates camera automatically
    void Initialize(Urho3D::Node& playerNode);
    void Shutdown();

    void SetMode(CameraMode mode);
    [[nodiscard]] CameraMode GetMode() const;

    void SetLookSensitivity(float sensitivity);
    [[nodiscard]] float GetLookSensitivity() const;

    void SetMoveSpeed(float speed);
    [[nodiscard]] float GetMoveSpeed() const;

    void SetPlayerNode(Urho3D::Node* playerNode);

    /// Toggle between FPS and DEBUG modes
    void ToggleMode();

    /// Get camera node (created during initialization)
    [[nodiscard]] Urho3D::Node* GetCameraNode() const;
    
    /// Get camera component (created during initialization)
    [[nodiscard]] Urho3D::Camera* GetCamera() const;

private:
    Urho3D::SharedPtr<CameraController> cameraController_;
    bool initialized_{false};
};

} // namespace Radon::Engine::Input
