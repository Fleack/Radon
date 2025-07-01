#include "PlayerComponent.hpp"

#include "Game/Components/Player/PlayerCameraBinding.hpp"
#include "Game/Components/Player/PlayerHealth.hpp"
#include "Game/Components/Player/PlayerInputHandler.hpp"
#include "Game/Components/Player/PlayerMovement.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

PlayerComponent::PlayerComponent(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_NO_EVENT);
}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerComponent>("Player");
}

void PlayerComponent::Start()
{
    if (initialized_)
        return;

    if (!inputHandler_)
        inputHandler_ = node_->GetOrCreateComponent<PlayerInputHandler>();
    if (!cameraBinding_)
        cameraBinding_ = node_->GetOrCreateComponent<PlayerCameraBinding>();
    if (!movement_)
        movement_ = node_->GetOrCreateComponent<PlayerMovement>();
    if (!health_)
        health_ = node_->GetOrCreateComponent<PlayerHealth>();

    initialized_ = true;
}

void PlayerComponent::DelayedStart()
{
    Start();
}

void PlayerComponent::SetInputHandler(PlayerInputHandler* handler)
{
    inputHandler_ = handler;
}

void PlayerComponent::SetMovement(PlayerMovement* movement)
{
    movement_ = movement;
}

void PlayerComponent::SetHealth(PlayerHealth* health)
{
    health_ = health;
}

void PlayerComponent::SetCameraBinding(PlayerCameraBinding* cameraBinding)
{
    cameraBinding_ = cameraBinding;
}

} // namespace Radon::Game::Components
