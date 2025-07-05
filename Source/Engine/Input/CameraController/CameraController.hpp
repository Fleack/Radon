#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

enum class CameraMode : uint8_t
{
    FPS, // Camera follows player
    DEBUG // Free-flying camera
};

/// Simplified camera controller for FPS and debug modes
class CameraController final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraController, Object);

public:
    explicit CameraController(Urho3D::Context* context);
    ~CameraController() override;

    void Initialize(Urho3D::Node& cameraNode, Urho3D::Node* playerNode = nullptr);
    void Shutdown();

    void SetMode(CameraMode mode);
    [[nodiscard]] CameraMode GetMode() const { return currentMode_; }

    void SetLookSensitivity(float sensitivity) { lookSensitivity_ = sensitivity; }
    [[nodiscard]] float GetLookSensitivity() const { return lookSensitivity_; }

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    [[nodiscard]] float GetMoveSpeed() const { return moveSpeed_; }

    void SetPlayerNode(Urho3D::Node* playerNode) { playerNode_ = playerNode; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void UpdateFPSCamera(float deltaTime);
    void UpdateDebugCamera(float deltaTime);
    void UpdateOrientation(float mouseX, float mouseY);

private:
    Urho3D::WeakPtr<Urho3D::Node> cameraNode_;
    Urho3D::WeakPtr<Urho3D::Node> playerNode_;

    CameraMode currentMode_{CameraMode::FPS};

    float yaw_{0.0f};
    float pitch_{0.0f};
    float lookSensitivity_{0.1f};
    float moveSpeed_{10.0f};

    // For debug mode
    Urho3D::Vector3 debugCameraOffset_{0.0f, 1.8f, 0.0f}; // Height offset from player

    bool initialized_{false};
};

} // namespace Radon::Engine::Input
