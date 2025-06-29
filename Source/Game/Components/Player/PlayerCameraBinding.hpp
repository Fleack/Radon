#pragma once

#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Urho3D
{
class Camera;
}

namespace Radon::Game::Components
{

class PlayerInputHandler;
class PlayerMovement;

class PlayerCameraBinding final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerCameraBinding, LogicComponent);

public:
    explicit PlayerCameraBinding(Urho3D::Context* context);
    ~PlayerCameraBinding() override;

    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void Update(float timeStep) override;

    void SetCameraHeight(float height) { cameraHeight_ = height; }
    [[nodiscard]] float GetCameraHeight() const { return cameraHeight_; }

    void SetHeadBobStrength(float strength) { headBobStrength_ = strength; }
    [[nodiscard]] float GetHeadBobStrength() const { return headBobStrength_; }

    void SetHeadBobSpeed(float speed) { headBobSpeed_ = speed; }
    [[nodiscard]] float GetHeadBobSpeed() const { return headBobSpeed_; }

private:
    void ApplyHeadBob(float timeStep);

private:

    PlayerInputHandler* inputHandler_{nullptr};
    PlayerMovement* movement_{nullptr};

    Urho3D::Node* cameraNode_{nullptr};
    Urho3D::Camera* camera_{nullptr};

    float cameraHeight_{1.8f};
    float headBobStrength_{0.05f};
    float headBobSpeed_{10.0f};
    float headBobTime_{0.0f};

    Urho3D::Vector3 originalCameraPosition_{Urho3D::Vector3::ZERO};

    bool initialized_{false};
};

} // namespace Radon::Game::Components
