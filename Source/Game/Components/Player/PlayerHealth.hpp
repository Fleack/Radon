#pragma once

#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Game::Components
{

class PlayerHealth final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerHealth, LogicComponent);

public:
    explicit PlayerHealth(Urho3D::Context* context);
    ~PlayerHealth() override;

    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void DelayedStart() override;

    [[nodiscard]] float GetHealth() const { return health_; }
    [[nodiscard]] float GetMaxHealth() const { return maxHealth_; }
    [[nodiscard]] float GetHealthPercent() const { return health_ / maxHealth_ * 100.0f; }
    [[nodiscard]] bool IsAlive() const { return health_ > 0.0f; }

    void SetMaxHealth(float maxHealth);
    void SetHealth(float health);
    void SetInvulnerable(bool invuln);
    [[nodiscard]] bool IsInvulnerable() const { return invulnerable_; }

    void TakeDamage(float amount, Urho3D::Node* source = nullptr);
    void Heal(float amount);
    void Respawn();

private:
    float health_{100.0f};
    float maxHealth_{100.0f};
    bool invulnerable_{false};

    bool initialized_{false};
};

} // namespace Radon::Game::Components
