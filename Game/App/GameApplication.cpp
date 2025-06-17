#include "GameApplication.hpp"

#include "Urho3D/Engine/EngineDefs.h"
#include "Urho3D/Graphics/Camera.h"
#include "Urho3D/Graphics/Light.h"
#include "Urho3D/Graphics/Renderer.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/IO/Log.h"
#include "Urho3D/Resource/ResourceCache.h"
#include "Urho3D/Scene/Scene.h"

void GameApplication::Setup()
{
    engineParameters_[EP_WINDOW_TITLE] = "Radon";
    engineParameters_[EP_WINDOW_WIDTH] = 1024;
    engineParameters_[EP_WINDOW_HEIGHT] = 768;
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_VSYNC] = true;
    engineParameters_[EP_LOG_LEVEL] = LOG_DEBUG;
    engineParameters_[EP_LOG_NAME] = "Radon.log";
    engineParameters_[EP_RESOURCE_PATHS] = "Data/";
}

void GameApplication::Start()
{
    // Create scene
    auto scene = MakeShared<Scene>(context_);
    
    // Create camera node
    Node* cameraNode = scene->CreateChild("Camera");
    auto* camera = cameraNode->CreateComponent<Camera>();
    
    // Set up viewport
    auto* renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport = MakeShared<Viewport>(context_, scene, camera);
    renderer->SetViewport(0, viewport);
    
    // Add some basic lighting
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