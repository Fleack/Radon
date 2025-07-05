#pragma once

#include "Engine/Graphics/ICameraEffect.hpp"

#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Engine::Graphics
{

class HeadBobEffect final : public ICameraEffect
{
    URHO3D_OBJECT(HeadBobEffect, ICameraEffect);

public:
    explicit HeadBobEffect(Urho3D::Context* context);

    void Initialize() override;
    void Shutdown() override;
    CameraEffectResult Update(float timeStep) override;

    void SetStrength(float strength) { strength_ = strength; }
    [[nodiscard]] float GetStrength() const { return strength_; }

    void SetSpeed(float speed) { speed_ = speed; }
    [[nodiscard]] float GetSpeed() const { return speed_; }

    void SetHorizontalFactor(float factor) { horizontalFactor_ = factor; }
    [[nodiscard]] float GetHorizontalFactor() const { return horizontalFactor_; }

    void SetVerticalFactor(float factor) { verticalFactor_ = factor; }
    [[nodiscard]] float GetVerticalFactor() const { return verticalFactor_; }

    void SetPlayerMoving(bool moving) { playerMoving_ = moving; }
    void SetPlayerRunning(bool running) { playerRunning_ = running; }
    void SetPlayerGrounded(bool grounded) { playerGrounded_ = grounded; }

private:
    void SubscribeToPlayerEvents();

private:
    // Effect params
    float strength_{0.04f};
    float speed_{1.5f};
    float horizontalFactor_{1.0f};
    float verticalFactor_{1.0f};

    // Player state
    bool playerMoving_{false};
    bool playerRunning_{false};
    bool playerGrounded_{true};

    // Inner state
    float bobTime_{0.0f};
    float bobLerp_{0.0f};
    bool initialized_{false};
};

} // namespace Radon::Engine::Graphics
