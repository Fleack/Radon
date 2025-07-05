#include "PlayerInputHandler.hpp"

#include "ComponentCategory.hpp"
#include "Events/GlobalVars.hpp"
#include "Events/PlayerEvents.hpp"

#include <cmath>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputConstants.h>
#include <Urho3D/Math/MathDefs.h>

namespace Urho3D
{
template <>
VariantType GetVariantType<Key>()
{
    return VAR_INT;
}
} // namespace Urho3D

namespace Radon::Game::Plugins
{

PlayerInputHandler::PlayerInputHandler(Urho3D::Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(Urho3D::USE_UPDATE);
}

PlayerInputHandler::~PlayerInputHandler() = default;

void PlayerInputHandler::RegisterObject(Urho3D::Context* context)
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    // TODO Change to config read
    context->AddFactoryReflection<PlayerInputHandler>(Category_Radon_Player);
    URHO3D_ATTRIBUTE("Mouse Sensitivity X", float, mouseSensitivityX_, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Mouse Sensitivity Y", float, mouseSensitivityY_, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Mouse Smoothing", float, mouseSmoothing_, 0.5f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Invert Mouse", bool, invertMouseY_, false, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Mouse Deadzone", float, mouseDeadzone_, 0.01f, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Forward Key", Urho3D::Key, forwardKey_, Urho3D::KEY_W, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Back Key", Urho3D::Key, backKey_, Urho3D::KEY_S, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Left Key", Urho3D::Key, leftKey_, Urho3D::KEY_A, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Right Key", Urho3D::Key, rightKey_, Urho3D::KEY_D, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Jump Key", Urho3D::Key, jumpKey_, Urho3D::KEY_SPACE, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Run Key", Urho3D::Key, runKey_, Urho3D::KEY_SHIFT, Urho3D::AM_DEFAULT);
    URHO3D_ATTRIBUTE("Interact Key", Urho3D::Key, interactKey_, Urho3D::KEY_E, Urho3D::AM_DEFAULT);
}

void PlayerInputHandler::Update(float timeStep)
{
    auto* input = GetSubsystem<Urho3D::Input>();
    if (!input)
        return;

    // Keyboard
    bool moveForward = input->GetKeyDown(forwardKey_);
    bool moveBack = input->GetKeyDown(backKey_);
    bool moveLeft = input->GetKeyDown(leftKey_);
    bool moveRight = input->GetKeyDown(rightKey_);
    bool jump = input->GetKeyDown(jumpKey_);
    bool run = input->GetKeyDown(runKey_);
    bool interact = input->GetKeyPress(interactKey_);

    // Mouse - block camera rotation when cursor is enabled for debug
    bool cursorEnabled = input->GetMouseMode() == Urho3D::MM_FREE; // TODO Camera completely broken
    if (!cursorEnabled)
    {
        float rawDeltaX = static_cast<float>(input->GetMouseMoveX()) * mouseSensitivityX_;
        float rawDeltaY = static_cast<float>(input->GetMouseMoveY()) * mouseSensitivityY_ * (invertMouseY_ ? -1.0f : 1.0f);
        
        static float smoothedDeltaX = 0.0f;
        static float smoothedDeltaY = 0.0f;
        smoothedDeltaX = smoothedDeltaX * mouseSmoothing_ + rawDeltaX * (1.0f - mouseSmoothing_);
        smoothedDeltaY = smoothedDeltaY * mouseSmoothing_ + rawDeltaY * (1.0f - mouseSmoothing_);
        if (std::abs(smoothedDeltaX) < mouseDeadzone_) smoothedDeltaX = 0.0f;
        if (std::abs(smoothedDeltaY) < mouseDeadzone_) smoothedDeltaY = 0.0f;
        mouseYaw_ += smoothedDeltaX;
        mousePitch_ += smoothedDeltaY;
        mousePitch_ = Urho3D::Clamp(mousePitch_, -80.0f, 80.0f);
        if (mouseYaw_ > 360.0f || mouseYaw_ < -360.0f)
            mouseYaw_ = std::fmod(mouseYaw_, 360.0f);
    }

    // Events and global vars
    SetGlobalVar(GlobalVars::PLAYER_MOVE_FORWARD, moveForward);
    SetGlobalVar(GlobalVars::PLAYER_MOVE_BACK, moveBack);
    SetGlobalVar(GlobalVars::PLAYER_MOVE_LEFT, moveLeft);
    SetGlobalVar(GlobalVars::PLAYER_MOVE_RIGHT, moveRight);
    SetGlobalVar(GlobalVars::PLAYER_JUMP, jump);
    SetGlobalVar(GlobalVars::PLAYER_RUN, run);
    SetGlobalVar(GlobalVars::PLAYER_MOUSE_YAW, mouseYaw_);
    SetGlobalVar(GlobalVars::PLAYER_MOUSE_PITCH, mousePitch_);
    if (interact)
        SendEvent(Events::E_PLAYER_INTERACTED);
}

} // namespace Radon::Game::Plugins
