#pragma once

#include "App/States/IGameState.hpp"

namespace Radon::States
{

class GameplayState : public IGameState
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

} // namespace Radon::States
