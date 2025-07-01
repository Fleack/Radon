#include "PlayerMovement.hpp"

#include "Engine/Core/Logger.hpp"
#include "Game/Components/Events/PlayerEvents.hpp"

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

    if (!characterController_)
        characterController_ = node_->GetOrCreateComponent<Urho3D::KinematicCharacterController>();

    SubscribeToEvent(Events::E_PLAYER_CAMERA_DIRECTION_CHANGED, [this](Urho3D::StringHash, Urho3D::VariantMap& eventData) {
        camForward_ = eventData[Events::P::FORWARD].GetVector3();
        camRight_ = eventData[Events::P::RIGHT].GetVector3();
    });

    SetJumpHeight(jumpHeight_);

    initialized_ = true;
}

void PlayerMovement::DelayedStart()
{
    Start();
}

void PlayerMovement::FixedUpdate(float timeStep)
{
    if (!initialized_ || !characterController_)
        return;

    bool moveForward = GetGlobalVar("PlayerMoveForward").GetBool();
    bool moveBack = GetGlobalVar("PlayerMoveBack").GetBool();
    bool moveLeft = GetGlobalVar("PlayerMoveLeft").GetBool();
    bool moveRight = GetGlobalVar("PlayerMoveRight").GetBool();
    bool run = GetGlobalVar("PlayerRun").GetBool();
    bool jump = GetGlobalVar("PlayerJump").GetBool();
    bool wasMoving = isMoving_;
    bool wasRunning = isRunning_;

    Urho3D::Vector3 direction = Urho3D::Vector3::ZERO;
    if (moveForward)
        direction += camForward_;
    if (moveBack)
        direction -= camForward_;
    if (moveRight)
        direction += camRight_;
    if (moveLeft)
        direction -= camRight_;

    if (!direction.Equals(Urho3D::Vector3::ZERO))
    {
        direction.Normalize();
        isMoving_ = true;
    }
    else
    {
        isMoving_ = false;
    }

    isRunning_ = run && isMoving_;
    currentSpeed_ = isRunning_ ? runSpeed_ : walkSpeed_;
    moveDirection_ = direction * currentSpeed_;
    characterController_->SetWalkIncrement(moveDirection_ * timeStep);

    if (isMoving_ && !wasMoving)
    {
        Urho3D::VariantMap eventData;
        eventData[Events::P::SPEED] = currentSpeed_;
        SendEvent(Events::E_PLAYER_STARTED_MOVING, eventData);
    }

    if (!isMoving_ && wasMoving)
    {
        SendEvent(Events::E_PLAYER_STOPPED_MOVING);
    }

    if (isRunning_ != wasRunning)
    {
        Urho3D::VariantMap eventData;
        eventData[Events::P::IS_RUNNING] = isRunning_;
        SendEvent(Events::E_PLAYER_RUN_STATE_CHANGED, eventData);
    }

    if (jump && !jumpPressedLastFrame_ && IsGrounded())
    {
        characterController_->Jump({0.0f, jumpHeight_, 0.0f});

        Urho3D::VariantMap eventData;
        eventData[Events::P::JUMP_HEIGHT] = jumpHeight_;
        SendEvent(Events::E_PLAYER_JUMPED);
    }
    jumpPressedLastFrame_ = jump;
}

void PlayerMovement::SetJumpHeight(float height)
{
    jumpHeight_ = height;
}

float PlayerMovement::GetJumpHeight() const
{
    return jumpHeight_;
}

bool PlayerMovement::IsGrounded() const
{
    return characterController_ ? characterController_->OnGround() : false;
}

void PlayerMovement::SetCharacterController(Urho3D::KinematicCharacterController* controller)
{
    characterController_ = controller;
}

} // namespace Radon::Game::Components
