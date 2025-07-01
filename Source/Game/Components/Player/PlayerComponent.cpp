#include "PlayerComponent.hpp"

#include "Game/Components/Player/PlayerCameraBinding.hpp"
#include "Game/Components/Player/PlayerHealth.hpp"
#include "Game/Components/Player/PlayerInputHandler.hpp"
#include "Game/Components/Player/PlayerMovement.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_DAMAGED("PlayerDamaged");
Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_HEALED("PlayerHealed");
Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_DIED("PlayerDied");
Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_JUMPED("PlayerJumped");
Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_STARTED_MOVING("PlayerStartedMoving");
Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_STOPPED_MOVING("PlayerStoppedMoving");
Urho3D::StringHash const PlayerComponent::EVENT_PLAYER_INTERACTED("PlayerInteracted");

PlayerComponent::PlayerComponent(Urho3D::Context* context)
    : LogicComponent(context)
{}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerComponent>();
}

void PlayerComponent::Start()
{
    if (initialized_)
        return;

    if (!inputHandler_)
        inputHandler_ = node_->GetOrCreateComponent<PlayerInputHandler>();
    if (!movement_)
        movement_ = node_->GetOrCreateComponent<PlayerMovement>();
    if (!health_)
        health_ = node_->GetOrCreateComponent<PlayerHealth>();
    if (!cameraBinding_)
        cameraBinding_ = node_->GetOrCreateComponent<PlayerCameraBinding>();

    SubscribeToComponentEvents();
    initialized_ = true;
}

void PlayerComponent::Update(float timeStep)
{
}

void PlayerComponent::SubscribeToComponentEvents()
{
    if (health_)
    {
        SubscribeToEvent(health_, PlayerHealth::EVENT_DAMAGED, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_DAMAGED, data);
        });
        SubscribeToEvent(health_, PlayerHealth::EVENT_HEALED, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_HEALED, data);
        });
        SubscribeToEvent(health_, PlayerHealth::EVENT_DIED, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_DIED, data);
        });
    }

    if (movement_)
    {
        SubscribeToEvent(movement_, PlayerMovement::EVENT_JUMPED, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_JUMPED, data);
        });
        SubscribeToEvent(movement_, PlayerMovement::EVENT_STARTED_MOVING, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_STARTED_MOVING, data);
        });
        SubscribeToEvent(movement_, PlayerMovement::EVENT_STOPPED_MOVING, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_STOPPED_MOVING, data);
        });
    }

    if (inputHandler_)
    {
        SubscribeToEvent(inputHandler_, PlayerInputHandler::EVENT_INTERACTED, [this](Urho3D::StringHash, Urho3D::VariantMap& data) {
            SendEvent(EVENT_PLAYER_INTERACTED, data);
        });
    }
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
