#include "SceneBuilder.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h>

namespace Radon
{

SceneBuilder::SceneBuilder(Context* context)
    : Object(context)
{
}

void SceneBuilder::SetupGameplayScene(Scene* scene)
{
    scene->CreateComponent<Octree>();
    scene->CreateComponent<DebugRenderer>();

    CreateLight(scene);
    CreateFloor(scene);
    CreateObjects(scene);
}

void SceneBuilder::SetupMenuScene(Scene* scene)
{
    // Create basic components
    scene->CreateComponent<Octree>();
    scene->CreateComponent<DebugRenderer>();

    // Setup lighting
    CreateLight(scene);

    // Create floor and objects for an interactive menu background
    CreateFloor(scene);
    CreateMenuBackgroundObjects(scene);
}

Node* SceneBuilder::CreateCamera(Scene* scene)
{
    Node* cameraNode = scene->CreateChild("Camera");
    cameraNode->SetPosition({0.0f, 5.0f, -10.0f});
    cameraNode->LookAt(Vector3::ZERO);

    auto* camera = cameraNode->CreateComponent<Camera>();
    camera->SetFarClip(300.0f);

    return cameraNode;
}

Node* SceneBuilder::CreateMenuCamera(Scene* scene, Vector3 const& position)
{
    // Create a camera with a position that shows off the scene nicely
    Node* cameraNode = scene->CreateChild("MenuCamera");
    cameraNode->SetPosition(position);

    // Position the camera to look at the scene at a slight angle
    Vector3 lookTarget(0.0f, 0.0f, 0.0f);
    cameraNode->LookAt(lookTarget);

    auto* camera = cameraNode->CreateComponent<Camera>();
    camera->SetFarClip(300.0f);
    camera->SetFov(45.0f); // Slightly wider FOV for menu

    return cameraNode;
}

Node* SceneBuilder::CreateLight(Scene* scene)
{
    Node* lightNode = scene->CreateChild("DirectionalLight");
    lightNode->SetDirection({0.6f, -1.0f, 0.8f});
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
    light->SetSpecularIntensity(0.5f);

    // Add a subtle blue ambient light for the menu scene
    Node* ambientNode = scene->CreateChild("AmbientLight");
    auto* ambientLight = ambientNode->CreateComponent<Light>();
    ambientLight->SetLightType(LIGHT_SPOT);
    ambientLight->SetColor(Color(0.1f, 0.1f, 0.15f));

    return lightNode;
}

void SceneBuilder::CreateFloor(Scene* scene)
{
    auto* cache = GetSubsystem<ResourceCache>();

    Node* floorNode = scene->CreateChild("Floor");
    floorNode->SetScale({100.0f, 1.0f, 100.0f});
    floorNode->SetPosition({0.0f, -0.5f, 0.0f});
    auto* floorModel = floorNode->CreateComponent<StaticModel>();
    floorModel->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
    floorModel->SetMaterial(cache->GetResource<Material>("Materials/Floor.xml"));
}

void SceneBuilder::CreateObjects(Scene* scene)
{
    auto* cache = GetSubsystem<ResourceCache>();

    Node* boxNode = scene->CreateChild("Box");
    boxNode->SetPosition({0.0f, 1.0f, 5.0f});
    auto* box = boxNode->CreateComponent<StaticModel>();
    box->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    box->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
}

void SceneBuilder::CreateMenuBackgroundObjects(Scene* scene)
{
    auto* cache = GetSubsystem<ResourceCache>();

    // Create several objects for an interesting background
    // Central feature - a stack of boxes
    for (int i = 0; i < 3; ++i)
    {
        Node* boxNode = scene->CreateChild("MenuBox" + i);
        boxNode->SetPosition({0.0f, 0.5f + i * 1.1f, 0.0f});
        float scale = 1.0f - (i * 0.2f);
        boxNode->SetScale(scale);
        boxNode->SetRotation(Quaternion(0.0f, i * 15.0f, 0.0f));
        auto* box = boxNode->CreateComponent<StaticModel>();
        box->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        box->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
    }

    // Add some scattered objects around the scene
    Vector3 positions[] = {
        {-4.0f, 0.5f, -3.0f},
        {3.0f, 0.5f, -5.0f},
        {5.0f, 0.5f, 2.0f},
        {-6.0f, 0.5f, 4.0f},
    };

    for (int i = 0; i < 4; ++i)
    {
        Node* objNode = scene->CreateChild("SceneObject" + i);
        objNode->SetPosition(positions[i]);
        objNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));

        auto* model = objNode->CreateComponent<StaticModel>();
        model->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        model->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
    }
}

} // namespace Radon
