#pragma once

#include "Urho3D/Physics/KinematicCharacterController.h"

#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Game::Components
{

class PlayerInputHandler;
class PlayerCameraBinding;

class PlayerMovement final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerMovement, LogicComponent);

public:
    explicit PlayerMovement(Urho3D::Context* context);
    ~PlayerMovement() override;

    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void DelayedStart() override;
    void FixedUpdate(float timeStep) override;

    void SetWalkSpeed(float speed) { walkSpeed_ = speed; }
    [[nodiscard]] float GetWalkSpeed() const { return walkSpeed_; }

    void SetRunSpeed(float speed) { runSpeed_ = speed; }
    [[nodiscard]] float GetRunSpeed() const { return runSpeed_; }

    void SetJumpHeight(float height);
    [[nodiscard]] float GetJumpHeight() const;

    [[nodiscard]] float GetCurrentSpeed() const { return currentSpeed_; }

    [[nodiscard]] bool IsMoving() const { return isMoving_; }
    [[nodiscard]] bool IsRunning() const { return isRunning_; }
    [[nodiscard]] bool IsGrounded() const;

    void SetInputHandler(PlayerInputHandler* handler);
    void SetCharacterController(Urho3D::KinematicCharacterController* controller);
    void SetCameraBinding(PlayerCameraBinding* cameraBinding);

    static Urho3D::StringHash const EVENT_JUMPED;
    static Urho3D::StringHash const EVENT_STARTED_MOVING;
    static Urho3D::StringHash const EVENT_STOPPED_MOVING;
    static Urho3D::StringHash const EVENT_RUN_STATE_CHANGED;

private:
    PlayerInputHandler* inputHandler_{nullptr};
    Urho3D::KinematicCharacterController* characterController_{nullptr};
    PlayerCameraBinding* cameraBinding_{nullptr};

    float walkSpeed_{3.0f};
    float runSpeed_{6.0f};
    float currentSpeed_{0.0f};

    Urho3D::Vector3 moveDirection_{Urho3D::Vector3::ZERO};

    bool isMoving_{false};
    bool isRunning_{false};
    bool jumpPressedLastFrame_{false};

    bool initialized_{false};
};

} // namespace Radon::Game::Components
