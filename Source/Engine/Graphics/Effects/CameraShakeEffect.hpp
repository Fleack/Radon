#pragma once

#include "Engine/Graphics/ICameraEffect.hpp"

namespace Radon::Engine::Graphics
{

class CameraShakeEffect final : public ICameraEffect
{
    URHO3D_OBJECT(CameraShakeEffect, ICameraEffect);

public:
    explicit CameraShakeEffect(Urho3D::Context* context);

    void Initialize() override;
    void Shutdown() override;
    CameraEffectResult Update(float timeStep) override;

    void StartShake(float duration, float strength, float frequency = 30.0f);
    void StopShake();

    void SetStrength(float const strength) { strength_ = strength; }
    [[nodiscard]] float GetStrength() const { return strength_; }

    void SetFrequency(float const frequency) { frequency_ = frequency; }
    [[nodiscard]] float GetFrequency() const { return frequency_; }

    void SetDuration(float const duration) { duration_ = duration; }
    [[nodiscard]] float GetDuration() const { return duration_; }

    [[nodiscard]] bool IsShaking() const { return shaking_; }
    [[nodiscard]] float GetRemainingTime() const { return remainingTime_; }

private:
    void SubscribeToShakeEvents();
    Urho3D::Vector3 GenerateShakeOffset(float timeStep);

private:
    // Effect params
    float strength_{1.0f};
    float frequency_{30.0f};
    float duration_{0.5f};

    // Effect state
    bool shaking_{false};
    float remainingTime_{0.0f};
    float shakeTime_{0.0f};

    Urho3D::Vector3 shakeOffset_{Urho3D::Vector3::ZERO};
    bool initialized_{false};
};

} // namespace Radon::Engine::Graphics
