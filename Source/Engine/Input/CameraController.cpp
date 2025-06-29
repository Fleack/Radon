#include "CameraController.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Math/MathDefs.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

CameraController::CameraController(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("CameraController: constructor called");
}

CameraController::~CameraController()
{
    RADON_LOGDEBUG("CameraController: destructor called");
    Shutdown();
}

void CameraController::Initialize(Node& cameraNode, float lookSensitivity, float moveSpeed)
{
    RADON_LOGDEBUG("CameraController: Initialize called");
    if (initialized_)
    {
        RADON_LOGWARN("CameraController: already initialized");
        return;
    }

    cameraNode_ = &cameraNode;
    lookSensitivity_ = lookSensitivity;
    moveSpeed_ = moveSpeed;

    Quaternion const initialRot = cameraNode_->GetRotation();
    yaw_ = initialRot.YawAngle();
    pitch_ = initialRot.PitchAngle();

    RADON_LOGINFO("CameraController: Initialized with yaw={:.2f}, pitch={:.2f}, sensitivity={:.2f}, speed={:.2f}", yaw_, pitch_, lookSensitivity_, moveSpeed_);

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CameraController, OnUpdate));
    initialized_ = true;
}

void CameraController::Shutdown()
{
    RADON_LOGDEBUG("CameraController: Shutdown called");
    if (!initialized_)
        return;

    UnsubscribeFromAllEvents();
    initialized_ = false;
    RADON_LOGINFO("CameraController: Shutdown complete");
}

void CameraController::OnUpdate(StringHash, VariantMap& eventData)
{
    if (cameraNode_.Expired())
    {
        RADON_LOGWARN("CameraController: camera node expired, skipping update");
        return;
    }

    float const dt = eventData[Update::P_TIMESTEP].GetFloat();

    auto* input = GetSubsystem<Urho3D::Input>();
    yaw_ += static_cast<float>(input->GetMouseMoveX()) * lookSensitivity_;
    pitch_ = Clamp(pitch_ + static_cast<float>(input->GetMouseMoveY()) * lookSensitivity_, pitchMin_, pitchMax_);

    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    Vector3 dir;
    if (input->GetKeyDown(KEY_W)) dir += Vector3::FORWARD;
    if (input->GetKeyDown(KEY_S)) dir += Vector3::BACK;
    if (input->GetKeyDown(KEY_A)) dir += Vector3::LEFT;
    if (input->GetKeyDown(KEY_D)) dir += Vector3::RIGHT;

    if (!dir.Equals(Vector3::ZERO))
    {
        cameraNode_->Translate(dir.Normalized() * moveSpeed_ * dt, TS_LOCAL);
    }
}
