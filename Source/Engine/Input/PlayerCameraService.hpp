#pragma once

#include "Engine/Input/CameraController/FPSCameraController.hpp"
#include "Engine/Input/CameraManager.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

/// Service that integrates Player plugin camera with engine camera system
/// Handles events from PlayerCamera component and configures engine controllers
class PlayerCameraService : public Urho3D::Object
{
    URHO3D_OBJECT(PlayerCameraService, Object);

public:
    explicit PlayerCameraService(Urho3D::Context* context);
    ~PlayerCameraService() override;

    /// Initialize the service
    void Initialize();
    
    /// Shutdown the service
    void Shutdown();

    /// Get the current camera manager
    [[nodiscard]] CameraManager* GetCameraManager() const { return cameraManager_.Get(); }

    /// Check if player camera is active
    [[nodiscard]] bool IsPlayerCameraActive() const { return playerCameraActive_; }

private:
    void OnPlayerCameraReady(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void OnCameraDirectionChanged(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    
    void SetupCameraManager(Urho3D::Node* cameraNode, Urho3D::Node* playerNode);

private:
    Urho3D::SharedPtr<CameraManager> cameraManager_;
    Urho3D::WeakPtr<Urho3D::Node> playerCameraNode_;
    Urho3D::WeakPtr<Urho3D::Node> playerNode_;
    
    bool initialized_{false};
    bool playerCameraActive_{false};
};

} // namespace Radon::Engine::Input
