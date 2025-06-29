#include "PlayerCameraBinding.hpp"

#include "Engine/Core/Logger.hpp"
#include "PlayerInputHandler.hpp"
#include "PlayerMovement.hpp"

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
}

void PlayerCameraBinding::Start()
{
    if (initialized_)
        return;

    inputHandler_ = node_->GetComponent<PlayerInputHandler>();
    movement_ = node_->GetComponent<PlayerMovement>();

    if (!inputHandler_ || !movement_)
    {
        RADON_LOGERROR("PlayerCameraBinding: Failed to get player components");
        return;
    }

    cameraNode_ = node_->GetChild("Camera");
    if (!cameraNode_)
    {
        RADON_LOGERROR("PlayerCameraBinding: Failed to find 'Camera' child node");
        return;
    }

    camera_ = cameraNode_->GetComponent<Urho3D::Camera>();
    if (!camera_)
    {
        RADON_LOGERROR("PlayerCameraBinding: Failed to get Camera component");
        return;
    }

    originalCameraPosition_ = cameraNode_->GetPosition();

    initialized_ = true;
}

void PlayerCameraBinding::Update(float timeStep)
{
    if (!initialized_ || !inputHandler_ || !movement_ || !cameraNode_ || !camera_)
        return;

    cameraNode_->SetRotation(Urho3D::Quaternion(inputHandler_->GetMousePitch(), Urho3D::Vector3::RIGHT));

    ApplyHeadBob(timeStep);
}

void PlayerCameraBinding::ApplyHeadBob(float timeStep)
{
    if (movement_->IsMoving())
    {
        float speed = movement_->IsRunning() ? headBobSpeed_ * 1.5f : headBobSpeed_;
        headBobTime_ += timeStep * speed;
    }

    Urho3D::Vector3 bobOffset = Urho3D::Vector3::ZERO;

    if (movement_->IsMoving())
    {
        float intensity = movement_->IsRunning() ? headBobStrength_ * 1.5f : headBobStrength_;
        bobOffset.y_ = Urho3D::Sin(headBobTime_ * 360.0f) * intensity;
        bobOffset.x_ = Urho3D::Cos(headBobTime_ * 180.0f) * intensity * 0.5f;
    }

    cameraNode_->SetPosition(originalCameraPosition_ + bobOffset);
}

} // namespace Radon::Game::Components
