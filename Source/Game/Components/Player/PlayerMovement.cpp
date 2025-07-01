#include "PlayerMovement.hpp"

#include "Engine/Core/Logger.hpp"
#include "Game/Components/Player/PlayerInputHandler.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

Urho3D::StringHash const PlayerMovement::EVENT_JUMPED("PlayerJumped");
Urho3D::StringHash const PlayerMovement::EVENT_STARTED_MOVING("PlayerStartedMoving");
Urho3D::StringHash const PlayerMovement::EVENT_STOPPED_MOVING("PlayerStoppedMoving");
Urho3D::StringHash const PlayerMovement::EVENT_RUN_STATE_CHANGED("PlayerRunStateChanged");

PlayerMovement::PlayerMovement(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_UPDATE);
}

PlayerMovement::~PlayerMovement() = default;

void PlayerMovement::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerMovement>();
}

void PlayerMovement::Start()
{
    if (initialized_)
        return;

    if (!inputHandler_)
        inputHandler_ = node_->GetComponent<PlayerInputHandler>();
    if (!characterController_)
        characterController_ = node_->GetOrCreateComponent<Urho3D::KinematicCharacterController>();

    if (!inputHandler_)
    {
        RADON_LOGERROR("PlayerMovement: Failed to get PlayerInputHandler component");
        return;
    }

    initialized_ = true;
}

void PlayerMovement::Update(float timeStep)
{
    if (!initialized_ || !inputHandler_ || !characterController_)
        return;

    Urho3D::Vector3 direction = Urho3D::Vector3::ZERO;

    if (inputHandler_->GetMoveForward())
        direction += node_->GetWorldDirection();
    if (inputHandler_->GetMoveBack())
        direction -= node_->GetWorldDirection();
    if (inputHandler_->GetMoveRight())
        direction += node_->GetWorldRight();
    if (inputHandler_->GetMoveLeft())
        direction -= node_->GetWorldRight();

    bool wasMoving = isMoving_;
    bool wasRunning = isRunning_;

    if (!direction.Equals(Urho3D::Vector3::ZERO))
    {
        direction.Normalize();
        isMoving_ = true;
    }
    else
    {
        isMoving_ = false;
    }

    isRunning_ = inputHandler_->GetRun() && isMoving_;
    currentSpeed_ = isRunning_ ? runSpeed_ : walkSpeed_;
    moveDirection_ = direction * currentSpeed_;
    characterController_->SetWalkIncrement(moveDirection_ * timeStep);

    if (isMoving_ && !wasMoving)
        SendEvent(EVENT_STARTED_MOVING);
    if (!isMoving_ && wasMoving)
        SendEvent(EVENT_STOPPED_MOVING);
    if (isRunning_ != wasRunning)
        SendEvent(EVENT_RUN_STATE_CHANGED);

    bool jumpPressed = inputHandler_->GetJump();
    if (jumpPressed && !jumpPressedLastFrame_ && IsGrounded())
    {
        characterController_->Jump();
        SendEvent(EVENT_JUMPED);
    }
    jumpPressedLastFrame_ = jumpPressed;

    node_->SetWorldRotation(Urho3D::Quaternion(inputHandler_->GetMouseYaw(), Urho3D::Vector3::UP)); // TODO Maybe laggy
}

void PlayerMovement::SetJumpHeight(float height)
{
    if (!characterController_)
        return;

    characterController_->SetMaxJumpHeight(height);
}

float PlayerMovement::GetJumpHeight() const
{
    return characterController_ ? characterController_->GetMaxJumpHeight() : 0.0f;
}

bool PlayerMovement::IsGrounded() const
{
    return characterController_ ? characterController_->OnGround() : false;
}

void PlayerMovement::SetInputHandler(PlayerInputHandler* handler)
{
    inputHandler_ = handler;
}

void PlayerMovement::SetCharacterController(Urho3D::KinematicCharacterController* controller)
{
    characterController_ = controller;
}

} // namespace Radon::Game::Components
