#include "MenuState.hpp"

#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Window.h>

namespace Radon
{

MenuState::MenuState(Context* context)
    : GameState(context)
{
    scene_->SetName("MenuScene");
}

void MenuState::Enter()
{
    URHO3D_LOGINFO("Entering menu state");
    CreateUI();
}

void MenuState::Exit()
{
    URHO3D_LOGINFO("Exiting menu state");
    GetSubsystem<UI>()->GetRoot()->RemoveAllChildren();
}

void MenuState::Update(float timeStep)
{
    // Update menu logic here
}

void MenuState::CreateUI()
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
    UIElement* root = ui->GetRoot();
    root->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    // Create a Window for the main menu
    SharedPtr<Window> window = MakeShared<Window>(context_);
    root->AddChild(window);
    window->SetStyleAuto();
    window->SetSize(300, 400);
    window->SetAlignment(HA_CENTER, VA_CENTER);
    window->SetName("MainMenu");

    // Create the title text
    SharedPtr<Text> titleText = MakeShared<Text>(context_);
    window->AddChild(titleText);
    titleText->SetStyleAuto();
    titleText->SetName("TitleText");
    titleText->SetText("RADON");
    titleText->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), 25);
    titleText->SetAlignment(HA_CENTER, VA_TOP);
    titleText->SetPosition(0, 20);

    // Create the Play button
    SharedPtr<Button> playButton = MakeShared<Button>(context_);
    window->AddChild(playButton);
    playButton->SetStyleAuto();
    playButton->SetName("PlayButton");
    playButton->SetSize(200, 40);
    playButton->SetAlignment(HA_CENTER, VA_CENTER);
    playButton->SetPosition(0, -40);

    // Add text to the Play button
    SharedPtr<Text> playText = MakeShared<Text>(context_);
    playButton->AddChild(playText);
    playText->SetStyleAuto();
    playText->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), 17);
    playText->SetText("Play Game");
    playText->SetAlignment(HA_CENTER, VA_CENTER);

    // Create the Exit button
    SharedPtr<Button> exitButton = MakeShared<Button>(context_);
    window->AddChild(exitButton);
    exitButton->SetStyleAuto();
    exitButton->SetName("ExitButton");
    exitButton->SetSize(200, 40);
    exitButton->SetAlignment(HA_CENTER, VA_CENTER);
    exitButton->SetPosition(0, 20);

    // Add text to the Exit button
    SharedPtr<Text> exitText = MakeShared<Text>(context_);
    exitButton->AddChild(exitText);
    exitText->SetStyleAuto();
    exitText->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), 17);
    exitText->SetText("Exit Game");
    exitText->SetAlignment(HA_CENTER, VA_CENTER);
}

} // namespace Radon
