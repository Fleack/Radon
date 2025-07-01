#include "PlayerHealth.hpp"

#include "Game/Components/Events/PlayerEvents.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

PlayerHealth::PlayerHealth(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_NO_EVENT);
}

PlayerHealth::~PlayerHealth() = default;

void PlayerHealth::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerHealth>("Player");

    URHO3D_ATTRIBUTE("MaxHealth", float, maxHealth_, 100.0f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Invulnerable", bool, invulnerable_, false, Urho3D::AM_DEFAULT);
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

void PlayerHealth::DelayedStart()
{
    Start();
}

void PlayerHealth::SetHealth(float health)
{
    health_ = Urho3D::Clamp(health, 0.0f, maxHealth_);
}

void PlayerHealth::SetInvulnerable(bool invuln)
{
    invulnerable_ = invuln;
}

void PlayerHealth::TakeDamage(float amount, Urho3D::Node* source)
{
    if (invulnerable_ || amount <= 0.0f || !IsAlive())
        return;

    float prevHealth = health_;
    health_ -= amount;
    health_ = Urho3D::Clamp(health_, 0.0f, maxHealth_);

    Urho3D::VariantMap eventData;
    eventData[Events::P::SOURCE] = source;
    eventData[Events::P::AMOUNT] = amount;
    eventData[Events::P::PREVIOUS_HEALTH] = prevHealth;
    eventData[Events::P::HEALTH] = health_;
    SendEvent(Events::E_PLAYER_DAMAGED, eventData);

    if (health_ <= 0.0f)
    {
        health_ = 0.0f;
        SendEvent(Events::E_PLAYER_DIED, eventData);
    }
}

void PlayerHealth::Heal(float amount)
{
    if (amount <= 0.0f || !IsAlive())
        return;

    float prevHealth = health_;
    health_ += amount;
    if (health_ > maxHealth_)
        health_ = maxHealth_;

    Urho3D::VariantMap eventData;
    eventData[Events::P::AMOUNT] = amount;
    eventData[Events::P::PREVIOUS_HEALTH] = prevHealth;
    eventData[Events::P::HEALTH] = health_;
    SendEvent(Events::E_PLAYER_HEALED, eventData);
}

void PlayerHealth::Respawn()
{
    health_ = maxHealth_;

    Urho3D::VariantMap eventData;
    eventData[Events::P::HEALTH] = health_;
    SendEvent(Events::E_PLAYER_RESPAWNED, eventData);
}

} // namespace Radon::Game::Components
