#include "GameApplication.hpp"

#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

using namespace Radon;

void GameApplication::Setup()
{
    // Window settings
    engineParameters_[EP_WINDOW_TITLE] = "Radon";
    engineParameters_[EP_WINDOW_WIDTH] = 1024;
    engineParameters_[EP_WINDOW_HEIGHT] = 768;
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_VSYNC] = true;

    // Logs settings
    engineParameters_[EP_LOG_QUIET] = false;
    engineParameters_[EP_LOG_LEVEL] = LOG_INFO;
    engineParameters_[EP_LOG_NAME] = "Radon.log";
}

void GameApplication::Start()
{
    auto scene = MakeShared<Scene>(context_);

    Node* cameraNode = scene->CreateChild("Camera");
    auto* camera = cameraNode->CreateComponent<Camera>();

    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport = MakeShared<Viewport>(context_, scene, camera);
    renderer->SetViewport(0, viewport);

    Node* lightNode = scene->CreateChild("DirectionalLight");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);

    itemRegistry_.LoadAll(context_, "Items");
    if (auto* item = itemRegistry_.Get("medkit"))
        URHO3D_LOGINFO("Got item 'medkit', name = {}", item->name_);
}

void GameApplication::Stop()
{
    URHO3D_LOGINFO("Radon shutting down");
}
