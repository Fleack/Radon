#pragma once

#include "Engine/StateMachine/IGameState.hpp"

namespace Radon::Game::States
{

class GameplayState : public Engine::StateMachine::IGameState
{
    URHO3D_OBJECT(GameplayState, IGameState)
public:
    explicit GameplayState(Urho3D::Context* context);
    void Enter() override;
    void Exit() override;
    void Update(float timeStep) override;

private:
    void HandleKeydown(Urho3D::StringHash, Urho3D::VariantMap&);

private:
    ea::string const gameplaySceneName_{"Gameplay"};
};

} // namespace Radon::Game::States
