#pragma once

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Game::Player
{

/// Player camera component that manages camera setup at fixed height
/// Provides camera interface to other systems without complex positioning logic
class PlayerCamera : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerCamera, LogicComponent);

public:
    explicit PlayerCamera(Urho3D::Context* context);
    ~PlayerCamera() override = default;

    static void RegisterObject(Urho3D::Context* context);

    void DelayedStart() override;

    [[nodiscard]] Urho3D::Node* GetCameraNode() const { return cameraNode_; }
    [[nodiscard]] Urho3D::Camera* GetCamera() const { return camera_; }

private:
    void NotifyCameraReady();

private:
    static constexpr float CAMERA_HEIGHT = 1.8f;

    Urho3D::Node* cameraNode_{nullptr};
    Urho3D::Camera* camera_{nullptr};

    bool initialized_{false};
    bool cameraRegistered_{false};
};

} // namespace Radon::Game::Player 