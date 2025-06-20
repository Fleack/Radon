#include "ViewportManager.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Renderer.h>

namespace Radon
{

ViewportManager::ViewportManager(Context* context)
    : Object(context)
{
}

void ViewportManager::SetupViewport(Scene* scene, Node* cameraNode)
{
    auto* renderer = GetSubsystem<Renderer>();
    auto* camera = cameraNode->GetComponent<Camera>();
    SharedPtr<Viewport> viewport = MakeShared<Viewport>(context_, scene, camera);
    renderer->SetViewport(0, viewport);
}

} // namespace Radon
