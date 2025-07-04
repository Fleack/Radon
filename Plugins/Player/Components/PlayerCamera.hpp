#pragma once

#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Urho3D
{
class Camera;
}

namespace Radon::Game::Plugins
{

class PlayerCamera final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerCamera, LogicComponent);

public:
    explicit PlayerCamera(Urho3D::Context* context);
    static void RegisterObject(Urho3D::Context* context);

    void DelayedStart() override;
    void Update(float timeStep) override;

    float GetCameraHeight() const { return cameraHeight_; }
    float GetHeadBobStrength() const { return headBobStrength_; }
    float GetHeadBobSpeed() const { return headBobSpeed_; }
    float GetHeadBobHorizontalFactor() const { return headBobHorizontalFactor_; }
    float GetHeadBobVerticalFactor() const { return headBobVerticalFactor_; }

    void SetCameraHeight(float v)
    {
        cameraHeight_ = v;
        UpdateCameraPosition();
    }
    void SetHeadBobStrength(float v) { headBobStrength_ = v; }
    void SetHeadBobSpeed(float v) { headBobSpeed_ = v; }
    void SetHeadBobHorizontalFactor(float v) { headBobHorizontalFactor_ = v; }
    void SetHeadBobVerticalFactor(float v) { headBobVerticalFactor_ = v; }

    [[nodiscard]] Urho3D::Node* GetCameraNode() const { return cameraNode_; }
    [[nodiscard]] Urho3D::Camera* GetCamera() const { return camera_; }

    void SetCameraNode(Urho3D::Node* node) { cameraNode_ = node; }
    void SetCamera(Urho3D::Camera* camera) { camera_ = camera; }

    Urho3D::Vector3 GetCamForward() const;
    Urho3D::Vector3 GetCamRight() const;

private:
    void SubscribeToEvents();
    void ApplyHeadBob(float timeStep);
    void UpdateCameraPosition();

private:
    Urho3D::Node* cameraNode_{nullptr};
    Urho3D::Camera* camera_{nullptr};

    bool playerMoving_{false};
    bool playerRunning_{false};
    bool playerJumped_{false};
    bool playerGrounded_{false};

    float cameraHeight_{1.8f};

    float headBobStrength_{0.04f};
    float headBobSpeed_{1.5f};
    float headBobTime_{0.0f};
    float headBobHorizontalFactor_{1.0f};
    float headBobVerticalFactor_{1.0f};
    float bobLerp_{0.0f};

    // Базовая позиция камеры (без эффектов)
    Urho3D::Vector3 basePosition_{Urho3D::Vector3::ZERO};

    bool initialized_{false};
};

} // namespace Radon::Game::Plugins
