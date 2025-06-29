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

    void Initialize(Urho3D::Node& cameraNode, float lookSensitivity = 0.1f, float moveSpeed = 5.0f);
    void Shutdown();

    void SetCameraMode(CameraMode mode);
    [[nodiscard]] CameraMode GetCurrentCameraMode() const { return currentMode_; }

    [[nodiscard]] Urho3D::WeakPtr<ICameraController> GetCurrentCamera() const { return currentCamera_; }
    [[nodiscard]] Urho3D::WeakPtr<ICameraController> GetFreeCamera() const { return GetCameraByMode(CameraMode::FREE_CAMERA); }
    [[nodiscard]] Urho3D::WeakPtr<ICameraController> GetFPSCamera() const { return GetCameraByMode(CameraMode::FPS_CAMERA); }

    void SetLookSensitivity(float sensitivity) const;
    [[nodiscard]] float GetLookSensitivity() const;

    void SetMoveSpeed(float speed) const;
    [[nodiscard]] float GetMoveSpeed() const;

private:
    Urho3D::SharedPtr<ICameraController> GetCameraByMode(CameraMode mode) const { return controllers_[std::to_underlying(mode)]; }

    template <class Fn>
    void ApplyToAll(Fn fn) const
    {
        for (auto const& controller : controllers_)
            if (controller) fn(*controller);
    }

private:
    static constexpr std::size_t CameraModeCount = std::to_underlying(CameraMode::COUNT);

    std::array<Urho3D::SharedPtr<ICameraController>, CameraModeCount> controllers_{};
    CameraMode currentMode_{CameraMode::FREE_CAMERA};
    Urho3D::WeakPtr<ICameraController> currentCamera_{nullptr};

    Urho3D::WeakPtr<Urho3D::Node> cameraNode_;
};

} // namespace Radon::Engine::Input
