#include "PlayerInputHandler.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Input/InputHandler.hpp"
#include "Game/Components/Events/PlayerEvents.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Input/Input.h>

namespace Radon::Game::Components
{

PlayerInputHandler::PlayerInputHandler(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_UPDATE);
}

PlayerInputHandler::~PlayerInputHandler() = default;

void PlayerInputHandler::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerInputHandler>("Player");

    URHO3D_ATTRIBUTE("MouseSensitivity", float, mouseSensitivity_, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("MouseSmoothing", float, mouseSmoothing_, 0.5f, Urho3D::AM_DEFAULT);
}

void PlayerInputHandler::Start()
{
    if (initialized_)
        return;

    inputHandler_ = context_->GetSubsystem<Engine::Input::InputHandler>();
    if (!inputHandler_)
    {
        RADON_LOGERROR("PlayerInputHandler: Failed to get InputHandler subsystem");
        return;
    }

    initialized_ = true;
}

void PlayerInputHandler::DelayedStart()
{
    Start();
}

void PlayerInputHandler::SetInputHandler(Radon::Engine::Input::InputHandler* handler)
{
    inputHandler_ = handler;
}

void PlayerInputHandler::Update(float timeStep)
{
    if (!initialized_ || !inputHandler_)
        return;

    moveForward_ = inputHandler_->GetForwardMove();
    moveBack_ = inputHandler_->GetBackMove();
    moveLeft_ = inputHandler_->GetLeftMove();
    moveRight_ = inputHandler_->GetRightMove();

    jump_ = inputHandler_->GetUpMove();
    run_ = inputHandler_->GetDownMove();

    SetGlobalVar("PlayerMoveForward", moveForward_);
    SetGlobalVar("PlayerMoveBack", moveBack_);
    SetGlobalVar("PlayerMoveLeft", moveLeft_);
    SetGlobalVar("PlayerMoveRight", moveRight_);
    SetGlobalVar("PlayerJump", jump_);
    SetGlobalVar("PlayerRun", run_);

    bool prevInteract = interact_;
    interact_ = inputHandler_->GetInteract();
    SetGlobalVar("PlayerInteract", interact_);

    if (interact_ && !prevInteract)
    {
        SendEvent(Events::E_PLAYER_INTERACTED);
    }

    float rawDeltaX = inputHandler_->GetMouseDeltaX() * mouseSensitivity_;
    float rawDeltaY = inputHandler_->GetMouseDeltaY() * mouseSensitivity_;

    static float smoothedDeltaX = 0.0f;
    static float smoothedDeltaY = 0.0f;

    smoothedDeltaX = smoothedDeltaX * mouseSmoothing_ + rawDeltaX * (1.0f - mouseSmoothing_);
    smoothedDeltaY = smoothedDeltaY * mouseSmoothing_ + rawDeltaY * (1.0f - mouseSmoothing_);

    mouseYaw_ += smoothedDeltaX;
    mousePitch_ += smoothedDeltaY;

    mousePitch_ = Urho3D::Clamp(mousePitch_, -80.0f, 80.0f);
    if (mouseYaw_ > 360.0f || mouseYaw_ < -360.0f)
        mouseYaw_ = std::fmod(mouseYaw_, 360.0f);

    SetGlobalVar("PlayerMouseYaw", mouseYaw_);
    SetGlobalVar("PlayerMousePitch", mousePitch_);
}

} // namespace Radon::Game::Components
