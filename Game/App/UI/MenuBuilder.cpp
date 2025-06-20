#include "MenuBuilder.hpp"

#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>

namespace Radon
{

MenuBuilder::MenuBuilder(Context* context)
    : Object(context)
{
}

void MenuBuilder::CreateMainMenu()
{
    // Create the main menu window
    menuWindow_ = CreateMenuWindow("RADON");

    // Create buttons
    CreateButton(menuWindow_, "PlayButton", "Play Game", IntVector2(0, -40));
    CreateButton(menuWindow_, "ExitButton", "Exit Game", IntVector2(0, 20));

    // Configure input for menus
    SetupInputMode();
}

SharedPtr<Window> MenuBuilder::CreateMenuWindow(const ea::string& title)
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
    UIElement* root = ui->GetRoot();
    root->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    // Create a Window for the menu
    SharedPtr<Window> window = MakeShared<Window>(context_);
    root->AddChild(window);
    window->SetStyleAuto();
    window->SetSize(300, 400);
    window->SetAlignment(HA_CENTER, VA_CENTER);
    window->SetName("MainMenu");

    // Create the title text
    CreateText(window, title, 25, IntVector2(0, 20));

    return window;
}

SharedPtr<Button> MenuBuilder::CreateButton(UIElement* parent, const ea::string& name, const ea::string& text, const IntVector2& position)
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create the button
    SharedPtr<Button> button = MakeShared<Button>(context_);
    parent->AddChild(button);
    button->SetStyleAuto();
    button->SetName(name);
    button->SetSize(200, 40);
    button->SetAlignment(HA_CENTER, VA_CENTER);
    button->SetPosition(position);

    // Add text to the button
    SharedPtr<Text> buttonText = MakeShared<Text>(context_);
    button->AddChild(buttonText);
    buttonText->SetStyleAuto();
    buttonText->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), 17);
    buttonText->SetText(text);
    buttonText->SetAlignment(HA_CENTER, VA_CENTER);

    return button;
}

SharedPtr<Text> MenuBuilder::CreateText(UIElement* parent, const ea::string& text, int fontSize, const IntVector2& position)
{
    auto* cache = GetSubsystem<ResourceCache>();

    SharedPtr<Text> textElement = MakeShared<Text>(context_);
    parent->AddChild(textElement);
    textElement->SetStyleAuto();
    textElement->SetText(text);
    textElement->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), fontSize);
    textElement->SetAlignment(HA_CENTER, VA_TOP);
    textElement->SetPosition(position);

    return textElement;
}

void MenuBuilder::SetPlayButtonHandler(Object* receiver, void (Object::*handler)(StringHash, VariantMap&))
{
    if (menuWindow_)
    {
        Button* playButton = menuWindow_->GetChildDynamicCast<Button>("PlayButton", true);
        if (playButton)
        {
            SubscribeToEvent(playButton, E_RELEASED, handler);
        }
    }
}

void MenuBuilder::SetExitButtonHandler(Object* receiver, void (Object::*handler)(StringHash, VariantMap&))
{
    if (menuWindow_)
    {
        Button* exitButton = menuWindow_->GetChildDynamicCast<Button>("ExitButton", true);
        if (exitButton)
        {
            SubscribeToEvent(exitButton, E_RELEASED, handler);
        }
    }
}

void MenuBuilder::SetupInputMode()
{
    // Let the OS cursor be visible and free to move
    auto* input = GetSubsystem<Input>();
    input->SetMouseMode(MM_FREE);
    input->SetMouseVisible(true);
}

} // namespace Radon
