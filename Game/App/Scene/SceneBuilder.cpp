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
    // For menu scene, we just need basic components and a light
    scene->CreateComponent<Octree>();

    // Create a simple ambient light for the menu scene
    Node* lightNode = scene->CreateChild("MenuLight");
    auto* light = lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetColor(Color(0.5f, 0.5f, 0.5f));
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

} // namespace Radon
