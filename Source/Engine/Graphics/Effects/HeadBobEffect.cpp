#include "HeadBobEffect.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/MathDefs.h>

// TODO: This should be resolved by proper plugin architecture
// For now, we'll use StringHash until Events/PlayerEvents.hpp is accessible
using namespace Urho3D;
using namespace Radon::Engine::Graphics;

HeadBobEffect::HeadBobEffect(Context* context)
    : ICameraEffect(context)
{
}

void HeadBobEffect::Initialize()
{
    if (initialized_)
        return;

    SubscribeToPlayerEvents();
    initialized_ = true;
}

void HeadBobEffect::Shutdown()
{
    if (!initialized_)
        return;

    UnsubscribeFromAllEvents();
    initialized_ = false;
}

void HeadBobEffect::SubscribeToPlayerEvents()
{
    // Subscribe to proper player events
    // TODO: Include Events/PlayerEvents.hpp when plugin architecture allows it
    SubscribeToEvent(StringHash("PlayerStartedMoving"), [this](StringHash, VariantMap&) {
        SetPlayerMoving(true);
    });

    SubscribeToEvent(StringHash("PlayerStoppedMoving"), [this](StringHash, VariantMap&) {
        SetPlayerMoving(false);
    });

    SubscribeToEvent(StringHash("PlayerRunStateChanged"), [this](StringHash, VariantMap& eventData) {
        SetPlayerRunning(eventData["IsRunning"].GetBool());
    });

    SubscribeToEvent(StringHash("PlayerGrounded"), [this](StringHash, VariantMap&) {
        SetPlayerGrounded(true);
    });

    SubscribeToEvent(StringHash("PlayerJumped"), [this](StringHash, VariantMap&) {
        SetPlayerGrounded(false);
    });
}

CameraEffectResult HeadBobEffect::Update(float timeStep)
{
    CameraEffectResult result;

    if (!enabled_ || !playerGrounded_ || intensity_ <= 0.0f)
        return result;

    if (playerMoving_)
    {
        float speed = playerRunning_ ? speed_ * 1.5f : speed_;
        bobTime_ += timeStep * speed;
        bobLerp_ = Min(bobLerp_ + timeStep * 2.0f, 1.0f);
    }
    else
    {
        bobLerp_ -= timeStep * 3.0f;
        if (bobLerp_ < 0.0f)
            bobLerp_ = 0.0f;
    }

    if (bobLerp_ > 0.0f)
    {
        float effectiveIntensity = playerRunning_ ? strength_ : strength_ * 0.7f;
        effectiveIntensity *= intensity_ * bobLerp_;

        result.positionOffset.y_ = Sin(bobTime_ * 360.0f) * effectiveIntensity * verticalFactor_;
        result.positionOffset.x_ = Sin(bobTime_ * 180.0f) * effectiveIntensity * horizontalFactor_;
    }

    return result;
}
