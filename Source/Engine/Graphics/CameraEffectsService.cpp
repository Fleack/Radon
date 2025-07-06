#include "CameraEffectsService.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/Effects/CameraShakeEffect.hpp"
#include "Engine/Graphics/Effects/HeadBobEffect.hpp"

#include <Urho3D/Core/CoreEvents.h>

using namespace Urho3D;
using namespace Radon::Engine::Graphics;

CameraEffectsService::CameraEffectsService(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("CameraEffectsService: constructor called");
}

CameraEffectsService::~CameraEffectsService()
{
    RADON_LOGDEBUG("CameraEffectsService: destructor called");
    Shutdown();
}

void CameraEffectsService::Initialize()
{
    if (initialized_)
        return;

    RADON_LOGDEBUG("CameraEffectsService: Initialize called");

    // Create effects manager
    effectsManager_ = MakeShared<CameraEffectsManager>(context_);

    // Temporarily disable effects creation for debugging
    // Add standard camera effects
    // HeadBob effect (moved from PlayerCamera)
    // auto headBobEffect = MakeShared<HeadBobEffect>(context_);
    // effectsManager_->AddEffect(headBobEffect);

    // Add camera shake effect
    // auto shakeEffect = MakeShared<CameraShakeEffect>(context_);
    // effectsManager_->AddEffect(shakeEffect);

    effectsManager_->Initialize();

    initialized_ = true;
    RADON_LOGINFO("CameraEffectsService: Initialized");
}

void CameraEffectsService::Shutdown()
{
    if (!initialized_)
        return;

    RADON_LOGDEBUG("CameraEffectsService: Shutdown called");

    ClearObservedCamera();

    if (effectsManager_)
    {
        effectsManager_->Shutdown();
        effectsManager_ = nullptr;
    }

    initialized_ = false;
    RADON_LOGINFO("CameraEffectsService: Shutdown complete");
}

void CameraEffectsService::SetObservedCamera(Node* cameraNode, Camera* camera)
{
    if (!cameraNode || !camera)
    {
        RADON_LOGWARN("CameraEffectsService: Invalid camera parameters");
        return;
    }

    observedCameraNode_ = cameraNode;
    observedCamera_ = camera;
    cachedBasePosition_ = cameraNode->GetPosition();

    if (!observingCamera_)
    {
        // Subscribe to update events to apply effects each frame
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CameraEffectsService, OnUpdate));
        observingCamera_ = true;
    }

    RADON_LOGINFO("CameraEffectsService: Camera observation started");
}

void CameraEffectsService::ClearObservedCamera()
{
    if (observingCamera_)
    {
        UnsubscribeFromEvent(E_UPDATE);
        observingCamera_ = false;
    }

    observedCameraNode_ = nullptr;
    observedCamera_ = nullptr;
    cachedBasePosition_ = Vector3::ZERO;

    RADON_LOGINFO("CameraEffectsService: Camera observation stopped");
}

void CameraEffectsService::SetIntegrationEnabled(bool enabled)
{
    if (integrationEnabled_ == enabled)
        return;

    integrationEnabled_ = enabled;

    if (!integrationEnabled_)
    {
        ClearObservedCamera();
    }

    RADON_LOGINFO("CameraEffectsService: Integration {}", enabled ? "enabled" : "disabled");
}

void CameraEffectsService::OnUpdate(StringHash, VariantMap& eventData)
{
    if (!integrationEnabled_ || !initialized_)
        return;

    // Apply effects each frame if we have a valid camera
    if (observedCameraNode_.Expired() || observedCamera_.Expired())
    {
        RADON_LOGWARN("CameraEffectsService: Camera references expired, stopping observation");
        ClearObservedCamera();
        return;
    }

    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    // Apply all camera effects using cached base position
    ApplyEffectsToCamera(observedCameraNode_.Get(), observedCamera_.Get(), cachedBasePosition_, timeStep);
}

void CameraEffectsService::ApplyEffectsToCamera(Node* cameraNode, Camera* camera, Vector3 const& basePosition, float timeStep)
{
    if (!effectsManager_ || !cameraNode || !camera)
    {
        RADON_LOGWARN("CameraEffectsService: Invalid objects in ApplyEffectsToCamera");
        return;
    }

    // Get effects result from effects manager
    CameraEffectResult effectResult = effectsManager_->Update(timeStep);

    // Apply position offset to base position
    if (!effectResult.positionOffset.Equals(Vector3::ZERO))
    {
        Vector3 newPosition = basePosition + effectResult.positionOffset;
        cameraNode->SetPosition(newPosition);
    }

    // Apply rotation offset
    if (!effectResult.rotationOffset.Equals(Quaternion::IDENTITY))
    {
        Quaternion currentRot = cameraNode->GetRotation();
        cameraNode->SetRotation(currentRot * effectResult.rotationOffset);
    }

    // Apply FOV multiplier
    if (effectResult.fovMultiplier != 1.0f)
    {
        float currentFov = camera->GetFov();
        camera->SetFov(currentFov * effectResult.fovMultiplier);
    }
}
