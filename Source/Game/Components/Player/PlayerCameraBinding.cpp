#include "PlayerCameraBinding.hpp"

#include "Engine/Core/Logger.hpp"
#include "Game/Components/Player/PlayerInputHandler.hpp"
#include "Game/Components/Player/PlayerMovement.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Components
{

Urho3D::StringHash const PlayerCameraBinding::EVENT_CAMERA_MOVED("PlayerCameraMoved");
Urho3D::StringHash const PlayerCameraBinding::EVENT_HEADBOB("PlayerHeadbob");

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

    if (!inputHandler_)
        inputHandler_ = node_->GetComponent<PlayerInputHandler>();
    if (!movement_)
        movement_ = node_->GetComponent<PlayerMovement>();

    if (!inputHandler_ || !movement_)
    {
        RADON_LOGERROR("PlayerCameraBinding: Failed to get player components");
        return;
    }

    if (!cameraNode_)
        cameraNode_ = node_->GetChild("Camera");
    if (!cameraNode_)
        cameraNode_ = node_->CreateChild("Camera");

    if (!camera_)
        camera_ = cameraNode_->GetOrCreateComponent<Urho3D::Camera>();

    cameraNode_->SetPosition(Urho3D::Vector3(0.0f, cameraHeight_, 0.0f));
    originalCameraPosition_ = cameraNode_->GetPosition();

    initialized_ = true;
}

void PlayerCameraBinding::DelayedStart()
{
    Start();
}

void PlayerCameraBinding::Update(float timeStep)
{
    if (!initialized_ || !inputHandler_ || !movement_ || !cameraNode_ || !camera_)
        return;

    auto prevRot = cameraNode_->GetRotation();
    auto yawQuat = Urho3D::Quaternion(inputHandler_->GetMouseYaw(), Urho3D::Vector3::UP);
    auto pitchQuat = Urho3D::Quaternion(inputHandler_->GetMousePitch(), Urho3D::Vector3::RIGHT);
    cameraNode_->SetRotation(yawQuat * pitchQuat);
    if (cameraNode_->GetRotation() != prevRot)
        SendEvent(EVENT_CAMERA_MOVED);

    ApplyHeadBob(timeStep);
}

void PlayerCameraBinding::ApplyHeadBob(float timeStep)
{
    return;
    static float bobLerp = 0.0f;
    if (movement_->IsMoving())
    {
        float speed = movement_->IsRunning() ? headBobSpeed_ * 1.5f : headBobSpeed_;
        headBobTime_ += timeStep * speed;
        bobLerp = 1.0f;
    }
    else
    {
        bobLerp -= timeStep * 3.0f;
        if (bobLerp < 0.0f) bobLerp = 0.0f;
    }

    Urho3D::Vector3 bobOffset = Urho3D::Vector3::ZERO;

    if (bobLerp > 0.0f)
    {
        float intensity = movement_->IsRunning() ? headBobStrength_ * 1.5f : headBobStrength_;
        bobOffset.y_ = Urho3D::Sin(headBobTime_ * 360.0f) * intensity * bobLerp;
        bobOffset.x_ = Urho3D::Cos(headBobTime_ * 180.0f) * intensity * 0.5f * bobLerp;
    }

    auto prevPos = cameraNode_->GetPosition();
    cameraNode_->SetPosition(originalCameraPosition_ + bobOffset);
    if (cameraNode_->GetPosition() != prevPos)
        SendEvent(EVENT_HEADBOB);
}

void PlayerCameraBinding::SetInputHandler(PlayerInputHandler* handler)
{
    inputHandler_ = handler;
}

void PlayerCameraBinding::SetMovement(PlayerMovement* movement)
{
    movement_ = movement;
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
