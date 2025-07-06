#include "GameplayState.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/ViewportManager.hpp"
#include "Engine/Input/CameraManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/AppStateManager.hpp"
#include "Engine/StateMachine/IGameState.hpp"
#include "Engine/UI/DebugHUD.hpp"
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

    scene_ = GetSubsystem<Scene::SceneManager>()->LoadScene(gameplaySceneName_);

    Urho3D::Node* playerNode = scene_->GetChild("Player", true);
    if (!playerNode)
    {
        RADON_LOGERROR("GameplayState: Player node not found in scene, creating a new one");
        return;
    }

    // Initialize CameraManager with player node - camera will be created automatically
    cameraManager_ = MakeShared<Input::CameraManager>(context_);
    cameraManager_->Initialize(*playerNode);

    // Setup viewport immediately after camera initialization
    GetSubsystem<Graphics::ViewportManager>()->SetupViewport(*scene_, *cameraManager_->GetCameraNode(), 0);

    GetSubsystem<UI::UIManager>()->ShowDocument("GameplayHUD");

    // TODO Add some flag for debugging
    auto* DebugHUD = scene_->CreateChild("DebugHUD");
    debugHUD_ = DebugHUD->CreateComponent<Engine::UI::DebugHUD>();

    auto* input = GetSubsystem<Urho3D::Input>();
    input->SetMouseMode(Urho3D::MM_RELATIVE);
    input->SetMouseVisible(false);

    SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER(GameplayState, HandleKeydown));

    RADON_LOGINFO("GameplayState: successfully entered");
}

void GameplayState::Exit()
{
    RADON_LOGINFO("GameplayState: exiting");
    UnsubscribeFromAllEvents();

    GetSubsystem<Graphics::ViewportManager>()->ClearViewport(0);

    // Shutdown CameraManager
    if (cameraManager_)
    {
        cameraManager_->Shutdown();
        cameraManager_ = nullptr;
    }

    GetSubsystem<UI::UIManager>()->UnloadDocument("GameplayHUD");
    GetSubsystem<Scene::SceneManager>()->UnloadScene(gameplaySceneName_);

    scene_ = nullptr;

    RADON_LOGINFO("GameplayState: exited and cleaned up");
}

void GameplayState::Update(float)
{
    // No special update logic needed - CameraManager handles everything
}

void GameplayState::HandleKeydown(Urho3D::StringHash, Urho3D::VariantMap& data)
{
    using namespace Urho3D;
    int key = data[KeyDown::P_KEY].GetInt();
    if (key == KEY_ESCAPE)
    {
        RADON_LOGINFO("GameplayState: ESC pressed, returning to menu");
        GetSubsystem<StateMachine::AppStateManager>()->ReplaceState(MakeShared<MenuState>(context_));
    }
    else if (key == KEY_F1) // Toggle debug camera
    {
        if (cameraManager_)
        {
            cameraManager_->ToggleMode();
        }
    }
}
