#include "AppStateManager.hpp"
#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>

using namespace Radon::Engine::StateMachine;

AppStateManager::AppStateManager(Urho3D::Context* context)
    : Object(context)
{
    Initialize();
    context->RegisterSubsystem(this);
    RADON_LOGDEBUG("AppStateManager: registered as subsystem");
}

AppStateManager::~AppStateManager()
{
    RADON_LOGDEBUG("AppStateManager: destructor called");
    Shutdown();
}

void AppStateManager::Initialize()
{
    RADON_LOGDEBUG("AppStateManager: Initialize called");

    SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(AppStateManager, OnUpdate));

    RADON_LOGINFO("AppStateManager: Initialized and subscribed to updates");
}

void AppStateManager::Shutdown()
{
    RADON_LOGDEBUG("AppStateManager: Shutdown called");

    UnsubscribeFromAllEvents();

    RADON_LOGINFO("AppStateManager: Shutdown complete");
}

void AppStateManager::OnUpdate(Urho3D::StringHash, Urho3D::VariantMap& eventData)
{
    for (auto& state : states_.get_container())
    {
        if (state)
        {
            state->Update(eventData[Urho3D::E_UPDATE].GetFloat());
        }
    }
}

void AppStateManager::PushState(Urho3D::SharedPtr<IGameState> state)
{
    RADON_LOGDEBUG("AppStateManager: PushState called");
    if (!states_.empty())
    {
        RADON_LOGDEBUG("AppStateManager: Exiting current state before push");
        states_.top()->Exit();
    }
    states_.push(state);
    RADON_LOGINFO("AppStateManager: New state pushed and entered");
    state->Enter();
}

void AppStateManager::ReplaceState(Urho3D::SharedPtr<IGameState> state)
{
    RADON_LOGDEBUG("AppStateManager: ReplaceState called");
    while (!states_.empty())
    {
        RADON_LOGDEBUG("AppStateManager: Exiting and popping state");
        states_.top()->Exit();
        states_.pop();
    }
    states_.push(state);
    RADON_LOGINFO("AppStateManager: State replaced and entered");
    state->Enter();
}

void AppStateManager::PopState()
{
    RADON_LOGDEBUG("AppStateManager: PopState called");
    if (states_.empty())
        return;
    states_.top()->Exit();
    states_.pop();
    if (!states_.empty())
    {
        RADON_LOGINFO("AppStateManager: Entering previous state after pop");
        states_.top()->Enter();
    }
    else
    {
        RADON_LOGINFO("AppStateManager: No more states after pop");
    }
}
