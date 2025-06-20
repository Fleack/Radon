#include "GameplayState.hpp"

#include "App/States/GameStateManager.hpp"

#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/UI/UI.h>

namespace Radon
{

GameplayState::GameplayState(Context* context)
    : GameState(context)
    , sceneBuilder_(MakeShared<SceneBuilder>(context))
    , cameraController_(MakeShared<CameraController>(context))
    , hudBuilder_(MakeShared<HUDBuilder>(context))
    , viewportManager_(MakeShared<ViewportManager>(context))
{
    scene_->SetName("GameplayScene");
}

void GameplayState::Enter()
{
    URHO3D_LOGINFO("Entering gameplay state");

    // Setup scene and camera
    sceneBuilder_->SetupGameplayScene(scene_);
    cameraNode_ = sceneBuilder_->CreateCamera(scene_);

    // Setup viewport with the camera
    viewportManager_->SetupViewport(scene_, cameraNode_);

    // Setup camera controller
    cameraController_->Setup(cameraNode_);

    // Create HUD
    hudBuilder_->CreateGameplayHUD();

    // Subscribe to key events for state management
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(GameplayState, HandleKeyDown));
}

void GameplayState::Exit()
{
    URHO3D_LOGINFO("Exiting gameplay state");
    UnsubscribeFromAllEvents();
    GetSubsystem<UI>()->GetRoot()->RemoveAllChildren();
    scene_->RemoveAllChildren();
}

void GameplayState::Update(float timeStep)
{
    // Update gameplay-specific logic here
}

void GameplayState::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
    int key = eventData[KeyDown::P_KEY].GetInt();
    if (key == KEY_ESCAPE)
    {
        GetSubsystem<GameStateManager>()->PopState();
    }
}

} // namespace Radon
