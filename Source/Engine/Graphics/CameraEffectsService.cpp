#include "CameraEffectsService.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/CameraEvents.hpp"
#include "Engine/Graphics/Effects/HeadBobEffect.hpp"
#include "Engine/Graphics/Effects/CameraShakeEffect.hpp"

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
    
    // Создаём менеджер эффектов
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
    
    StopObservingPlayerCamera();
    
    if (effectsManager_)
    {
        effectsManager_->Shutdown();
        effectsManager_ = nullptr;
    }

    initialized_ = false;
    RADON_LOGINFO("CameraEffectsService: Shutdown complete");
}

void CameraEffectsService::StartObservingPlayerCamera()
{
    if (observingPlayerCamera_)
        return;

    // Subscribe to new PlayerCamera events
    SubscribeToEvent(StringHash("PlayerCameraEvent"), 
                    URHO3D_HANDLER(CameraEffectsService, HandlePlayerCameraEvent));
    
    // Subscribe to update events to apply effects each frame
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CameraEffectsService, OnUpdate));

    observingPlayerCamera_ = true;
    RADON_LOGINFO("CameraEffectsService: Started observing PlayerCamera");
}

void CameraEffectsService::StopObservingPlayerCamera()
{
    if (!observingPlayerCamera_)
        return;

    UnsubscribeFromEvent(StringHash("PlayerCameraEvent"));
    UnsubscribeFromEvent(E_UPDATE);

    observedCameraNode_ = nullptr;
    observedCamera_ = nullptr;
    observingPlayerCamera_ = false;
    
    RADON_LOGINFO("CameraEffectsService: Stopped observing PlayerCamera");
}

void CameraEffectsService::SetIntegrationEnabled(bool enabled)
{
    if (integrationEnabled_ == enabled)
        return;

    integrationEnabled_ = enabled;
    
    if (integrationEnabled_)
    {
        StartObservingPlayerCamera();
    }
    else
    {
        StopObservingPlayerCamera();
    }
    
    RADON_LOGINFO("CameraEffectsService: Integration {}", enabled ? "enabled" : "disabled");
}

void CameraEffectsService::HandlePlayerCameraEvent(StringHash, VariantMap& eventData)
{
    if (!integrationEnabled_ || !initialized_)
        return;

    ea::string eventType = eventData["EventType"].GetString();
    
    if (eventType == "CameraInitialized")
    {
        // Safely get camera references and base position
        auto* cameraNode = static_cast<Node*>(eventData["CameraNode"].GetPtr());
        auto* camera = static_cast<Camera*>(eventData["Camera"].GetPtr());
        
        if (cameraNode && camera)
        {
            observedCameraNode_ = cameraNode;
            observedCamera_ = camera;
            cachedBasePosition_ = eventData["BasePosition"].GetVector3();
            
            RADON_LOGINFO("CameraEffectsService: Camera initialized and cached");
        }
        else
        {
            RADON_LOGWARN("CameraEffectsService: Invalid camera objects in initialization event");
        }
    }
    else if (eventType == "BasePositionChanged")
    {
        // Update cached base position
        cachedBasePosition_ = eventData["BasePosition"].GetVector3();
        RADON_LOGINFO("CameraEffectsService: Base position updated");
    }
}

void CameraEffectsService::OnUpdate(StringHash, VariantMap& eventData)
{
    if (!integrationEnabled_ || !initialized_)
        return;

    // Apply effects each frame if we have a valid camera
    if (observedCameraNode_.Expired() || observedCamera_.Expired())
    {
        RADON_LOGWARN("CameraEffectsService: Camera references expired, stopping observation");
        StopObservingPlayerCamera();
        return;
    }

    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    
    // Apply all camera effects using cached base position
    ApplyEffectsToCamera(observedCameraNode_.Get(), observedCamera_.Get(), cachedBasePosition_, timeStep);
}

void CameraEffectsService::ApplyEffectsToCamera(Node* cameraNode, Camera* camera, const Vector3& basePosition, float timeStep)
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