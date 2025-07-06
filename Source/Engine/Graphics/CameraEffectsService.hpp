#pragma once

#include "CameraEffectsManager.hpp"
#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Graphics
{

/// Camera Effects Service
/// Manages camera effects integration with the game
class CameraEffectsService final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraEffectsService, Object);

public:
    explicit CameraEffectsService(Urho3D::Context* context);
    ~CameraEffectsService() override;

    void Initialize();
    void Shutdown();

    void SetIntegrationEnabled(bool enabled);
    [[nodiscard]] bool IsIntegrationEnabled() const { return integrationEnabled_; }

    /// Manually set camera to observe (for use without PlayerCamera)
    void SetObservedCamera(Urho3D::Node* cameraNode, Urho3D::Camera* camera);

    /// Clear observed camera
    void ClearObservedCamera();

    /// Get effects manager for direct access
    [[nodiscard]] CameraEffectsManager* GetEffectsManager() const { return effectsManager_; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void ApplyEffectsToCamera(Urho3D::Node* cameraNode, Urho3D::Camera* camera, Urho3D::Vector3 const& basePosition, float timeStep);

private:
    Urho3D::SharedPtr<CameraEffectsManager> effectsManager_;

    // Camera observation
    Urho3D::WeakPtr<Urho3D::Node> observedCameraNode_;
    Urho3D::WeakPtr<Urho3D::Camera> observedCamera_;
    Urho3D::Vector3 cachedBasePosition_{Urho3D::Vector3::ZERO};

    bool initialized_{false};
    bool integrationEnabled_{true};
    bool observingCamera_{false};
};

} // namespace Radon::Engine::Graphics
