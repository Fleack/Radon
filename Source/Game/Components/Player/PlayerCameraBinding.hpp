#pragma once

#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Urho3D
{
class Camera;
}

namespace Radon::Game::Components
{

class PlayerCameraBinding final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerCameraBinding, LogicComponent);

public:
    explicit PlayerCameraBinding(Urho3D::Context* context);
    ~PlayerCameraBinding() override;

    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void DelayedStart() override;
    void Update(float timeStep) override;

    void SetCameraHeight(float height) { cameraHeight_ = height; }
    [[nodiscard]] float GetCameraHeight() const { return cameraHeight_; }

    void SetHeadBobStrength(float strength) { headBobStrength_ = strength; }
    [[nodiscard]] float GetHeadBobStrength() const { return headBobStrength_; }

    void SetHeadBobSpeed(float speed) { headBobSpeed_ = speed; }
    [[nodiscard]] float GetHeadBobSpeed() const { return headBobSpeed_; }

    void SetHeadBobHorizontalFactor(float factor) { headBobHorizontalFactor_ = factor; }
    [[nodiscard]] float GetHeadBobHorizontalFactor() const { return headBobHorizontalFactor_; }

    void SetHeadBobVerticalFactor(float factor) { headBobVerticalFactor_ = factor; }
    [[nodiscard]] float GetHeadBobVerticalFactor() const { return headBobVerticalFactor_; }

    void SetCameraNode(Urho3D::Node* node);
    void SetCamera(Urho3D::Camera* camera);

    Urho3D::Vector3 GetCamForward() const;
    Urho3D::Vector3 GetCamRight() const;

private:
    void SubscribeToEvents();

    void ApplyHeadBob(float timeStep);

private:
    Urho3D::Node* cameraNode_{nullptr};
    Urho3D::Camera* camera_{nullptr};

    bool playerMoving_{false};
    bool playerRunning_{false};
    bool playerJumped_{false};
    bool playerGrounded_{false};

    float cameraHeight_{1.8f};
    float cameraSmoothing_{20.0f};

    float headBobStrength_{0.04f};
    float headBobSpeed_{1.5f};
    float headBobTime_{0.0f};
    float headBobHorizontalFactor_{1.0f};
    float headBobVerticalFactor_{1.0f};

    Urho3D::Vector3 originalCameraPosition_{Urho3D::Vector3::ZERO};

    bool initialized_{false};
};

} // namespace Radon::Game::Components
