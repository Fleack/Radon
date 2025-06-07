#include "RadonApp.hpp"

#include "Core.SamplePlugin/SampleComponent.h"
#include "CrashHandler.hpp"
#include "Urho3D/Core/CoreEvents.h"
#include "Urho3D/Engine/EngineDefs.h"
#include "Urho3D/Graphics/Camera.h"
#include "Urho3D/Graphics/Light.h"
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Graphics/Octree.h"
#include "Urho3D/Graphics/Renderer.h"
#include "Urho3D/Graphics/StaticModel.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/Graphics/Zone.h"
#include "Urho3D/Input/Input.h"
#include "Urho3D/RenderPipeline/RenderPath.h"
#include "Urho3D/Resource/ResourceCache.h"
#include "Urho3D/Scene/Node.h"
#include "Urho3D/Scene/Scene.h"
#include "Urho3D/SystemUI/SystemUI.h"

RadonApp::RadonApp(Context* context)
    : Application(context) {}

void RadonApp::Setup()
{
    InstallCrashHandler();

    engineParameters_[EP_WINDOW_TITLE] = "Radon";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_HEADLESS] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_LOG_NAME] = "Radon.log";
    engineParameters_[EP_RESOURCE_PATHS] = "CoreData;Data";
}

void RadonApp::Start()
{
    SampleComponent::RegisterObject(context_);

    auto cache = GetSubsystem<ResourceCache>();
    auto renderer = GetSubsystem<Renderer>();

    // Create scene.
    m_scene = MakeShared<Scene>(context_);
    m_scene->CreateComponent<Octree>();
    cache->BackgroundLoadResource<Scene>("Scenes/MyScene.xml");

    // Create camera.
    Node* cameraNode = m_scene->CreateChild("Camera");
    Camera* camera = cameraNode->CreateComponent<Camera>();

    // Create zone.
    Zone* zone = m_scene->CreateComponent<Zone>();
    zone->SetFogColor(0xC9C0BB_rgb);
    zone->SetAmbientColor(0x706B64_rgb);

    // Create box geometry.
    m_geometryNode = m_scene->CreateChild("Box");
    m_geometryNode->SetPosition(Vector3{0.0f, 0.0f, 5.0f});
    m_geometryNode->SetRotation(Quaternion{-30.0f, 60.0f, 50.0f});
    auto* geometry = m_geometryNode->CreateComponent<StaticModel>();
    geometry->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    geometry->SetMaterial(cache->GetResource<Material>("Materials/DefaultGrey.xml"));
    auto* sampleComponent = m_geometryNode->CreateComponent<SampleComponent>();
    sampleComponent->SetAxis(Vector3::UP);
    sampleComponent->SetRotationSpeed(10.0f);

    // Create light.
    Node* lightNode = m_scene->CreateChild("Light");
    lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
    Light* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);

    // Create viewport.
    auto const viewport = MakeShared<Viewport>(context_, m_scene, camera);
    renderer->SetViewport(0, viewport);

    SubscribeToEvent(E_UPDATE, &RadonApp::Update);
}

void RadonApp::Stop()
{
}

void RadonApp::Update(VariantMap&)
{
    auto input = GetSubsystem<Input>();
    if (input->GetKeyPress(KEY_ESCAPE))
        SendEvent(E_EXITREQUESTED);
}
