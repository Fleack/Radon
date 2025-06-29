#include "FPSCameraController.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>

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
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(FPSCameraController, OnUpdate));
        RADON_LOGINFO("FPSCameraController: Initialized and subscribed to updates");
    }
}

void FPSCameraController::SetPlayerNode(Node* playerNode)
{
    playerNode_ = playerNode;
    RADON_LOGINFO("FPSCameraController: Player node set");
}

void FPSCameraController::OnUpdate(StringHash, VariantMap& eventData)
{
    if (cameraNode_.Expired())
    {
        RADON_LOGWARN("FPSCameraController: camera node expired, skipping update");
        return;
    }

    auto* input = GetSubsystem<Urho3D::Input>();

    UpdateOrientation(static_cast<float>(input->GetMouseMoveX()), static_cast<float>(input->GetMouseMoveY()));

    if (!playerNode_.Expired())
    {
        Vector3 playerPos = playerNode_->GetPosition();
        Vector3 cameraPos = playerPos + cameraOffset_;
        cameraNode_->SetPosition(cameraPos);

        Quaternion playerRot = playerNode_->GetRotation();
        playerRot.FromEulerAngles(0.0f, yaw_, 0.0f);
        playerNode_->SetRotation(playerRot);
    }
}
