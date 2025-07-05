#include "CameraShakeEffect.hpp"

#include "Engine/Graphics/CameraEvents.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Random.h>

using namespace Urho3D;
using namespace Radon::Engine::Graphics;

CameraShakeEffect::CameraShakeEffect(Context* context)
    : ICameraEffect(context)
{
}

void CameraShakeEffect::Initialize()
{
    if (initialized_)
        return;

    SubscribeToShakeEvents();
    initialized_ = true;
}

void CameraShakeEffect::Shutdown()
{
    if (!initialized_)
        return;

    UnsubscribeFromAllEvents();
    shaking_ = false;
    initialized_ = false;
}

void CameraShakeEffect::SubscribeToShakeEvents()
{
    SubscribeToEvent(Events::E_CAMERA_SHAKE_REQUESTED, [this](StringHash, VariantMap& eventData) {
        float duration = eventData[Events::P::DURATION].GetFloat();
        float strength = eventData[Events::P::SHAKE_STRENGTH].GetFloat();
        float frequency = eventData[Events::P::SHAKE_FREQUENCY].GetFloat();

        if (frequency <= 0.0f)
            frequency = frequency_;

        StartShake(duration, strength, frequency);
    });
}

CameraEffectResult CameraShakeEffect::Update(float timeStep)
{
    CameraEffectResult result;

    if (!enabled_ || !shaking_ || intensity_ <= 0.0f)
        return result;

    remainingTime_ -= timeStep;
    if (remainingTime_ <= 0.0f)
    {
        StopShake();
        return result;
    }

    result.positionOffset = GenerateShakeOffset(timeStep);
    result.positionOffset *= intensity_;
    return result;
}

void CameraShakeEffect::StartShake(float duration, float strength, float frequency)
{
    duration_ = duration;
    strength_ = strength;
    frequency_ = frequency;

    shaking_ = true;
    remainingTime_ = duration_;
    shakeTime_ = 0.0f;
}

void CameraShakeEffect::StopShake()
{
    shaking_ = false;
    remainingTime_ = 0.0f;
    shakeOffset_ = Vector3::ZERO;
}

Vector3 CameraShakeEffect::GenerateShakeOffset(float timeStep)
{
    shakeTime_ += timeStep;

    // Затухание эффекта со временем
    float fadeOut = remainingTime_ / duration_;
    float currentStrength = strength_ * fadeOut;

    // Генерируем случайное смещение с заданной частотой
    float phase = shakeTime_ * frequency_;

    Vector3 offset;
    offset.x_ = Sin(phase * 1.3f) * currentStrength * Random(-1.0f, 1.0f);
    offset.y_ = Sin(phase * 0.8f) * currentStrength * Random(-1.0f, 1.0f);
    offset.z_ = Sin(phase * 1.7f) * currentStrength * Random(-1.0f, 1.0f) * 0.5f;

    return offset;
}
