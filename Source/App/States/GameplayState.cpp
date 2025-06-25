#include "GameplayState.hpp"

#include "App/Graphics/ViewportManager.hpp"
#include "App/Input/CameraController.hpp"
#include "App/Logger/Logger.hpp"
#include "App/Scene/SceneManager.hpp"
#include "App/States/GameStateManager.hpp"
#include "App/States/MenuState.hpp"
#include "App/UI/UIManager.hpp"

#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/RmlUI/RmlUI.h>
#include <Urho3D/UI/UI.h>

using namespace Radon::States;
using namespace Urho3D;

GameplayState::GameplayState(Context* context)
    : IGameState(context)
{
}

void GameplayState::Enter()
{
    RADON_LOGINFO("GameplayState: entering");

    WeakPtr<Urho3D::Scene> scene = GetSubsystem<Scene::SceneManager>()->LoadScene(gameplaySceneName_);
    Node* cameraNode = scene->GetChild("Camera", true);
    GetSubsystem<Graphics::ViewportManager>()->SetupViewport(*scene, *cameraNode, 0);
    GetSubsystem<UI::UIManager>()->ShowDocument("GameplayHUD");

    auto* input = GetSubsystem<Urho3D::Input>();
    input->SetMouseMode(MM_RELATIVE);
    input->SetMouseVisible(false);

    GetSubsystem<Input::CameraController>()->Initialize(*cameraNode, 0.1f, 5.0f);

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(GameplayState, HandleKeydown));

    RADON_LOGINFO("GameplayState: successfully entered");
}

void GameplayState::Exit()
{
    RADON_LOGINFO("GameplayState: exiting");
    UnsubscribeFromAllEvents();
    GetSubsystem<Graphics::ViewportManager>()->ClearViewport(0);
    GetSubsystem<UI::UIManager>()->UnloadDocument("GameplayHUD");
    GetSubsystem<Input::CameraController>()->Shutdown();
    GetSubsystem<Scene::SceneManager>()->UnloadScene(gameplaySceneName_);
    RADON_LOGINFO("GameplayState: exited and cleaned up");
}

void GameplayState::Update(float)
{
}

void GameplayState::HandleKeydown(StringHash, VariantMap& data)
{
    using namespace Urho3D;
    int key = data[KeyDown::P_KEY].GetInt();
    if (key == KEY_ESCAPE)
    {
        RADON_LOGINFO("GameplayState: ESC pressed, returning to menu");
        GetSubsystem<GameStateManager>()->ReplaceState(MakeShared<MenuState>(context_));
    }
}
