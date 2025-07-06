#pragma once

#include "Engine/StateMachine/IGameState.hpp"

#include <Urho3D/Scene/Scene.h>

namespace Radon::Engine::UI
{
class DebugHUD;
}

namespace Radon::Engine::Input
{
class CameraManager;
}

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

    Urho3D::WeakPtr<Urho3D::Scene> scene_;
    Urho3D::WeakPtr<Engine::UI::DebugHUD> debugHUD_;
    Urho3D::SharedPtr<Engine::Input::CameraManager> cameraManager_;
};

} // namespace Radon::Game::States
