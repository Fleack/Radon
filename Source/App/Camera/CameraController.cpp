#include "CameraController.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Math/MathDefs.h>

using namespace Urho3D;
using namespace Radon;

CameraController::CameraController(Context* context)
    : Object(context) {}

CameraController::~CameraController()
{
    Shutdown();
}

void CameraController::Initialize(Node& cameraNode, float lookSensitivity, float moveSpeed)
{
    if (initialized_)
    {
        URHO3D_LOGWARNING("CameraController: already initialized");
        return;
    }

    cameraNode_ = &cameraNode;
    lookSensitivity_ = lookSensitivity;
    moveSpeed_ = moveSpeed;

    Quaternion const initialRot = cameraNode_->GetRotation();
    yaw_ = initialRot.YawAngle();
    pitch_ = initialRot.PitchAngle();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CameraController, OnUpdate));

    auto* input = GetSubsystem<Input>();
    input->SetMouseMode(MM_RELATIVE);
    input->SetMouseVisible(false);

    initialized_ = true;
}

void CameraController::Shutdown()
{
    if (!initialized_)
        return;

    UnsubscribeFromAllEvents();
    initialized_ = false;
}

void CameraController::OnUpdate(StringHash, VariantMap& eventData)
{
    if (cameraNode_.Expired())
        return;

    float const dt = eventData[Update::P_TIMESTEP].GetFloat();

    auto* input = GetSubsystem<Input>();
    yaw_ += input->GetMouseMoveX() * lookSensitivity_;
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
