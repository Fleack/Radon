#include "FPSCameraController.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/CameraEvents.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Math/MathDefs.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

FPSCameraController::FPSCameraController(Context* context)
    : BaseCameraController(context)
{
    RADON_LOGDEBUG("FPSCameraController: constructor called");
}

FPSCameraController::~FPSCameraController()
{
    RADON_LOGDEBUG("FPSCameraController: destructor called");
}

void FPSCameraController::Initialize(Node& cameraNode)
{
    BaseCameraController::Initialize(cameraNode);

    if (initialized_)
    {
        lastRotation_ = cameraNode_->GetRotation();
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(FPSCameraController, OnUpdate));
        RADON_LOGINFO("FPSCameraController: Initialized and subscribed to updates");
    }
}

void FPSCameraController::Shutdown()
{
    UnsubscribeFromEvent(E_UPDATE);
    BaseCameraController::Shutdown();
}

void FPSCameraController::OnUpdate(StringHash, VariantMap&)
{
    if (cameraNode_.Expired())
    {
        RADON_LOGWARN("FPSCameraController: camera node expired, skipping update");
        return;
    }

    // TODO: Use PlayerInputHandler
    auto* input = GetSubsystem<Urho3D::Input>();
    if (!input)
        return;

    UpdateOrientation(static_cast<float>(input->GetMouseMoveX()), static_cast<float>(input->GetMouseMoveY()));

    if (!cameraNode_->GetRotation().Equals(lastRotation_))
    {
        directionChanged_ = true;
        lastRotation_ = cameraNode_->GetRotation();
    }

    // Notify about direction changes
    if (directionChanged_)
    {
        NotifyDirectionChanged();
        directionChanged_ = false;
    }
}

void FPSCameraController::NotifyDirectionChanged()
{
    using namespace Radon::Engine::Graphics::Events;

    VariantMap eventData;
    eventData[P::FORWARD] = GetForwardDirection();
    eventData[P::RIGHT] = GetRightDirection();
    eventData[P::PITCH] = pitch_;
    eventData[P::YAW] = yaw_;

    SendEvent(E_CAMERA_BASE_TRANSFORM_CHANGED, eventData);
}

Vector3 FPSCameraController::GetForwardDirection() const
{
    if (cameraNode_.Expired())
        return Vector3::FORWARD;

    Vector3 forward = cameraNode_->GetDirection();
    forward.y_ = 0.0f;
    forward.Normalize();
    return forward;
}

Vector3 FPSCameraController::GetRightDirection() const
{
    if (cameraNode_.Expired())
        return Vector3::RIGHT;

    Vector3 right = cameraNode_->GetRight();
    right.y_ = 0.0f;
    right.Normalize();
    return right;
}
