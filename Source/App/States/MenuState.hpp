#pragma once

#include "App/States/IGameState.hpp"
#include "App/UI/EventListenerGuard.hpp"

#include <RmlUi/Core/ElementDocument.h>

namespace Radon::States
{

class MenuState : public IGameState
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
    ea::string const mainMenuName_{"MainMenu"};

    Rml::ElementDocument* currentDocument_;
    ea::vector<UI::ListenerGuard> subscriptions_;
};

} // namespace Radon::States
