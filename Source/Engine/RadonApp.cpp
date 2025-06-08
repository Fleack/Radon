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
    auto cache = GetSubsystem<ResourceCache>();
    auto renderer = GetSubsystem<Renderer>();

    m_scene = MakeShared<Scene>(context_);
    if (!m_scene->LoadFile("Scenes/Default.scene"))
    {
        URHO3D_LOGERROR("Failed to load scene file!");
        return;
    }

    Node* cameraNode = m_scene->GetChild("Camera", true);
    if (!cameraNode)
    {
        cameraNode = m_scene->CreateChild("Camera");
        cameraNode->CreateComponent<Camera>();
    }

    Camera* camera = cameraNode->GetComponent<Camera>();
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
