#include "PlayerCameraService.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/CameraEvents.hpp"

#include <Urho3D/Math/Vector3.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

PlayerCameraService::PlayerCameraService(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("PlayerCameraService: constructor called");
}

PlayerCameraService::~PlayerCameraService()
{
    RADON_LOGDEBUG("PlayerCameraService: destructor called");
    Shutdown();
}

void PlayerCameraService::Initialize()
{
    if (initialized_)
        return;

    RADON_LOGDEBUG("PlayerCameraService: Initialize called");

    // Subscribe to player camera ready event
    SubscribeToEvent(StringHash("PlayerCameraReady"), 
                    URHO3D_HANDLER(PlayerCameraService, OnPlayerCameraReady));
    
    // Subscribe to camera direction changes from FPS controller
    SubscribeToEvent(Graphics::Events::E_CAMERA_BASE_TRANSFORM_CHANGED,
                    URHO3D_HANDLER(PlayerCameraService, OnCameraDirectionChanged));

    initialized_ = true;
    RADON_LOGINFO("PlayerCameraService: Initialized and listening for player camera events");
}

void PlayerCameraService::Shutdown()
{
    if (!initialized_)
        return;

    RADON_LOGDEBUG("PlayerCameraService: Shutdown called");
    
    UnsubscribeFromAllEvents();
    
    if (cameraManager_)
    {
        cameraManager_->Shutdown();
        cameraManager_.Reset();
    }
    
    playerCameraActive_ = false;
    initialized_ = false;
    
    RADON_LOGINFO("PlayerCameraService: Shutdown complete");
}

void PlayerCameraService::OnPlayerCameraReady(StringHash, VariantMap& eventData)
{
    RADON_LOGDEBUG("PlayerCameraService: Player camera ready event received");
    
    auto* cameraNode = static_cast<Node*>(eventData["CameraNode"].GetPtr());
    auto* playerNode = static_cast<Node*>(eventData["PlayerNode"].GetPtr());
    
    if (!cameraNode || !playerNode)
    {
        RADON_LOGERROR("PlayerCameraService: Invalid camera or player node in event");
        return;
    }
    
    playerCameraNode_ = cameraNode;
    playerNode_ = playerNode;
    
    SetupCameraManager(cameraNode, playerNode);
    
    playerCameraActive_ = true;
    RADON_LOGINFO("PlayerCameraService: Player camera activated");
}

// TODO Зачем этот метод, если это просто пересылка ивента?
void PlayerCameraService::OnCameraDirectionChanged(StringHash, VariantMap& eventData)
{
    // Forward camera direction changes to player movement system
    // This allows player movement to use camera direction without tight coupling
    
    VariantMap playerEventData;
    playerEventData["Forward"] = eventData["Forward"];
    playerEventData["Right"] = eventData["Right"];
    playerEventData["Pitch"] = eventData["Pitch"];
    playerEventData["Yaw"] = eventData["Yaw"];
    
    SendEvent(StringHash("PlayerCameraDirectionChanged"), playerEventData);
}

void PlayerCameraService::SetupCameraManager(Node* cameraNode, Node* playerNode)
{
    if (!cameraNode || !playerNode)
        return;

    // Create camera manager if it doesn't exist
    if (!cameraManager_)
    {
        cameraManager_ = MakeShared<CameraManager>(context_);
    }
    
    // Initialize camera manager with player's camera node
    cameraManager_->Initialize(*cameraNode, 0.1f);
    
    RADON_LOGINFO("PlayerCameraService: Camera manager setup complete");
}