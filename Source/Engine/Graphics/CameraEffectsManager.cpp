#include "CameraEffectsManager.hpp"

#include "Engine/Core/Logger.hpp"

using namespace Urho3D;
using namespace Radon::Engine::Graphics;

CameraEffectsManager::CameraEffectsManager(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("CameraEffectsManager: constructor called");
}

CameraEffectsManager::~CameraEffectsManager()
{
    RADON_LOGDEBUG("CameraEffectsManager: destructor called");
    Shutdown();
}

void CameraEffectsManager::Initialize()
{
    if (initialized_)
        return;

    RADON_LOGDEBUG("CameraEffectsManager: Initialize called");

    for (auto& effect : effects_)
    {
        if (effect)
            effect->Initialize();
    }

    initialized_ = true;
    RADON_LOGINFO("CameraEffectsManager: Initialized with {} effects", effects_.size());
}

void CameraEffectsManager::Shutdown()
{
    if (!initialized_)
        return;

    RADON_LOGDEBUG("CameraEffectsManager: Shutdown called");

    for (auto& effect : effects_)
    {
        if (effect)
            effect->Shutdown();
    }

    initialized_ = false;
    RADON_LOGINFO("CameraEffectsManager: Shutdown complete");
}

CameraEffectResult CameraEffectsManager::Update(float timeStep)
{
    CameraEffectResult totalResult;

    if (!initialized_)
        return totalResult;

    for (auto& effect : effects_)
    {
        if (!effect || !effect->IsEnabled())
            continue;

        CameraEffectResult effectResult = effect->Update(timeStep);

        effectResult.positionOffset *= globalIntensity_;
        if (globalIntensity_ < 1.0f)
        {
            effectResult.rotationOffset = Quaternion::IDENTITY.Slerp(effectResult.rotationOffset, globalIntensity_);
        }

        totalResult += effectResult;
    }

    return totalResult;
}

void CameraEffectsManager::AddEffect(SharedPtr<ICameraEffect> effect)
{
    if (!effect)
    {
        RADON_LOGWARN("CameraEffectsManager: Attempted to add null effect");
        return;
    }

    effects_.push_back(effect);

    if (initialized_)
        effect->Initialize();

    RADON_LOGINFO("CameraEffectsManager: Added effect, total: {}", effects_.size());
}

void CameraEffectsManager::RemoveEffect(ICameraEffect const* effect)
{
    if (!effect)
        return;

    for (auto it = effects_.begin(); it != effects_.end(); ++it)
    {
        if (it->Get() == effect)
        {
            (*it)->Shutdown();
            effects_.erase(it);
            RADON_LOGINFO("CameraEffectsManager: Removed effect, total: {}", effects_.size());
            return;
        }
    }
}

void CameraEffectsManager::ClearEffects()
{
    for (auto& effect : effects_)
    {
        if (effect)
            effect->Shutdown();
    }

    effects_.clear();
    RADON_LOGINFO("CameraEffectsManager: Cleared all effects");
}

void CameraEffectsManager::SetAllEffectsEnabled(bool enabled)
{
    for (auto& effect : effects_)
    {
        if (effect)
            effect->SetEnabled(enabled);
    }

    RADON_LOGINFO("CameraEffectsManager: Set all effects enabled: {}", enabled);
}

void CameraEffectsManager::SetGlobalIntensity(float intensity)
{
    globalIntensity_ = Clamp(intensity, 0.0f, 1.0f);
    RADON_LOGINFO("CameraEffectsManager: Set global intensity: {:.2f}", globalIntensity_);
}
