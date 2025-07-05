#include "PlayerCamera.hpp"

#include "ComponentCategory.hpp"
#include "Events/GlobalVars.hpp"
#include "Events/PlayerEvents.hpp"
#include "Urho3D/IO/Log.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Plugins
{

PlayerCamera::PlayerCamera(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_UPDATE);
}

void PlayerCamera::RegisterObject(Urho3D::Context* context)
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    context->AddFactoryReflection<PlayerCamera>(Category_Radon_Player);

    URHO3D_ATTRIBUTE("Camera Height", float, cameraHeight_, 1.8f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("HeadBob Strength", float, headBobStrength_, 0.04f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("HeadBob Speed", float, headBobSpeed_, 1.5f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("HeadBob Horizontal Factor", float, headBobHorizontalFactor_, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("HeadBob Vertical Factor", float, headBobVerticalFactor_, 1.0f, Urho3D::AM_DEFAULT);
}

void PlayerCamera::DelayedStart()
{
    if (initialized_)
        return;

    cameraNode_ = node_->GetChild("Camera");
    if (!cameraNode_)
        cameraNode_ = node_->CreateChild("Camera");

    camera_ = cameraNode_->GetOrCreateComponent<Urho3D::Camera>();

    cameraNode_->SetPosition(Urho3D::Vector3(0.0f, cameraHeight_, 0.0f));
    basePosition_ = cameraNode_->GetPosition();

    SubscribeToEvents();

    initialized_ = true;
}

void PlayerCamera::SubscribeToEvents()
{
    // TODO: Jump with ceiling above player?
    SubscribeToEvent(Events::E_PLAYER_JUMPED, [this](Urho3D::StringHash, Urho3D::VariantMap&) {
        playerJumped_ = true;
        playerGrounded_ = false;
    });

    SubscribeToEvent(Events::E_PLAYER_GROUNDED, [this](Urho3D::StringHash, Urho3D::VariantMap&) {
        playerJumped_ = false;
        playerGrounded_ = true;
    });

    SubscribeToEvent(Events::E_PLAYER_STARTED_MOVING, [this](Urho3D::StringHash, Urho3D::VariantMap&) {
        playerMoving_ = true;
    });

    SubscribeToEvent(Events::E_PLAYER_STOPPED_MOVING, [this](Urho3D::StringHash, Urho3D::VariantMap&) {
        playerMoving_ = false;
    });

    SubscribeToEvent(Events::E_PLAYER_RUN_STATE_CHANGED, [this](Urho3D::StringHash, Urho3D::VariantMap& eventData) {
        playerRunning_ = eventData[Events::P::IS_RUNNING].GetBool();
    });
}

void PlayerCamera::Update(float timeStep)
{
    if (!initialized_ || !cameraNode_ || !camera_)
        return;

    Urho3D::Variant const& mouseYaw = GetGlobalVar(GlobalVars::PLAYER_MOUSE_YAW);
    Urho3D::Variant const& mousePitch = GetGlobalVar(GlobalVars::PLAYER_MOUSE_PITCH);

    auto prevRot = cameraNode_->GetRotation();
    auto yawQuat = Urho3D::Quaternion(mouseYaw.GetFloat(), Urho3D::Vector3::UP);
    auto pitchQuat = Urho3D::Quaternion(mousePitch.GetFloat(), Urho3D::Vector3::RIGHT);
    Urho3D::Quaternion targetRotation = yawQuat * pitchQuat;
    Urho3D::Quaternion smoothedRotation = cameraNode_->GetRotation().Slerp(targetRotation, Urho3D::Min(1.0f, timeStep * 20.0f));
    //cameraNode_->SetRotation(smoothedRotation);

    if (cameraNode_->GetRotation() != prevRot)
    {
        // TODO Replace with global vars
        Urho3D::VariantMap dirEventData;
        dirEventData[Events::P::FORWARD] = GetCamForward();
        dirEventData[Events::P::RIGHT] = GetCamRight();
        dirEventData[Events::P::PITCH] = mousePitch.GetFloat();
        dirEventData[Events::P::YAW] = mouseYaw.GetFloat();
        SendEvent(Events::E_PLAYER_CAMERA_DIRECTION_CHANGED, dirEventData);
    }

    ApplyHeadBob(timeStep);
}

void PlayerCamera::ApplyHeadBob(float timeStep)
{
    if (!playerGrounded_)
        return;

    if (playerMoving_)
    {
        float speed = playerRunning_ ? headBobSpeed_ * 1.5f : headBobSpeed_;
        headBobTime_ += timeStep * speed;
        bobLerp_ = Urho3D::Min(bobLerp_ + timeStep * 2.0f, 1.0f);
    }
    else
    {
        bobLerp_ -= timeStep * 3.0f;
        if (bobLerp_ < 0.0f) bobLerp_ = 0.0f;
    }

    Urho3D::Vector3 bobOffset = Urho3D::Vector3::ZERO;
    if (bobLerp_ > 0.0f)
    {
        float intensity = playerRunning_ ? headBobStrength_ : headBobStrength_ * 0.7f;
        bobOffset.y_ = Urho3D::Sin(headBobTime_ * 360.0f) * intensity * bobLerp_ * headBobVerticalFactor_;
        bobOffset.x_ = Urho3D::Sin(headBobTime_ * 180.0f) * intensity * bobLerp_ * headBobHorizontalFactor_;
    }

    Urho3D::Vector3 newPosition = basePosition_ + bobOffset;
    if (!cameraNode_->GetPosition().Equals(newPosition))
    {
        cameraNode_->SetPosition(newPosition);
        SendEvent(Events::E_PLAYER_HEADBOB);
    }
}

void PlayerCamera::UpdateCameraPosition()
{
    if (!initialized_ || !cameraNode_)
        return;

    basePosition_ = Urho3D::Vector3(0.0f, cameraHeight_, 0.0f);
    cameraNode_->SetPosition(basePosition_);
}

Urho3D::Vector3 PlayerCamera::GetCamForward() const
{
    if (!cameraNode_)
    {
        return Urho3D::Vector3::FORWARD;
    }

    Urho3D::Vector3 camForward = cameraNode_->GetDirection();
    camForward.y_ = 0.0f;
    camForward.Normalize();

    return camForward;
}

Urho3D::Vector3 PlayerCamera::GetCamRight() const
{
    if (!cameraNode_)
    {
        return Urho3D::Vector3::RIGHT;
    }

    Urho3D::Vector3 camRight = cameraNode_->GetRight();
    camRight.y_ = 0.0f;
    camRight.Normalize();

    return camRight;
}

} // namespace Radon::Game::Plugins
