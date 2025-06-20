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
{
    scene_->SetName("MenuScene");
}

void MenuState::Enter()
{
    URHO3D_LOGINFO("Entering menu state");

    // Setup basic menu scene
    sceneBuilder_->SetupMenuScene(scene_);

    // Setup camera and viewport
    cameraNode_ = sceneBuilder_->CreateCamera(scene_);
    viewportManager_->SetupViewport(scene_, cameraNode_);

    // Create the menu UI
    menuBuilder_->CreateMainMenu();

    // Connect menu button handlers
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
    // Update menu-specific logic here (if needed)
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
