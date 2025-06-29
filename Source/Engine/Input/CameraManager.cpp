#include "CameraManager.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Input/CameraMode/FPSCameraController.hpp"

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
    if (initialized_)
    {
        RADON_LOGWARN("CameraManager: already initialized");
        return;
    }

    cameraNode_ = &cameraNode;

    freeCamera_ = MakeShared<FreeCameraController>(context_);
    freeCamera_->SetLookSensitivity(lookSensitivity);
    freeCamera_->SetMoveSpeed(moveSpeed);

    fpsCamera_ = MakeShared<FPSCameraController>(context_);
    fpsCamera_->SetLookSensitivity(lookSensitivity);
    fpsCamera_->SetMoveSpeed(moveSpeed);

    SetCameraMode(CameraMode::FREE_CAMERA);

    initialized_ = true;
    RADON_LOGINFO("CameraManager: Initialized with sensitivity={:.2f}, speed={:.2f}", lookSensitivity, moveSpeed);
}

void CameraManager::Shutdown()
{
    RADON_LOGDEBUG("CameraManager: Shutdown called");
    if (!initialized_)
        return;

    if (freeCamera_)
    {
        freeCamera_->Shutdown();
        freeCamera_ = nullptr;
    }

    if (fpsCamera_)
    {
        fpsCamera_->Shutdown();
        fpsCamera_ = nullptr;
    }

    currentCamera_ = nullptr;
    initialized_ = false;
    RADON_LOGINFO("CameraManager: Shutdown complete");
}

void CameraManager::SetCameraMode(CameraMode mode)
{
    if (mode == currentMode_ && currentCamera_)
        return;

    if (currentCamera_)
    {
        currentCamera_->Shutdown();
    }

    currentMode_ = mode;

    switch (mode)
    {
    case CameraMode::FREE_CAMERA:
        currentCamera_ = freeCamera_;
        RADON_LOGINFO("CameraManager: Switched to FREE_CAMERA mode");
        break;

    case CameraMode::FPS_CAMERA:
        currentCamera_ = fpsCamera_;
        RADON_LOGINFO("CameraManager: Switched to FPS_CAMERA mode");
        break;
    }

    if (!cameraNode_.Expired() && currentCamera_)
    {
        currentCamera_->Initialize(*cameraNode_);
    }
}

void CameraManager::SetLookSensitivity(float sensitivity) const
{
    if (currentCamera_)
    {
        currentCamera_->SetLookSensitivity(sensitivity);
    }

    if (freeCamera_) freeCamera_->SetLookSensitivity(sensitivity);
    if (fpsCamera_) fpsCamera_->SetLookSensitivity(sensitivity);
}

float CameraManager::GetLookSensitivity() const
{
    if (currentCamera_)
        return currentCamera_->GetLookSensitivity();

    return 0.1f;
}

void CameraManager::SetMoveSpeed(float speed) const
{
    if (currentCamera_)
    {
        currentCamera_->SetMoveSpeed(speed);
    }

    if (freeCamera_) freeCamera_->SetMoveSpeed(speed);
    if (fpsCamera_) fpsCamera_->SetMoveSpeed(speed);
}

float CameraManager::GetMoveSpeed() const
{
    if (currentCamera_)
        return currentCamera_->GetMoveSpeed();

    return 5.0f;
}
