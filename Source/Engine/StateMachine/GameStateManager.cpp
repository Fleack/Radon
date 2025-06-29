#include "GameStateManager.hpp"

#include "Engine/Core/Logger.hpp"

#include <Urho3D/Core/Context.h>

using namespace Radon::Engine::StateMachine;

GameStateManager::GameStateManager(Urho3D::Context* context)
    : Object(context)
{
    context->RegisterSubsystem(this);
    RADON_LOGDEBUG("GameStateManager: registered as subsystem");
}

void GameStateManager::Update(float timeStep)
{
    if (!states_.empty())
    {
        states_.top()->Update(timeStep);
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
