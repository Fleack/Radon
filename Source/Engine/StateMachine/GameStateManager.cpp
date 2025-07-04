#include "GameStateManager.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>

using namespace Radon::Engine::StateMachine;

GameStateManager::GameStateManager(Urho3D::Context* context)
    : Object(context)
{
    Initialize();
    context->RegisterSubsystem(this);
    RADON_LOGDEBUG("GameStateManager: registered as subsystem");
}

GameStateManager::~GameStateManager()
{
    RADON_LOGDEBUG("GameStateManager: destructor called");
    Shutdown();
}

void GameStateManager::Initialize()
{
    RADON_LOGDEBUG("GameStateManager: Initialize called");

    SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(GameStateManager, OnUpdate));

    RADON_LOGINFO("GameStateManager: Initialized and subscribed to updates");
}

void GameStateManager::Shutdown()
{
    RADON_LOGDEBUG("GameStateManager: Shutdown called");

    UnsubscribeFromAllEvents();

    RADON_LOGINFO("GameStateManager: Shutdown complete");
}

void GameStateManager::OnUpdate(Urho3D::StringHash, Urho3D::VariantMap& eventData)
{
    for (auto& state : states_.get_container())
    {
        if (state)
        {
            state->Update(eventData[Urho3D::E_UPDATE].GetFloat());
        }
    }
}

void GameStateManager::PushState(Urho3D::SharedPtr<IGameState> state)
{
    RADON_LOGDEBUG("GameStateManager: PushState called");
    if (!states_.empty())
    {
        RADON_LOGDEBUG("GameStateManager: Exiting current state before push");
        states_.top()->Exit();
    }
    states_.push(state);
    RADON_LOGINFO("GameStateManager: New state pushed and entered");
    state->Enter();
}

void GameStateManager::ReplaceState(Urho3D::SharedPtr<IGameState> state)
{
    RADON_LOGDEBUG("GameStateManager: ReplaceState called");
    while (!states_.empty())
    {
        RADON_LOGDEBUG("GameStateManager: Exiting and popping state");
        states_.top()->Exit();
        states_.pop();
    }
    states_.push(state);
    RADON_LOGINFO("GameStateManager: State replaced and entered");
    state->Enter();
}

void GameStateManager::PopState()
{
    RADON_LOGDEBUG("GameStateManager: PopState called");
    if (states_.empty())
        return;
    states_.top()->Exit();
    states_.pop();
    if (!states_.empty())
    {
        RADON_LOGINFO("GameStateManager: Entering previous state after pop");
        states_.top()->Enter();
    }
    else
    {
        RADON_LOGINFO("GameStateManager: No more states after pop");
    }
}
