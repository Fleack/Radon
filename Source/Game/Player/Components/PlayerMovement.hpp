#pragma once

#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Physics/KinematicCharacterController.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Game::Player
{

class PlayerMovement final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerMovement, LogicComponent);

public:
    explicit PlayerMovement(Urho3D::Context* context);
    ~PlayerMovement() override;

    static void RegisterObject(Urho3D::Context* context);

    void DelayedStart() override;
    void FixedUpdate(float timeStep) override;

    float GetWalkSpeed() const { return walkSpeed_; }
    void SetWalkSpeed(float v) { walkSpeed_ = v; }
    float GetRunSpeed() const { return runSpeed_; }
    void SetRunSpeed(float v) { runSpeed_ = v; }
    float GetJumpHeight() const { return jumpHeight_; }
    void SetJumpHeight(float v) { jumpHeight_ = v; }
    float GetMovementSmoothing() const { return movementSmoothingFactor_; }
    void SetMovementSmoothing(float v) { movementSmoothingFactor_ = v; }

    [[nodiscard]] float GetCurrentSpeed() const { return currentSpeed_; }

    [[nodiscard]] bool IsMoving() const { return isMoving_; }
    [[nodiscard]] bool IsRunning() const { return isRunning_; }
    [[nodiscard]] bool IsGrounded() const { return characterController_ ? characterController_->OnGround() : false; };

    void SetCharacterController(Urho3D::KinematicCharacterController* controller);

private:
    Urho3D::Node* characterControllerNode{nullptr};
    Urho3D::KinematicCharacterController* characterController_{nullptr};

    float walkSpeed_{3.0f};
    float runSpeed_{6.0f};
    float jumpHeight_{6.0f};
    float movementSmoothingFactor_{10.0f};

    float currentSpeed_{0.0f};

    Urho3D::Vector3 moveDirection_{Urho3D::Vector3::ZERO};
    Urho3D::Vector3 targetMoveDirection_{Urho3D::Vector3::ZERO};
    Urho3D::Vector3 lastGroundedDirection_{Urho3D::Vector3::ZERO};
    Urho3D::Vector3 camForward_{Urho3D::Vector3::FORWARD};
    Urho3D::Vector3 camRight_{Urho3D::Vector3::RIGHT};

    bool isMoving_{false};
    bool isRunning_{false};
    bool jumpPressedLastFrame_{false};
    bool groundedLastFrame_{false};

    bool initialized_{false};
};

} // namespace Radon::Game::Player 