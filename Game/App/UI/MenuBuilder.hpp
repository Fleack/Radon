#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Window.h>

namespace Radon
{
using namespace Urho3D;

class MenuBuilder : public Object
{
    URHO3D_OBJECT(MenuBuilder, Object);

public:
    explicit MenuBuilder(Context* context);
    ~MenuBuilder() override = default;

    void CreateMainMenu();

    // Event handlers for buttons - these will be connected to in the calling state
    void SetPlayButtonHandler(Object* receiver, void (Object::*handler)(StringHash, VariantMap&));
    void SetExitButtonHandler(Object* receiver, void (Object::*handler)(StringHash, VariantMap&));

private:
    SharedPtr<Window> CreateMenuWindow(const ea::string& title);
    SharedPtr<Button> CreateButton(UIElement* parent, const ea::string& name, const ea::string& text, const IntVector2& position);
    SharedPtr<Text> CreateText(UIElement* parent, const ea::string& text, int fontSize, const IntVector2& position);

    void SetupInputMode();
    void StyleWindowForTransparency(Window& window);

private:
    SharedPtr<Window> menuWindow_;
};

} // namespace Radon
