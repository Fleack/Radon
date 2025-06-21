#include "ViewportManager.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Renderer.h>

namespace Radon
{

ViewportManager::ViewportManager(Context* context)
    : Object(context)
    , renderer_(nullptr)
{
    Initialize();
}

ViewportManager::~ViewportManager()
{
    Cleanup();
}

void ViewportManager::Initialize()
{
    renderer_ = GetSubsystem<Renderer>();
    if (!renderer_)
    {
        URHO3D_LOGERROR("Failed to initialize ViewportManager: Renderer subsystem not found");
    }
}

void ViewportManager::Cleanup()
{
    for (auto& pair : viewports_)
    {
        ClearViewport(pair.first);
    }
    viewports_.clear();
    enabledViewports_.clear();
}

void ViewportManager::SetupViewport(Scene& scene, Node const& cameraNode, uint8_t viewportIndex)
{
    if (!renderer_)
    {
        URHO3D_LOGERROR("Renderer not initialized");
        return;
    }

    auto* camera = cameraNode.GetComponent<Camera>();
    if (!camera)
    {
        URHO3D_LOGERROR("Camera component not found on camera node");
        return;
    }

    SharedPtr<Viewport> viewport = MakeShared<Viewport>(context_, &scene, camera);
    viewports_[viewportIndex] = viewport;
    enabledViewports_.insert(viewportIndex);
    UpdateViewport(viewportIndex);
}

void ViewportManager::ClearViewport(uint8_t viewportIndex)
{
    if (viewports_.contains(viewportIndex))
    {
        viewports_.erase(viewportIndex);
        enabledViewports_.erase(viewportIndex);
        renderer_->SetViewport(viewportIndex, nullptr);
    }
}

void ViewportManager::SetViewportEnabled(bool enabled, uint8_t viewportIndex)
{
    if (enabled)
    {
        enabledViewports_.insert(viewportIndex);
    }
    else
    {
        enabledViewports_.erase(viewportIndex);
    }
    UpdateViewport(viewportIndex);
}

bool ViewportManager::IsViewportEnabled(uint8_t viewportIndex) const
{
    return enabledViewports_.contains(viewportIndex);
}


void ViewportManager::SetViewportRect(IntRect const& rect, uint8_t viewportIndex)
{
    if (auto viewport = viewports_[viewportIndex])
    {
        viewport->SetRect(rect);
        UpdateViewport(viewportIndex);
    }
}

uint8_t ViewportManager::GetViewportCount() const
{
    return viewports_.size();
}

void ViewportManager::UpdateViewport(uint8_t viewportIndex)
{
    if (auto viewport = viewports_[viewportIndex])
    {
        if (enabledViewports_.contains(viewportIndex))
        {
            renderer_->SetViewport(viewportIndex, viewport);
        }
        else
        {
            renderer_->SetViewport(viewportIndex, nullptr);
        }
    }
}

} // namespace Radon
