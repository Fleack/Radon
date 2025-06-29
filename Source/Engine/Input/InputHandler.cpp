#include "InputHandler.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>

using namespace Urho3D;
using namespace Radon::Engine::Input;

InputHandler::InputHandler(Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("InputHandler: constructor called");
}

InputHandler::~InputHandler()
{
    RADON_LOGDEBUG("InputHandler: destructor called");
    Shutdown();
}

void InputHandler::Initialize()
{
    RADON_LOGDEBUG("InputHandler: Initialize called");
    if (initialized_)
    {
        RADON_LOGWARN("InputHandler: already initialized");
        return;
    }

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(InputHandler, OnUpdate));
    initialized_ = true;
    RADON_LOGINFO("InputHandler: Initialized");
}

void InputHandler::Shutdown()
{
    RADON_LOGDEBUG("InputHandler: Shutdown called");
    if (!initialized_)
        return;

    UnsubscribeFromAllEvents();
    initialized_ = false;
    RADON_LOGINFO("InputHandler: Shutdown complete");
}

void InputHandler::OnUpdate(StringHash, VariantMap&)
{
    auto* input = GetSubsystem<Urho3D::Input>();

    forwardMove_ = input->GetKeyDown(forwardKey_);
    backMove_ = input->GetKeyDown(backKey_);
    leftMove_ = input->GetKeyDown(leftKey_);
    rightMove_ = input->GetKeyDown(rightKey_);
    upMove_ = input->GetKeyDown(upKey_);
    downMove_ = input->GetKeyDown(downKey_);

    mouseDeltaX_ = static_cast<float>(input->GetMouseMoveX());
    mouseDeltaY_ = static_cast<float>(input->GetMouseMoveY());
}
