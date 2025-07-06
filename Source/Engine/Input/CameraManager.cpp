#include "CameraManager.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Graphics/Camera.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

CameraManager::CameraManager(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("CameraManager: constructor called");
}

// TODO Просто передает вызовы в CameraController, избавиться
CameraManager::~CameraManager()
{
    RADON_LOGDEBUG("CameraManager: destructor called");
    Shutdown();
}

void CameraManager::Initialize(Node& playerNode)
{
    RADON_LOGDEBUG("CameraManager: Initialize called");
    
    if (initialized_)
    {
        RADON_LOGWARN("CameraManager: already initialized");
        return;
    }

    cameraController_ = MakeShared<CameraController>(context_);
    cameraController_->Initialize(playerNode);
    
    initialized_ = true;
    RADON_LOGINFO("CameraManager: Initialized with CameraController");
}

void CameraManager::Shutdown()
{
    RADON_LOGDEBUG("CameraManager: Shutdown called");
    
    if (!initialized_)
        return;

    if (cameraController_)
    {
        cameraController_->Shutdown();
        cameraController_.Reset();
    }
    
    initialized_ = false;
    RADON_LOGINFO("CameraManager: Shutdown complete");
}

void CameraManager::SetMode(CameraMode mode)
{
    if (cameraController_)
        cameraController_->SetMode(mode);
}

CameraMode CameraManager::GetMode() const
{
    return cameraController_ ? cameraController_->GetMode() : CameraMode::FPS;
}

void CameraManager::SetLookSensitivity(float sensitivity)
{
    if (cameraController_)
        cameraController_->SetLookSensitivity(sensitivity);
}

float CameraManager::GetLookSensitivity() const
{
    return cameraController_ ? cameraController_->GetLookSensitivity() : 0.1f;
}

void CameraManager::SetMoveSpeed(float speed)
{
    if (cameraController_)
        cameraController_->SetMoveSpeed(speed);
}

float CameraManager::GetMoveSpeed() const
{
    return cameraController_ ? cameraController_->GetMoveSpeed() : 10.0f;
}

void CameraManager::SetPlayerNode(Node* playerNode)
{
    if (cameraController_)
        cameraController_->SetPlayerNode(playerNode);
}

void CameraManager::ToggleMode()
{
    if (!cameraController_)
        return;
        
    CameraMode currentMode = cameraController_->GetMode();
    CameraMode newMode = (currentMode == CameraMode::FPS) ? CameraMode::DEBUG : CameraMode::FPS;
    
    cameraController_->SetMode(newMode);
    RADON_LOGINFO("CameraManager: Toggled to {} mode", newMode == CameraMode::FPS ? "FPS" : "DEBUG");
}

Node* CameraManager::GetCameraNode() const
{
    return cameraController_ ? cameraController_->GetCameraNode() : nullptr;
}

Camera* CameraManager::GetCamera() const
{
    return cameraController_ ? cameraController_->GetCamera() : nullptr;
}
