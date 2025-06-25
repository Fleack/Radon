#include "ViewportManager.hpp"

#include "App/Logger/Logger.hpp"

#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Renderer.h>

namespace Radon::Graphics
{

ViewportManager::ViewportManager(Context* context)
    : Object(context)
    , renderer_(nullptr)
{
    RADON_LOGDEBUG("ViewportManager: constructor called");
    Initialize();
}

ViewportManager::~ViewportManager()
{
    RADON_LOGDEBUG("ViewportManager: destructor called");
    Cleanup();
}

void ViewportManager::Initialize()
{
    RADON_LOGDEBUG("ViewportManager: Initialize called");
    renderer_ = GetSubsystem<Renderer>();
    if (!renderer_)
    {
        RADON_LOGERROR("Failed to initialize ViewportManager: Renderer subsystem not found");
    }
    else
    {
        RADON_LOGINFO("ViewportManager: Renderer subsystem acquired");
    }
}

void ViewportManager::Cleanup()
{
    RADON_LOGDEBUG("ViewportManager: Cleanup called");
    for (auto& pair : viewports_)
    {
        RADON_LOGDEBUG("ViewportManager: Clearing viewport {}", pair.first);
        ClearViewport(pair.first);
    }
    viewports_.clear();
    enabledViewports_.clear();
    RADON_LOGINFO("ViewportManager: All viewports cleared");
}

void ViewportManager::SetupViewport(Scene& scene, Node const& cameraNode, uint8_t viewportIndex)
{
    RADON_LOGDEBUG("ViewportManager: SetupViewport called for index {}", viewportIndex);
    if (!renderer_)
    {
        RADON_LOGERROR("Renderer not initialized");
        return;
    }

    auto* camera = cameraNode.GetComponent<Camera>();
    if (!camera)
    {
        RADON_LOGERROR("Camera component not found on camera node");
        return;
    }

    SharedPtr<Viewport> viewport = MakeShared<Viewport>(context_, &scene, camera);
    viewports_[viewportIndex] = viewport;
    enabledViewports_.insert(viewportIndex);
    RADON_LOGINFO("ViewportManager: Viewport {} set up and enabled", viewportIndex);
    UpdateViewport(viewportIndex);
}

void ViewportManager::ClearViewport(uint8_t viewportIndex)
{
    RADON_LOGDEBUG("ViewportManager: ClearViewport called for index {}", viewportIndex);
    if (viewports_.contains(viewportIndex))
    {
        viewports_.erase(viewportIndex);
        enabledViewports_.erase(viewportIndex);
        renderer_->SetViewport(viewportIndex, nullptr);
        RADON_LOGINFO("ViewportManager: Viewport {} cleared", viewportIndex);
    }
    else
    {
        RADON_LOGWARN("ViewportManager: Tried to clear non-existent viewport {}", viewportIndex);
    }
}

void ViewportManager::SetViewportEnabled(bool enabled, uint8_t viewportIndex)
{
    RADON_LOGDEBUG("ViewportManager: SetViewportEnabled({}, {})", enabled, viewportIndex);
    if (enabled)
    {
        enabledViewports_.insert(viewportIndex);
        RADON_LOGINFO("ViewportManager: Viewport {} enabled", viewportIndex);
    }
    else
    {
        enabledViewports_.erase(viewportIndex);
        RADON_LOGINFO("ViewportManager: Viewport {} disabled", viewportIndex);
    }
    UpdateViewport(viewportIndex);
}

bool ViewportManager::IsViewportEnabled(uint8_t viewportIndex) const
{
    return enabledViewports_.contains(viewportIndex);
}

void ViewportManager::SetViewportRect(IntRect const& rect, uint8_t viewportIndex)
{
    RADON_LOGDEBUG("ViewportManager: SetViewportRect called for index {}", viewportIndex);
    if (auto viewport = viewports_[viewportIndex])
    {
        viewport->SetRect(rect);
        RADON_LOGINFO("ViewportManager: Viewport %u rect set to [{},{},{},{}]", viewportIndex, rect.left_, rect.top_, rect.right_, rect.bottom_);
        UpdateViewport(viewportIndex);
    }
    else
    {
        RADON_LOGWARN("ViewportManager: Tried to set rect for non-existent viewport {}", viewportIndex);
    }
}

uint8_t ViewportManager::GetViewportCount() const
{
    return viewports_.size();
}

void ViewportManager::UpdateViewport(uint8_t viewportIndex)
{
    RADON_LOGDEBUG("ViewportManager: UpdateViewport called for index {}", viewportIndex);
    if (auto viewport = viewports_[viewportIndex])
    {
        if (enabledViewports_.contains(viewportIndex))
        {
            renderer_->SetViewport(viewportIndex, viewport);
            RADON_LOGINFO("ViewportManager: Viewport {} updated and enabled", viewportIndex);
        }
        else
        {
            renderer_->SetViewport(viewportIndex, nullptr);
            RADON_LOGINFO("ViewportManager: Viewport {} updated and disabled", viewportIndex);
        }
    }
    else
    {
        RADON_LOGWARN("ViewportManager: Tried to update non-existent viewport {}", viewportIndex);
    }
}

} // namespace Radon
