#pragma once

#include "Engine/StateMachine/IGameState.hpp"
#include "Engine/UI/EventListenerGuard.hpp"

#include <RmlUi/Core/ElementDocument.h>

namespace Radon::Game::States
{

class MenuState : public Engine::StateMachine::IGameState
{
    URHO3D_OBJECT(MenuState, IGameState)
public:
    explicit MenuState(Urho3D::Context* context);
    ~MenuState() override = default;
    void Enter() override;
    void Exit() override;
    void Update(float timeStep) override;

private:
    void HandlePlay();
    void HandleExit();

    template <typename RML_LISTENER, void (MenuState::*Fn)()>
    void RegisterButton(ea::string const& id, Rml::EventId eventId);

private:
    ea::string const mainMenuSceneName_{"MainMenu"};

    Rml::ElementDocument* currentDocument_;
    ea::vector<Engine::UI::ListenerGuard> subscriptions_;
};

} // namespace Radon::Game::States
