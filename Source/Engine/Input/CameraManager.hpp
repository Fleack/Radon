#pragma once

#include "CameraController/ICameraController.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Engine::Input
{

enum class CameraMode : uint8_t
{
    FREE_CAMERA = 0,
    FPS_CAMERA = 1,

    // Always last
    COUNT
};

class CameraManager final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraManager, Object);

public:
    explicit CameraManager(Urho3D::Context* context);
    ~CameraManager() override;

    void Initialize(Urho3D::Node& cameraNode, float lookSensitivity = 0.1f);
    void Shutdown();

    void SetCameraMode(CameraMode mode);
    [[nodiscard]] CameraMode GetCurrentCameraMode() const { return currentMode_; }

    [[nodiscard]] Urho3D::WeakPtr<ICameraController> GetCurrentCamera() const { return currentCamera_; }
    [[nodiscard]] Urho3D::WeakPtr<ICameraController> GetFreeCamera() const { return GetCameraByMode(CameraMode::FREE_CAMERA); }
    [[nodiscard]] Urho3D::WeakPtr<ICameraController> GetFPSCamera() const { return GetCameraByMode(CameraMode::FPS_CAMERA); }

    void SetLookSensitivity(float sensitivity) const;
    [[nodiscard]] float GetLookSensitivity() const;

    /// Check if player camera is active
    [[nodiscard]] bool IsPlayerCameraActive() const { return playerCameraActive_; }

    void EnablePlayerCameraIntegration();
    void DisablePlayerCameraIntegration();

private:
    Urho3D::WeakPtr<ICameraController> GetCameraByMode(CameraMode mode) const { return controllers_[std::to_underlying(mode)]; }

    template <class Fn>
    void ApplyToAll(Fn fn) const
    {
        for (auto const& controller : controllers_)
            if (controller) fn(*controller);
    }

    void OnPlayerCameraReady(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    static constexpr std::size_t CameraModeCount = std::to_underlying(CameraMode::COUNT);

    CameraMode currentMode_{CameraMode::FREE_CAMERA};
    Urho3D::WeakPtr<ICameraController> currentCamera_{nullptr};
    std::array<Urho3D::SharedPtr<ICameraController>, CameraModeCount> controllers_{};

    Urho3D::WeakPtr<Urho3D::Node> cameraNode_;
    Urho3D::WeakPtr<Urho3D::Node> playerNode_;
    
    bool playerCameraActive_{false};
    bool listeningForPlayerCamera_{false};
};

} // namespace Radon::Engine::Input
