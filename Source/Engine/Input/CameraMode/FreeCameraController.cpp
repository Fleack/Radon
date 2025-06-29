#include "FreeCameraController.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

FreeCameraController::FreeCameraController(Context* context)
    : BaseCameraController(context)
{
    RADON_LOGDEBUG("FreeCameraController: constructor called");
}

FreeCameraController::~FreeCameraController()
{
    RADON_LOGDEBUG("FreeCameraController: destructor called");
}

void FreeCameraController::Initialize(Node& cameraNode)
{
    BaseCameraController::Initialize(cameraNode);
    if (initialized_)
    {
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(FreeCameraController, OnUpdate));
        RADON_LOGINFO("FreeCameraController: Initialized and subscribed to updates");
    }
}

void FreeCameraController::OnUpdate(StringHash, VariantMap& eventData)
{
    if (cameraNode_.Expired())
    {
        RADON_LOGWARN("FreeCameraController: camera node expired, skipping update");
        return;
    }

    float const dt = eventData[Update::P_TIMESTEP].GetFloat();

    auto* input = GetSubsystem<Urho3D::Input>();

    UpdateOrientation(static_cast<float>(input->GetMouseMoveX()), static_cast<float>(input->GetMouseMoveY()));

    Vector3 dir;
    if (input->GetKeyDown(KEY_W)) dir += Vector3::FORWARD;
    if (input->GetKeyDown(KEY_S)) dir += Vector3::BACK;
    if (input->GetKeyDown(KEY_A)) dir += Vector3::LEFT;
    if (input->GetKeyDown(KEY_D)) dir += Vector3::RIGHT;
    if (input->GetKeyDown(KEY_SPACE)) dir += Vector3::UP;
    if (input->GetKeyDown(KEY_CTRL)) dir += Vector3::DOWN;

    if (!dir.Equals(Vector3::ZERO))
    {
        cameraNode_->Translate(dir.Normalized() * moveSpeed_ * dt, TS_LOCAL);
    }
}
