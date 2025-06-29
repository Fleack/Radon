#include "PlayerComponent.hpp"
#include "PlayerInputHandler.hpp"
#include "PlayerMovement.hpp"
#include "PlayerHealth.hpp"
#include "PlayerCameraBinding.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

PlayerComponent::PlayerComponent(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_UPDATE);
}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerComponent>();
}

void PlayerComponent::Start()
{
    if (initialized_)
        return;

    CreateComponents();
    initialized_ = true;
}

void PlayerComponent::Update(float timeStep)
{

}

void PlayerComponent::CreateComponents()
{
    inputHandler_ = node_->CreateComponent<PlayerInputHandler>();
    movement_ = node_->CreateComponent<PlayerMovement>();
    health_ = node_->CreateComponent<PlayerHealth>();
    cameraBinding_ = node_->CreateComponent<PlayerCameraBinding>();
}

} // namespace Radon::Game::Components
