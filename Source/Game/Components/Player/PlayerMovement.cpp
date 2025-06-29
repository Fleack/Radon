#include "PlayerMovement.hpp"

#include "Engine/Core/Logger.hpp"
#include "Game/Components/Player/PlayerInputHandler.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

PlayerMovement::PlayerMovement(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_FIXEDUPDATE);
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

    inputHandler_ = node_->GetComponent<PlayerInputHandler>();
    if (!inputHandler_)
    {
        RADON_LOGERROR("PlayerMovement: Failed to get PlayerInputHandler component");
        return;
    }

    auto* characterNode = node_->GetChild("Character");
    if (!characterNode)
    {
        RADON_LOGERROR("PlayerMovement: Failed to find 'Character' child node");
        return;
    }

    characterController_ = characterNode->GetComponent<Urho3D::KinematicCharacterController>();
    if (!characterController_)
    {
        RADON_LOGERROR("PlayerMovement: Failed to get KinematicCharacterController component");
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

    bool jumpPressed = inputHandler_->GetJump();
    if (jumpPressed && !jumpPressedLastFrame_ && IsGrounded())
        characterController_->Jump();
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

} // namespace Radon::Game::Components
