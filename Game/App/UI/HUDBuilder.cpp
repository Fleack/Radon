#include "HUDBuilder.hpp"

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/UI.h>

namespace Radon
{

HUDBuilder::HUDBuilder(Context* context)
    : Object(context)
{
}

void HUDBuilder::CreateGameplayHUD()
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
    UIElement* root = ui->GetRoot();
    root->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    // Create a Text element for displaying game information
    CreateInfoText("Radon Game - Gameplay State");
}

SharedPtr<Text> HUDBuilder::CreateInfoText(const ea::string& content)
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
    UIElement* root = ui->GetRoot();

    SharedPtr<Text> infoText = MakeShared<Text>(context_);
    root->AddChild(infoText);
    infoText->SetStyleAuto();
    infoText->SetName("GameInfo");
    infoText->SetText(content);
    infoText->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), 12);
    infoText->SetAlignment(HA_LEFT, VA_TOP);
    infoText->SetPosition(10, 10);

    return infoText;
}

} // namespace Radon
