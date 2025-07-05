#include "PlayerCamera.hpp"

#include "../ComponentCategory.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

namespace Radon::Game::Player
{

PlayerCamera::PlayerCamera(Urho3D::Context* context)
    : LogicComponent(context)
{
    // Simple camera setup - no complex logic needed
}

void PlayerCamera::RegisterObject(Urho3D::Context* context)
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    context->AddFactoryReflection<PlayerCamera>(Category_Radon_Player);
}

void PlayerCamera::DelayedStart()
{
    if (initialized_)
        return;

    // Create or find camera node
    cameraNode_ = node_->GetChild("Camera");
    if (!cameraNode_)
        cameraNode_ = node_->CreateChild("Camera");

    // Create camera component
    camera_ = cameraNode_->GetOrCreateComponent<Urho3D::Camera>();
    cameraNode_->SetPosition(Urho3D::Vector3(0.0f, CAMERA_HEIGHT, 0.0f));

    initialized_ = true;
}

} // namespace Radon::Game::Player 