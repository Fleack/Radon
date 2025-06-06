#include "RadonApp.hpp"

#include "CrashHandler.hpp"
#include "Urho3D/Engine/EngineDefs.h"
#include "Urho3D/Graphics/Octree.h"
#include "Urho3D/RenderPipeline/RenderPath.h"
#include "Urho3D/SystemUI/SystemUI.h"

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

RadonApp::RadonApp(Context* context)
    : Application(context) {}

void RadonApp::Setup()
{
    InstallCrashHandler();

    engineParameters_[EP_WINDOW_TITLE] = "Radon";
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_HEADLESS] = false;
    engineParameters_[EP_LOG_NAME] = "Radon.log";
    engineParameters_[EP_RESOURCE_PREFIX_PATHS] = "../Assets";
    engineParameters_[EP_RESOURCE_PATHS] = "CoreData;Data";
}

void RadonApp::Start()
{
    m_scene = new Scene(context_);
    m_scene->CreateComponent<Octree>();

    m_cameraNode = m_scene->CreateChild("Camera");
    Camera* camera = m_cameraNode->CreateComponent<Camera>();
    m_cameraNode->SetPosition(Vector3(0, 2, -10));

    SharedPtr<Viewport> viewport(new Viewport(context_, m_scene, camera));
    GetSubsystem<Renderer>()->SetViewport(0, viewport);
}

void RadonApp::Stop()
{
    m_scene.Reset();
}
