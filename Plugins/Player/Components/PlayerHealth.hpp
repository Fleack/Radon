#pragma once

#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Game::Plugins
{

class PlayerHealth final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerHealth, LogicComponent);

public:
    explicit PlayerHealth(Urho3D::Context* context);

    static void RegisterObject(Urho3D::Context* context);

    [[nodiscard]] float GetHealth() const { return health_; }
    [[nodiscard]] float GetMaxHealth() const { return maxHealth_; }
    [[nodiscard]] float GetHealthPercent() const { return health_ / maxHealth_ * 100.0f; }
    [[nodiscard]] bool IsAlive() const { return health_ > 0.0f; }

    void SetMaxHealth(float v);
    void SetHealth(float v);
    void SetInvulnerable(bool v) { invulnerable_ = v; }
    [[nodiscard]] bool IsInvulnerable() const { return invulnerable_; }

    void TakeDamage(float amount, Urho3D::Node* source = nullptr);
    void Heal(float amount);
    void Respawn();

protected:
    float health_{100.0f};
    float maxHealth_{100.0f};
    bool invulnerable_{false};
    float regenRate_{0.0f};
    bool initialized_{false};
};

} // namespace Radon::Game::Plugins
