#include "GameStateManager.hpp"

#include <Urho3D/IO/Log.h>

namespace Radon
{

GameStateManager::GameStateManager(Context* context)
    : Object(context)
{
    URHO3D_LOGINFO("Game State Manager initialized");
}

void GameStateManager::Update(float timeStep)
{
    if (!states_.empty())
        states_.top()->Update(timeStep);
}

void GameStateManager::PopState()
{
    if (!states_.empty())
    {
        states_.top()->Exit();
        states_.pop();

        if (!states_.empty())
            states_.top()->Enter();
    }
}

void GameStateManager::ClearStates()
{
    while (!states_.empty())
    {
        states_.top()->Exit();
        states_.pop();
    }
}

} // namespace Radon
