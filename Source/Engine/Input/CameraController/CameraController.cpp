#include "CameraController.hpp"
#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/CoreEvents.h>
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

void CameraController::Initialize(Node& cameraNode, Node* playerNode)
{
    RADON_LOGDEBUG("CameraController: Initialize called");

    if (initialized_)
    {
        RADON_LOGWARN("CameraController: already initialized");
        return;
    }

    cameraNode_ = &cameraNode;
    playerNode_ = playerNode;

    // Initialize camera rotation from current node rotation
    Quaternion const initialRot = cameraNode_->GetRotation();
    yaw_ = initialRot.YawAngle();
    pitch_ = initialRot.PitchAngle();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CameraController, OnUpdate));

    initialized_ = true;
    RADON_LOGINFO("CameraController: Initialized with mode={}, yaw={:.2f}, pitch={:.2f}", currentMode_ == CameraMode::FPS ? "FPS" : "DEBUG", yaw_, pitch_);
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

void CameraController::SetMode(CameraMode mode)
{
    if (mode == currentMode_)
        return;

    currentMode_ = mode;
    RADON_LOGINFO("CameraController: Switched to {} mode", mode == CameraMode::FPS ? "FPS" : "DEBUG");
}

void CameraController::OnUpdate(StringHash, VariantMap& eventData)
{
    if (cameraNode_.Expired())
    {
        RADON_LOGWARN("CameraController: camera node expired, skipping update");
        return;
    }

    float const deltaTime = eventData[Update::P_TIMESTEP].GetFloat();
    auto* input = GetSubsystem<Urho3D::Input>();

    if (!input)
        return;

    // Handle mouse look
    auto mouseX = static_cast<float>(input->GetMouseMoveX());
    auto mouseY = static_cast<float>(input->GetMouseMoveY());

    if (mouseX != 0.0f || mouseY != 0.0f)
    {
        UpdateOrientation(mouseX, mouseY);
    }

    // Handle mode-specific updates
    switch (currentMode_)
    {
    case CameraMode::FPS:
        UpdateFPSCamera(deltaTime);
        break;
    case CameraMode::DEBUG:
        UpdateDebugCamera(deltaTime);
        break;
    }
}

void CameraController::UpdateFPSCamera(float)
{
    // In FPS mode, camera follows player if player node exists
    if (!playerNode_.Expired())
    {
        Vector3 playerPos = playerNode_->GetPosition();
        Vector3 cameraPos = playerPos + debugCameraOffset_;
        cameraNode_->SetPosition(cameraPos);
    }

    // Apply rotation
    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
}

void CameraController::UpdateDebugCamera(float deltaTime)
{
    auto* input = GetSubsystem<Urho3D::Input>();

    // Free camera movement
    Vector3 moveDir = Vector3::ZERO;

    if (input->GetKeyDown(KEY_W)) moveDir += Vector3::FORWARD;
    if (input->GetKeyDown(KEY_S)) moveDir += Vector3::BACK;
    if (input->GetKeyDown(KEY_A)) moveDir += Vector3::LEFT;
    if (input->GetKeyDown(KEY_D)) moveDir += Vector3::RIGHT;
    if (input->GetKeyDown(KEY_SPACE)) moveDir += Vector3::UP;
    if (input->GetKeyDown(KEY_CTRL)) moveDir += Vector3::DOWN;

    if (!moveDir.Equals(Vector3::ZERO))
    {
        moveDir.Normalize();
        cameraNode_->Translate(moveDir * moveSpeed_ * deltaTime, TS_LOCAL);
    }

    // Apply rotation
    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));
}

void CameraController::UpdateOrientation(float mouseX, float mouseY)
{
    yaw_ += mouseX * lookSensitivity_;

    // Normalize yaw
    if (yaw_ > 360.0f || yaw_ < -360.0f)
        yaw_ = std::fmod(yaw_, 360.0f);

    // Clamp pitch
    pitch_ = Clamp(pitch_ + mouseY * lookSensitivity_, -89.0f, 89.0f);
}
