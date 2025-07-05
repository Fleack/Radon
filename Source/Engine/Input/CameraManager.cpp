#include "CameraManager.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Input/CameraController/FPSCameraController.hpp"
#include "Engine/Input/CameraController/FreeCameraController.hpp"

using namespace Urho3D;
using namespace Radon::Engine::Input;

CameraManager::CameraManager(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("CameraManager: constructor called");
}

CameraManager::~CameraManager()
{
    RADON_LOGDEBUG("CameraManager: destructor called");
    Shutdown();
}

void CameraManager::Initialize(Node& cameraNode, float lookSensitivity)
{
    RADON_LOGDEBUG("CameraManager: Initialize called");

    cameraNode_ = &cameraNode;

    controllers_[std::to_underlying(CameraMode::FREE_CAMERA)] =
        MakeShared<FreeCameraController>(context_);
    controllers_[std::to_underlying(CameraMode::FPS_CAMERA)] =
        MakeShared<FPSCameraController>(context_);

    ApplyToAll([lookSensitivity](auto& camera) {
        camera.SetLookSensitivity(lookSensitivity);
    });

    SetCameraMode(CameraMode::FPS_CAMERA); // TODO Free camera broken

    RADON_LOGINFO("CameraManager: Initialized with sensitivity={:.2f}", lookSensitivity);
}

void CameraManager::EnablePlayerCameraIntegration()
{
    if (listeningForPlayerCamera_)
        return;

    RADON_LOGDEBUG("CameraManager: EnablePlayerCameraIntegration called");

    SubscribeToEvent(StringHash("PlayerCameraReady"), URHO3D_HANDLER(CameraManager, OnPlayerCameraReady));

    listeningForPlayerCamera_ = true;
    RADON_LOGINFO("CameraManager: Now listening for PlayerCameraReady events");
}

void CameraManager::DisablePlayerCameraIntegration()
{
    if (!listeningForPlayerCamera_)
        return;

    RADON_LOGDEBUG("CameraManager: DisablePlayerCameraIntegration called");

    UnsubscribeFromEvent(StringHash("PlayerCameraReady"));
    listeningForPlayerCamera_ = false;

    RADON_LOGINFO("CameraManager: Stopped listening for PlayerCameraReady events");
}

void CameraManager::Shutdown()
{
    RADON_LOGDEBUG("CameraManager: Shutdown called");

    DisablePlayerCameraIntegration();

    if (!controllers_[0])
        return;

    ApplyToAll([](auto& camera) {
        camera.Shutdown();
    });

    currentCamera_ = nullptr;
    playerCameraActive_ = false;
    RADON_LOGINFO("CameraManager: Shutdown complete");
}

void CameraManager::SetCameraMode(CameraMode mode)
{
    if (mode == currentMode_ && currentCamera_)
        return;

    if (currentCamera_)
        currentCamera_->Shutdown();

    currentMode_ = mode;
    currentCamera_ = GetCameraByMode(mode);

    if (!cameraNode_.Expired() && currentCamera_)
    {
        currentCamera_->Initialize(*cameraNode_);
    }
    RADON_LOGINFO("CameraManager: Switched to {} mode", mode == CameraMode::FREE_CAMERA ? "FREE_CAMERA" : "FPS_CAMERA");
}

void CameraManager::SetLookSensitivity(float sensitivity) const
{
    ApplyToAll([&](auto& camera) { camera.SetLookSensitivity(sensitivity); });
}

float CameraManager::GetLookSensitivity() const
{
    return currentCamera_ ? currentCamera_->GetLookSensitivity() : 0.1f;
}

void CameraManager::OnPlayerCameraReady(StringHash, VariantMap& eventData)
{
    RADON_LOGDEBUG("CameraManager: Player camera ready event received");

    auto* cameraNode = dynamic_cast<Node*>(eventData["CameraNode"].GetPtr());
    auto* playerNode = dynamic_cast<Node*>(eventData["PlayerNode"].GetPtr());

    if (!cameraNode || !playerNode)
    {
        RADON_LOGERROR("CameraManager: Invalid camera or player node in event");
        return;
    }

    playerNode_ = playerNode;

    Initialize(*cameraNode);

    playerCameraActive_ = true;
    RADON_LOGINFO("CameraManager: Player camera activated");
}
