#include "PlayerMovement.hpp"

#include "ComponentCategory.hpp"
#include "Events/GlobalVars.hpp"
#include "Events/PlayerEvents.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Plugins
{

PlayerMovement::PlayerMovement(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_FIXEDUPDATE);
}

PlayerMovement::~PlayerMovement() = default;

void PlayerMovement::RegisterObject(Urho3D::Context* context)
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    context->AddFactoryReflection<PlayerMovement>(Category_Radon_Player);

    URHO3D_ATTRIBUTE("Walk Speed", float, walkSpeed_, 3.0f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Run Speed", float, runSpeed_, 6.0f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Jump Height", float, jumpHeight_, 6.0f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Movement Smoothing", float, movementSmoothingFactor_, 10.0f, Urho3D::AM_DEFAULT);
}

void PlayerMovement::DelayedStart()
{
    if (initialized_)
        return;

    if (!characterControllerNode)
        characterControllerNode = node_->GetChild("Character Controller");
    if (!characterControllerNode)
        characterControllerNode = node_->CreateChild("Character Controller");

    if (!characterController_)
        characterController_ = characterControllerNode->GetOrCreateComponent<Urho3D::KinematicCharacterController>();

    characterControllerNode->SetPosition(Urho3D::Vector3::ZERO);

    SubscribeToEvent(Events::E_PLAYER_CAMERA_DIRECTION_CHANGED, [this](Urho3D::StringHash, Urho3D::VariantMap& eventData) {
        camForward_ = eventData[Events::P::FORWARD].GetVector3();
        camRight_ = eventData[Events::P::RIGHT].GetVector3();
    });

    SetJumpHeight(jumpHeight_);

    initialized_ = true;
}

void PlayerMovement::FixedUpdate(float timeStep)
{
    if (!initialized_ || !characterController_)
        return;

    bool const grounded = IsGrounded();

    bool moveForward = GetGlobalVar(GlobalVars::PLAYER_MOVE_FORWARD).GetBool();
    bool moveBack = GetGlobalVar(GlobalVars::PLAYER_MOVE_BACK).GetBool();
    bool moveLeft = GetGlobalVar(GlobalVars::PLAYER_MOVE_LEFT).GetBool();
    bool moveRight = GetGlobalVar(GlobalVars::PLAYER_MOVE_RIGHT).GetBool();
    bool run = GetGlobalVar(GlobalVars::PLAYER_RUN).GetBool();
    bool jump = GetGlobalVar(GlobalVars::PLAYER_JUMP).GetBool();
    bool wasMoving = isMoving_;
    bool wasRunning = isRunning_;

    Urho3D::Vector3 direction = Urho3D::Vector3::ZERO;
    if (grounded) [[likely]]
    {
        if (moveForward)
            direction += camForward_;
        if (moveBack)
            direction -= camForward_;
        if (moveRight)
            direction += camRight_;
        if (moveLeft)
            direction -= camRight_;
    }

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

    if (grounded) [[likely]]
    {
        targetMoveDirection_ = direction * currentSpeed_;
        lastGroundedDirection_ = targetMoveDirection_;
        float smoothing = Urho3D::Min(1.0f, timeStep * movementSmoothingFactor_);
        moveDirection_ = moveDirection_.Lerp(targetMoveDirection_, smoothing);

        if (!isMoving_ && moveDirection_.Length() < 0.1f)
            moveDirection_ = Urho3D::Vector3::ZERO;
    }
    else
    {
        if (moveDirection_ == Urho3D::Vector3::ZERO && lastGroundedDirection_ != Urho3D::Vector3::ZERO)
            moveDirection_ = lastGroundedDirection_;
    }

    characterController_->SetWalkIncrement(moveDirection_ * timeStep);

    // Оптимизировано: обновляем позицию узла игрока только при необходимости
    Urho3D::Vector3 controllerLocalPosition = characterControllerNode->GetPosition();
    if (!controllerLocalPosition.Equals(Urho3D::Vector3::ZERO))
    {
        // Перемещаем основной узел игрока
        node_->SetPosition(node_->GetPosition() + controllerLocalPosition);
        characterControllerNode->SetPosition(Urho3D::Vector3::ZERO);
    }

    // Отправляем только значимые события состояния
    if (isMoving_ && !wasMoving && grounded)
    {
        Urho3D::VariantMap eventData;
        eventData[Events::P::SPEED] = currentSpeed_;
        SendEvent(Events::E_PLAYER_STARTED_MOVING, eventData);
    }

    if (!isMoving_ && wasMoving && grounded)
    {
        SendEvent(Events::E_PLAYER_STOPPED_MOVING);
    }

    if (isRunning_ != wasRunning && grounded)
    {
        Urho3D::VariantMap eventData;
        eventData[Events::P::IS_RUNNING] = isRunning_;
        SendEvent(Events::E_PLAYER_RUN_STATE_CHANGED, eventData);
    }

    if (jump && !jumpPressedLastFrame_ && grounded)
    {
        characterController_->Jump({0.0f, jumpHeight_, 0.0f});

        Urho3D::VariantMap eventData;
        eventData[Events::P::JUMP_HEIGHT] = jumpHeight_;
        SendEvent(Events::E_PLAYER_JUMPED, eventData);
    }

    if (!groundedLastFrame_ && grounded)
    {
        SendEvent(Events::E_PLAYER_GROUNDED);
    }

    groundedLastFrame_ = grounded;
    jumpPressedLastFrame_ = jump;
}

void PlayerMovement::SetCharacterController(Urho3D::KinematicCharacterController* controller)
{
    characterController_ = controller;
}

} // namespace Radon::Game::Plugins
