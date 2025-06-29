#include "MenuState.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/ViewportManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/GameStateManager.hpp"
#include "Engine/UI/UIManager.hpp"
#include "Game/States/GameplayState.hpp"

#include <functional>

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/RmlUI/RmlUI.h>
#include <Urho3D/UI/UI.h>

using namespace Radon::Game::States;
using namespace Radon::Engine;

struct RmlClickListener : public Rml::EventListener
{
    using Callback = std::function<void()>;

    explicit RmlClickListener(Callback cb)
        : cb_(std::move(cb)) {}
    void ProcessEvent(Rml::Event&) override
    {
        cb_();
    }

    Callback cb_;
};

MenuState::MenuState(Urho3D::Context* context)
    : IGameState(context)
{
}

void MenuState::Enter()
{
    RADON_LOGINFO("MenuState: entering");
    subscriptions_.clear();
    subscriptions_.reserve(2);

    Urho3D::WeakPtr<Urho3D::Scene> scene = GetSubsystem<Scene::SceneManager>()->LoadScene(mainMenuSceneName_);
    Urho3D::Node* cameraNode = scene->GetChild("Camera", true);
    GetSubsystem<Graphics::ViewportManager>()->SetupViewport(*scene, *cameraNode, 0);
    currentDocument_ = GetSubsystem<UI::UIManager>()->ShowDocument(mainMenuSceneName_);

    auto* input = GetSubsystem<Urho3D::Input>();
    input->SetMouseMode(Urho3D::MM_FREE);
    input->SetMouseVisible(true);

    RegisterButton<RmlClickListener, &MenuState::HandlePlay>("play-button", Rml::EventId::Click);
    RegisterButton<RmlClickListener, &MenuState::HandleExit>("exit-button", Rml::EventId::Click);
    RADON_LOGINFO("MenuState: successfully entered");
}

void MenuState::Exit()
{
    RADON_LOGINFO("MenuState: exiting");

    subscriptions_.clear();

    GetSubsystem<Graphics::ViewportManager>()->ClearViewport(0);
    GetSubsystem<Scene::SceneManager>()->UnloadScene(mainMenuSceneName_);

    auto* ui = GetSubsystem<UI::UIManager>();
    ui->UnloadDocument(mainMenuSceneName_);
    currentDocument_ = nullptr;

    RADON_LOGDEBUG("MenuState: MainMenu document and listeners removed");
}

void MenuState::Update(float)
{
}

void MenuState::HandlePlay()
{
    RADON_LOGINFO("MenuState: Play button pressed");
    GetSubsystem<StateMachine::GameStateManager>()->ReplaceState(MakeShared<GameplayState>(context_));
}

void MenuState::HandleExit()
{
    RADON_LOGINFO("MenuState: Exit button pressed");
    Exit();
    GetSubsystem<Urho3D::Engine>()->Exit();
}

template <typename RML_LISTENER, void (MenuState::*Fn)()>
void MenuState::RegisterButton(ea::string const& id, Rml::EventId eventId)
{
    auto* button = currentDocument_->GetElementById(id);
    if (!button)
    {
        RADON_LOGWARN("MenuState: '{}' not found", id);
        return;
    }
    auto listener = Rml::MakeShared<RML_LISTENER>([this] { (this->*Fn)(); });
    subscriptions_.emplace_back(button, eventId, listener);
    RADON_LOGDEBUG("MenuState: '{}' listener added", id);
}
