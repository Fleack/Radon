#include "GameStateManager.hpp"

#include "App/States/GameplayState.hpp"
#include "App/States/MenuState.hpp"

#include <Urho3D/IO/Log.h>

namespace Radon
{

GameStateManager::GameStateManager(Context* context)
    : Object(context)
{
    URHO3D_LOGINFO("Game State Manager initialized");
    context->RegisterSubsystem(this);
}

void GameStateManager::Update(float timeStep)
{
    if (!states_.empty())
        states_.top()->Update(timeStep);
}

void GameStateManager::ClearStates()
{
    while (!states_.empty())
    {
        states_.top()->Exit();
        states_.pop();
    }
}

void GameStateManager::PopState()
{
    if (states_.empty()) return;

    states_.top()->Exit();
    states_.pop();

    if (!states_.empty())
        states_.top()->Enter();
}

template <typename T, typename... Args>
void GameStateManager::PushState(Args&&... args)
{
    if (!states_.empty())
        states_.top()->Exit();
    auto state = MakeShared<T>(context_, std::forward<Args>(args)...);
    states_.push(state);
    state->Enter();
}

template <typename T, typename... Args>
void GameStateManager::ReplaceState(Args&&... args)
{
    ClearStates();
    auto state = MakeShared<T>(context_, std::forward<Args>(args)...);
    states_.push(state);
    state->Enter();
}

template void GameStateManager::PushState<MenuState>();
template void GameStateManager::PushState<GameplayState>();
template void GameStateManager::ReplaceState<MenuState>();
template void GameStateManager::ReplaceState<GameplayState>();

} // namespace Radon
