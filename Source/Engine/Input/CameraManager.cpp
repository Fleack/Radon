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

void CameraManager::Initialize(Node& cameraNode, float lookSensitivity, float moveSpeed)
{
    RADON_LOGDEBUG("CameraManager: Initialize called");

    cameraNode_ = &cameraNode;

    controllers_[std::to_underlying(CameraMode::FREE_CAMERA)] =
        MakeShared<FreeCameraController>(context_);
    controllers_[std::to_underlying(CameraMode::FPS_CAMERA)] =
        MakeShared<FPSCameraController>(context_);

    ApplyToAll([lookSensitivity, moveSpeed](auto& camera) {
        camera.SetLookSensitivity(lookSensitivity);
        camera.SetMoveSpeed(moveSpeed);
    });

    SetCameraMode(CameraMode::FPS_CAMERA);

    RADON_LOGINFO("CameraManager: Initialized with sensitivity={:.2f}, speed={:.2f}", lookSensitivity, moveSpeed);
}

void CameraManager::Shutdown()
{
    RADON_LOGDEBUG("CameraManager: Shutdown called");
    if (!controllers_[0])
        return;

    ApplyToAll([](auto& camera) {
        camera.Shutdown();
    });

    currentCamera_ = nullptr;
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

void CameraManager::SetMoveSpeed(float speed) const
{
    ApplyToAll([&](auto& camera) { camera.SetMoveSpeed(speed); });
}

float CameraManager::GetMoveSpeed() const
{
    return currentCamera_ ? currentCamera_->GetMoveSpeed() : 5.0f;
}
