#include "PlayerInputHandler.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Input/InputHandler.hpp"

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

    interact_ = false; // TODO: реализовать взаимодействие

    mouseYaw_ += inputHandler_->GetMouseDeltaX() * mouseSensitivity_;
    mousePitch_ += inputHandler_->GetMouseDeltaY() * mouseSensitivity_;

    mousePitch_ = Urho3D::Clamp(mousePitch_, -80.0f, 80.0f);
}

} // namespace Radon::Game::Components
