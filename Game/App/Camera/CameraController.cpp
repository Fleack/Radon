#include "CameraController.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>

namespace Radon
{

CameraController::CameraController(Context* context)
    : Object(context)
{
}

void CameraController::Setup(Node* cameraNode)
{
    cameraNode_ = cameraNode;

    Quaternion initialRot = cameraNode_->GetRotation();
    yaw_ = initialRot.YawAngle();
    pitch_ = initialRot.PitchAngle();

    // Subscribe to update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CameraController, HandleUpdate));

    // Configure input
    auto* input = GetSubsystem<Input>();
    input->SetMouseMode(MM_RELATIVE);
    input->SetMouseVisible(false);
}

void CameraController::HandleUpdate(StringHash, VariantMap& eventData)
{
    float dt = eventData[Update::P_TIMESTEP].GetFloat();
    auto* input = GetSubsystem<Input>();

    yaw_ += static_cast<float>(input->GetMouseMoveX()) * lookSensitivity_;
    pitch_ += static_cast<float>(input->GetMouseMoveY()) * lookSensitivity_;
    pitch_ = Clamp(pitch_, -89.0f, 89.0f);

    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    Vector3 dir;
    if (input->GetKeyDown(KEY_W)) dir += Vector3::FORWARD;
    if (input->GetKeyDown(KEY_S)) dir += Vector3::BACK;
    if (input->GetKeyDown(KEY_A)) dir += Vector3::LEFT;
    if (input->GetKeyDown(KEY_D)) dir += Vector3::RIGHT;
    if (!dir.Equals(Vector3::ZERO))
        cameraNode_->Translate(dir.Normalized() * moveSpeed_ * dt, TS_LOCAL);
}

} // namespace Radon
