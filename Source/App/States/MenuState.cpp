#include "MenuState.hpp"

#include "App/Graphics/ViewportManager.hpp"
#include "App/Logger/Logger.hpp"
#include "App/Scene/SceneManager.hpp"
#include "App/States/GameStateManager.hpp"
#include "App/States/GameplayState.hpp"
#include "App/UI/UIManager.hpp"

#include <functional>

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/RmlUI/RmlUI.h>
#include <Urho3D/UI/UI.h>

using namespace Radon::States;
using namespace Urho3D;

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

MenuState::MenuState(Context* context)
    : IGameState(context)
{
}

void MenuState::Enter()
{
    RADON_LOGINFO("MenuState: entering");
    subscriptions_.clear();
    subscriptions_.reserve(2);

    WeakPtr<Urho3D::Scene> scene = GetSubsystem<Scene::SceneManager>()->LoadScene(mainMenuName_);
    Node* cameraNode = scene->GetChild("MenuCamera", true);
    GetSubsystem<Graphics::ViewportManager>()->SetupViewport(*scene, *cameraNode, 0);

    auto* input = GetSubsystem<Input>();
    input->SetMouseMode(MM_FREE);
    input->SetMouseVisible(true);

    auto* uiManager = GetSubsystem<UI::UIManager>();
    currentDocument_ = uiManager->ShowDocument(mainMenuName_);

    RegisterButton<RmlClickListener, &MenuState::HandlePlay>("play-button", Rml::EventId::Click);
    RegisterButton<RmlClickListener, &MenuState::HandleExit>("exit-button", Rml::EventId::Click);
}

void MenuState::Exit()
{
    RADON_LOGINFO("MenuState: exiting");

    subscriptions_.clear();

    GetSubsystem<Graphics::ViewportManager>()->ClearViewport(0);
    GetSubsystem<Scene::SceneManager>()->UnloadScene(mainMenuName_);

    auto* ui = GetSubsystem<UI::UIManager>();
    ui->UnloadDocument(mainMenuName_);
    currentDocument_ = nullptr;

    RADON_LOGDEBUG("MenuState: MainMenu document and listeners removed");
}

void MenuState::Update(float)
{
}

void MenuState::HandlePlay()
{
    RADON_LOGINFO("MenuState: Play button pressed");
    GetSubsystem<GameStateManager>()->ReplaceState(MakeShared<GameplayState>(context_));
}

void MenuState::HandleExit()
{
    RADON_LOGINFO("MenuState: Exit button pressed");
    Exit();
    GetSubsystem<Engine>()->Exit();
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
