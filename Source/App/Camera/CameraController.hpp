#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon
{

class CameraController : public Urho3D::Object
{
    URHO3D_OBJECT(CameraController, Object);

public:
    explicit CameraController(Urho3D::Context* context);
    ~CameraController() override;

    void Initialize(Urho3D::Node& cameraNode, float lookSensitivity = 0.1f, float moveSpeed = 5.0f);

    void Shutdown();

    void SetLookSensitivity(float sensitivity) { lookSensitivity_ = sensitivity; }
    [[nodiscard]] float GetLookSensitivity() const { return lookSensitivity_; }

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    [[nodiscard]] float GetMoveSpeed() const { return moveSpeed_; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::WeakPtr<Urho3D::Node> cameraNode_;
    float yaw_{0.0f}, pitch_{0.0f};
    float lookSensitivity_{0.1f};
    float moveSpeed_{5.0f};
    float pitchMin_{-89.0f}, pitchMax_{89.0f};
    bool initialized_{false};
};
} // namespace Radon
