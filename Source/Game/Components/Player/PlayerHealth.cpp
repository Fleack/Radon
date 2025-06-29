#include "PlayerHealth.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

Urho3D::StringHash EVENT_PLAYER_DEATH("PlayerDeath");

PlayerHealth::PlayerHealth(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_NO_EVENT);
}

PlayerHealth::~PlayerHealth() = default;

void PlayerHealth::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerHealth>("Player");
}

void PlayerHealth::Start()
{
    if (initialized_)
        return;

    health_ = maxHealth_;

    initialized_ = true;
}

void PlayerHealth::SetMaxHealth(float maxHealth)
{
    if (maxHealth <= 0.0f)
        return;

    maxHealth_ = maxHealth;
    if (health_ > maxHealth_)
        health_ = maxHealth_;
}

void PlayerHealth::TakeDamage(float amount, Urho3D::Node* source)
{
    if (invulnerable_ || amount <= 0.0f || !IsAlive())
        return;

    health_ -= amount;

    if (health_ <= 0.0f)
    {
        health_ = 0.0f;

        Urho3D::VariantMap eventData;
        eventData["Source"] = source;
        SendEvent(EVENT_PLAYER_DEATH, eventData);
    }
}

void PlayerHealth::Heal(float amount)
{
    if (amount <= 0.0f || !IsAlive())
        return;

    health_ += amount;
    if (health_ > maxHealth_)
        health_ = maxHealth_;
}

void PlayerHealth::Respawn()
{
    health_ = maxHealth_;
}

} // namespace Radon::Game::Components
