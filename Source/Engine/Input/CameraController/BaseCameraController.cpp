#include "BaseCameraController.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Math/MathDefs.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

BaseCameraController::BaseCameraController(Context* context)
    : ICameraController(context)
{
    RADON_LOGDEBUG("BaseCameraController: constructor called");
}

BaseCameraController::~BaseCameraController()
{
    RADON_LOGDEBUG("BaseCameraController: destructor called");
    BaseCameraController::Shutdown();
}

void BaseCameraController::Initialize(Node& cameraNode)
{
    RADON_LOGDEBUG("BaseCameraController: Initialize called");
    if (initialized_)
    {
        RADON_LOGWARN("BaseCameraController: already initialized");
        return;
    }

    cameraNode_ = &cameraNode;

    Quaternion const initialRot = cameraNode_->GetRotation();
    yaw_ = initialRot.YawAngle();
    pitch_ = initialRot.PitchAngle();

    RADON_LOGINFO(
        "BaseCameraController: Initialized with yaw={:.2f}, pitch={:.2f}, sensitivity={:.2f}, speed={:.2f}",
        yaw_,
        pitch_,
        lookSensitivity_,
        moveSpeed_);

    initialized_ = true;
}

void BaseCameraController::Shutdown()
{
    RADON_LOGDEBUG("BaseCameraController: Shutdown called");
    if (!initialized_)
        return;

    UnsubscribeFromAllEvents();
    initialized_ = false;
    RADON_LOGINFO("BaseCameraController: Shutdown complete");
}

void BaseCameraController::UpdateOrientation(float mouseXMove, float mouseYMove)
{
    if (cameraNode_.Expired())
    {
        RADON_LOGWARN("BaseCameraController: camera node expired, skipping orientation update");
        return;
    }

    yaw_ += mouseXMove * lookSensitivity_;
    pitch_ = Clamp(pitch_ + mouseYMove * lookSensitivity_, pitchMin_, pitchMax_);

    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
}
