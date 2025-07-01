#include "PlayerCameraBinding.hpp"

#include "Engine/Core/Logger.hpp"
#include "Game/Components/Events/PlayerEvents.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

PlayerCameraBinding::PlayerCameraBinding(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_UPDATE);
}

PlayerCameraBinding::~PlayerCameraBinding() = default;

void PlayerCameraBinding::RegisterObject(Urho3D::Context* context)
{
    context->AddFactoryReflection<PlayerCameraBinding>();

    URHO3D_ATTRIBUTE("CameraHeight", float, cameraHeight_, 1.8f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Head Bob Strength", float, headBobStrength_, 0.05f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Head Bob Speed", float, headBobSpeed_, 10.0f, Urho3D::AM_DEFAULT);
}

void PlayerCameraBinding::Start()
{
    if (initialized_)
        return;

    if (!cameraNode_)
        cameraNode_ = node_->GetChild("Camera");
    if (!cameraNode_)
        cameraNode_ = node_->CreateChild("Camera");

    if (!camera_)
        camera_ = cameraNode_->GetOrCreateComponent<Urho3D::Camera>();

    cameraNode_->SetPosition(Urho3D::Vector3(0.0f, cameraHeight_, 0.0f));
    originalCameraPosition_ = cameraNode_->GetPosition();

    SubscribeToEvent(Events::E_PLAYER_STARTED_MOVING, [this](Urho3D::StringHash, Urho3D::VariantMap&) {
        bool isRunning = GetGlobalVar("PlayerRun").GetBool();
        HandlePlayerMovementState(true, isRunning);
    });

    SubscribeToEvent(Events::E_PLAYER_STOPPED_MOVING, [this](Urho3D::StringHash, Urho3D::VariantMap&) {
        HandlePlayerMovementState(false, false);
    });

    SubscribeToEvent(Events::E_PLAYER_RUN_STATE_CHANGED, [this](Urho3D::StringHash, Urho3D::VariantMap& eventData) {
        bool isRunning = eventData[Events::P::IS_RUNNING].GetBool();
        bool isMoving = GetGlobalVar("PlayerMoving").GetBool();
        HandlePlayerMovementState(isMoving, isRunning);
    });

    initialized_ = true;
}

void PlayerCameraBinding::DelayedStart()
{
    Start();
}

void PlayerCameraBinding::Update(float timeStep)
{
    if (!initialized_ || !cameraNode_ || !camera_)
        return;

    Urho3D::Variant const& mouseYaw = GetGlobalVar("PlayerMouseYaw");
    Urho3D::Variant const& mousePitch = GetGlobalVar("PlayerMousePitch");

    if (!mouseYaw.IsEmpty() && !mousePitch.IsEmpty())
    {
        auto prevRot = cameraNode_->GetRotation();
        auto yawQuat = Urho3D::Quaternion(mouseYaw.GetFloat(), Urho3D::Vector3::UP);
        auto pitchQuat = Urho3D::Quaternion(mousePitch.GetFloat(), Urho3D::Vector3::RIGHT);
        Urho3D::Quaternion targetRotation = yawQuat * pitchQuat;
        Urho3D::Quaternion smoothedRotation = cameraNode_->GetRotation().Slerp(targetRotation, Urho3D::Min(1.0f, timeStep * 20.0f));
        cameraNode_->SetRotation(smoothedRotation);

        if (cameraNode_->GetRotation() != prevRot)
        {
            Urho3D::VariantMap dirEventData;
            dirEventData[Events::P::FORWARD] = GetCamForward();
            dirEventData[Events::P::RIGHT] = GetCamRight();
            dirEventData[Events::P::PITCH] = mousePitch.GetFloat();
            dirEventData[Events::P::YAW] = mouseYaw.GetFloat();
            SendEvent(Events::E_PLAYER_CAMERA_DIRECTION_CHANGED, dirEventData);
        }
    }

    ApplyHeadBob(timeStep);
}

void PlayerCameraBinding::ApplyHeadBob(float timeStep)
{
    static float bobLerp = 0.0f;
    bool isMoving = GetGlobalVar("PlayerMoving").GetBool();
    bool isRunning = GetGlobalVar("PlayerRun").GetBool();

    if (isMoving)
    {
        float speed = isRunning ? headBobSpeed_ * 1.5f : headBobSpeed_;
        headBobTime_ += timeStep * speed;
        bobLerp = Urho3D::Min(bobLerp + timeStep * 2.0f, 1.0f);
    }
    else
    {
        bobLerp -= timeStep * 3.0f;
        if (bobLerp < 0.0f) bobLerp = 0.0f;
    }

    Urho3D::Vector3 bobOffset = Urho3D::Vector3::ZERO;

    if (bobLerp > 0.0f)
    {
        float intensity = isRunning ? headBobStrength_ : headBobStrength_ * 0.7f;
        bobOffset.y_ = Urho3D::Sin(headBobTime_ * 360.0f) * intensity * bobLerp;
        bobOffset.x_ = Urho3D::Cos(headBobTime_ * 180.0f) * intensity * 0.3f * bobLerp; // Уменьшаем боковой покачивания
    }

    auto prevPos = cameraNode_->GetPosition();
    cameraNode_->SetPosition(originalCameraPosition_ + bobOffset);
    if (cameraNode_->GetPosition() != prevPos)
    {
        using namespace Radon::Game::Events;
        SendEvent(E_PLAYER_HEADBOB);
    }
}

void PlayerCameraBinding::HandlePlayerMovementState(bool isMoving, bool isRunning)
{
    SetGlobalVar("PlayerMoving", isMoving);
}

void PlayerCameraBinding::SetCameraNode(Urho3D::Node* node)
{
    cameraNode_ = node;
}

void PlayerCameraBinding::SetCamera(Urho3D::Camera* camera)
{
    camera_ = camera;
}

Urho3D::Vector3 PlayerCameraBinding::GetCamForward() const
{
    if (!cameraNode_)
    {
        RADON_LOGERROR("PlayerCameraBinding: Camera node is null, returning default forward vector");
        return Urho3D::Vector3::FORWARD;
    }

    Urho3D::Vector3 camForward = cameraNode_->GetDirection();
    camForward.y_ = 0.0f;
    camForward.Normalize();

    return camForward;
}

Urho3D::Vector3 PlayerCameraBinding::GetCamRight() const
{
    if (!cameraNode_)
    {
        RADON_LOGERROR("PlayerCameraBinding: Camera node is null, returning default right vector");
        return Urho3D::Vector3::RIGHT;
    }

    Urho3D::Vector3 camRight = cameraNode_->GetRight();
    camRight.y_ = 0.0f;
    camRight.Normalize();

    return camRight;
}

} // namespace Radon::Game::Components
