#include "GameplayState.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

namespace Radon
{

GameplayState::GameplayState(Context* context)
    : GameState(context)
{
    scene_->SetName("GameplayScene");
}

void GameplayState::Enter()
{
    URHO3D_LOGINFO("Entering gameplay state");
    SetupScene();
    SetupViewport();
    CreateHUD();
}

void GameplayState::Exit()
{
    URHO3D_LOGINFO("Exiting gameplay state");
    GetSubsystem<UI>()->GetRoot()->RemoveAllChildren();
}

void GameplayState::Update(float timeStep)
{
    // Update gameplay logic here
}

void GameplayState::SetupScene()
{
    // Create a camera
    Node* cameraNode = scene_->CreateChild("Camera");
    cameraNode->SetPosition(Vector3(0.0f, 5.0f, -10.0f));
    cameraNode->LookAt(Vector3::ZERO);
    auto* camera = cameraNode->CreateComponent<Camera>();
    camera->SetFarClip(300.0f);

    // Create a directional light
    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
    light->SetSpecularIntensity(0.5f);

    // Create a floor (placeholder)
    Node* floorNode = scene_->CreateChild("Floor");
    floorNode->SetScale(Vector3(100.0f, 1.0f, 100.0f));
    floorNode->SetPosition(Vector3(0.0f, -0.5f, 0.0f));
}

void GameplayState::SetupViewport()
{
    auto* renderer = GetSubsystem<Renderer>();
    auto* camera = scene_->GetChild("Camera")->GetComponent<Camera>();

    // Set up viewport
    SharedPtr<Viewport> viewport = MakeShared<Viewport>(context_, scene_, camera);
    renderer->SetViewport(0, viewport);
}

void GameplayState::CreateHUD()
{
    auto* cache = GetSubsystem<ResourceCache>();
    auto* ui = GetSubsystem<UI>();
    UIElement* root = ui->GetRoot();
    root->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    // Create a Text element for displaying game information
    SharedPtr<Text> infoText = MakeShared<Text>(context_);
    root->AddChild(infoText);
    infoText->SetStyleAuto();
    infoText->SetName("GameInfo");
    infoText->SetText("Radon Game - Gameplay State");
    infoText->SetFont(cache->GetResource<Font>("Fonts/Dead Kansas.ttf"), 12);
    infoText->SetAlignment(HA_LEFT, VA_TOP);
    infoText->SetPosition(10, 10);
}

} // namespace Radon
