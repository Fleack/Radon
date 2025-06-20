#include "GameplayState.hpp"

#include "App/States/GameStateManager.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
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

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(GameplayState, HandleUpdate));
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
    // Update gameplay logic here
}

void GameplayState::SetupScene()
{
    auto* cache = GetSubsystem<ResourceCache>();
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<DebugRenderer>();

    cameraNode_ = scene_->CreateChild("Camera");
    cameraNode_->SetPosition({0.0f, 5.0f, -10.0f});
    cameraNode_->LookAt(Vector3::ZERO);

    Quaternion initialRot = cameraNode_->GetRotation();
    yaw_ = initialRot.YawAngle();
    pitch_ = initialRot.PitchAngle();

    auto* camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(300.0f);

    Node* lightNode = scene_->CreateChild("DirectionalLight");
    lightNode->SetDirection({0.6f, -1.0f, 0.8f});
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
    light->SetSpecularIntensity(0.5f);

    Node* floorNode = scene_->CreateChild("Floor");
    floorNode->SetScale({100.0f, 1.0f, 100.0f});
    floorNode->SetPosition({0.0f, -0.5f, 0.0f});
    auto* floorModel = floorNode->CreateComponent<StaticModel>();
    floorModel->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
    floorModel->SetMaterial(cache->GetResource<Material>("Materials/Floor.xml"));

    Node* boxNode = scene_->CreateChild("Box");
    boxNode->SetPosition({0.0f, 1.0f, 5.0f});
    auto* box = boxNode->CreateComponent<StaticModel>();
    box->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    box->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));

    auto* input = GetSubsystem<Input>();
    input->SetMouseMode(MM_RELATIVE);
    input->SetMouseVisible(false);
}

void GameplayState::SetupViewport()
{
    auto* renderer = GetSubsystem<Renderer>();
    auto* camera = cameraNode_->GetComponent<Camera>();
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

void GameplayState::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
    int key = eventData[KeyDown::P_KEY].GetInt();
    if (key == KEY_ESCAPE)
    {
        GetSubsystem<GameStateManager>()->PopState();
    }
}

void GameplayState::HandleUpdate(StringHash, VariantMap& eventData)
{
    float dt = eventData[Update::P_TIMESTEP].GetFloat();
    auto* input = GetSubsystem<Input>();

    yaw_ += static_cast<float>(input->GetMouseMoveX()) * lookSensitivity_;
    pitch_ += static_cast<float>(input->GetMouseMoveY()) * lookSensitivity_;
    pitch_ = Clamp(pitch_, -89.0f, 89.0f);

    cameraNode_->SetRotation(Quaternion(pitch_, yaw_, 0.0f));

    Vector3 dir;
    if (input->GetKeyDown(KEY_W)) dir += Vector3::FORWARD;
    if (input->GetKeyDown(KEY_S)) dir += Vector3::BACK;
    if (input->GetKeyDown(KEY_A)) dir += Vector3::LEFT;
    if (input->GetKeyDown(KEY_D)) dir += Vector3::RIGHT;
    if (!dir.Equals(Vector3::ZERO))
        cameraNode_->Translate(dir.Normalized() * moveSpeed_ * dt, TS_LOCAL);
}

} // namespace Radon
