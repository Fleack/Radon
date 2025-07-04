#include "GameplayState.hpp"

#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/ViewportManager.hpp"
#include "Engine/Input/CameraManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/GameStateManager.hpp"
#include "Engine/StateMachine/IGameState.hpp"
#include "Engine/UI/UIManager.hpp"
#include "Engine/UI/DebugHUD.hpp"
#include "Game/States/MenuState.hpp"
#include "Player/Components/PlayerCamera.hpp"

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
        assert(0);
        return;
    }

    playerCamera_ = playerNode->GetComponent<Plugins::PlayerCamera>();
    if (!playerCamera_)
    {
        RADON_LOGERROR("GameplayState: PlayerCamera component not found on Player node");
        assert(0);
        return;
    }

    GetSubsystem<UI::UIManager>()->ShowDocument("GameplayHUD");

    // Создаем и добавляем DebugHUD
    auto* debugHudNode = scene_->CreateChild("DebugHUD");
    debugHUD_ = debugHudNode->CreateComponent<Engine::UI::DebugHUD>();

    auto* input = GetSubsystem<Urho3D::Input>();
    input->SetMouseMode(Urho3D::MM_RELATIVE);
    input->SetMouseVisible(false);

    SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER(GameplayState, HandleKeydown));

    isInitialized_ = false;
    RADON_LOGINFO("GameplayState: successfully entered");
}

void GameplayState::Exit()
{
    RADON_LOGINFO("GameplayState: exiting");
    UnsubscribeFromAllEvents();

    if (isInitialized_)
    {
        GetSubsystem<Graphics::ViewportManager>()->ClearViewport(0);
        GetSubsystem<Input::CameraManager>()->Shutdown();
    }

    GetSubsystem<UI::UIManager>()->UnloadDocument("GameplayHUD");
    GetSubsystem<Scene::SceneManager>()->UnloadScene(gameplaySceneName_);

    scene_ = nullptr;
    playerCamera_ = nullptr;
    isInitialized_ = false;

    RADON_LOGINFO("GameplayState: exited and cleaned up");
}

void GameplayState::Update(float)
{
    if (!isInitialized_ && playerCamera_ && playerCamera_->GetCameraNode()) [[unlikely]]
    {
        RADON_LOGINFO("GameplayState: Camera is ready, completing initialization");

        GetSubsystem<Graphics::ViewportManager>()->SetupViewport(*scene_, *playerCamera_->GetCameraNode(), 0);
        GetSubsystem<Input::CameraManager>()->Initialize(*playerCamera_->GetCameraNode(), 0.1f, 5.0f);
        isInitialized_ = true;

        RADON_LOGINFO("GameplayState: Initialization completed successfully");
    }
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
