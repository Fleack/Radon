#include "GameplayState.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/ViewportManager.hpp"
#include "Engine/Input/CameraManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/GameStateManager.hpp"
#include "Engine/StateMachine/IGameState.hpp"
#include "Engine/UI/UIManager.hpp"
#include "Game/States/MenuState.hpp"

#include <Urho3D/Input/Input.h>
#include <Urho3D/RmlUI/RmlUI.h>
#include <Urho3D/UI/UI.h>

using namespace Radon::Game::States;
using namespace Radon::Engine;

GameplayState::GameplayState(Urho3D::Context* context)
    : StateMachine::IGameState(context)
{
}

void GameplayState::Enter()
{
    RADON_LOGINFO("GameplayState: entering");

    Urho3D::WeakPtr<Urho3D::Scene> scene = GetSubsystem<Scene::SceneManager>()->LoadScene(gameplaySceneName_);
    Urho3D::Node* cameraNode = scene->GetChild("Camera", true);
    GetSubsystem<Graphics::ViewportManager>()->SetupViewport(*scene, *cameraNode, 0);
    GetSubsystem<UI::UIManager>()->ShowDocument("GameplayHUD");

    auto* input = GetSubsystem<Urho3D::Input>();
    input->SetMouseMode(Urho3D::MM_RELATIVE);
    input->SetMouseVisible(false);

    GetSubsystem<Input::CameraManager>()->Initialize(*cameraNode, 0.1f, 5.0f);

    SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER(GameplayState, HandleKeydown));

    RADON_LOGINFO("GameplayState: successfully entered");
}

void GameplayState::Exit()
{
    RADON_LOGINFO("GameplayState: exiting");
    UnsubscribeFromAllEvents();
    GetSubsystem<Graphics::ViewportManager>()->ClearViewport(0);
    GetSubsystem<UI::UIManager>()->UnloadDocument("GameplayHUD");
    GetSubsystem<Input::CameraManager>()->Shutdown();
    GetSubsystem<Scene::SceneManager>()->UnloadScene(gameplaySceneName_);
    RADON_LOGINFO("GameplayState: exited and cleaned up");
}

void GameplayState::Update(float)
{
}

void GameplayState::HandleKeydown(Urho3D::StringHash, Urho3D::VariantMap& data)
{
    using namespace Urho3D;
    int key = data[KeyDown::P_KEY].GetInt();
    if (key == KEY_ESCAPE)
    {
        RADON_LOGINFO("GameplayState: ESC pressed, returning to menu");
        GetSubsystem<StateMachine::GameStateManager>()->ReplaceState(MakeShared<MenuState>(context_));
    }
}
