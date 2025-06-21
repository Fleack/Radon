#include "MenuState.hpp"

#include "App/States/GameStateManager.hpp"
#include "App/States/GameplayState.hpp"

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>

namespace Radon
{

MenuState::MenuState(Context* context)
    : GameState(context)
    , sceneBuilder_(MakeShared<SceneBuilder>(context))
    , menuBuilder_(MakeShared<MenuBuilder>(context))
    , viewportManager_(MakeShared<ViewportManager>(context))
    , sceneAnimator_(MakeShared<MenuSceneAnimator>(context))
{
    scene_->SetName("MenuScene");
}

void MenuState::Enter()
{
    URHO3D_LOGINFO("Entering menu state");

    sceneBuilder_->SetupMenuScene(scene_);

    cameraNode_ = sceneBuilder_->CreateMenuCamera(scene_, Vector3(12.0f, 4.0f, 0.0f));
    viewportManager_->SetupViewport(*scene_, *cameraNode_, 1);

    sceneAnimator_->Setup(scene_, cameraNode_);

    menuBuilder_->CreateMainMenu();

    menuBuilder_->SetPlayButtonHandler(this, static_cast<void (Object::*)(StringHash, VariantMap&)>(&MenuState::HandlePlayPressed));
    menuBuilder_->SetExitButtonHandler(this, static_cast<void (Object::*)(StringHash, VariantMap&)>(&MenuState::HandleExitPressed));
}

void MenuState::Exit()
{
    URHO3D_LOGINFO("Exiting menu state");
    UnsubscribeFromAllEvents();
    GetSubsystem<UI>()->GetRoot()->RemoveAllChildren();
    scene_->RemoveAllChildren();
}

void MenuState::Update(float timeStep)
{
    // No need for explicit updates as our MenuSceneAnimator handles them through events
}

void MenuState::HandlePlayPressed(StringHash, VariantMap&)
{
    GetSubsystem<GameStateManager>()->PushState<GameplayState>();
}

void MenuState::HandleExitPressed(StringHash, VariantMap&)
{
    GetSubsystem<Engine>()->Exit();
}

} // namespace Radon
