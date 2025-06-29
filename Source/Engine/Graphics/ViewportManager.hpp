#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Scene/Scene.h>

namespace Radon::Engine::Graphics
{
using namespace Urho3D;

class ViewportManager : public Object
{
    URHO3D_OBJECT(ViewportManager, Object);

public:
    explicit ViewportManager(Context* context);
    ~ViewportManager() override;

    void SetupViewport(Scene& scene, Node const& cameraNode, uint8_t viewportIndex);
    void ClearViewport(uint8_t viewportIndex = 0);
    void SetViewportEnabled(bool enabled, uint8_t viewportIndex = 0);
    bool IsViewportEnabled(uint8_t viewportIndex = 0) const;
    void SetViewportRect(IntRect const&, uint8_t viewportIndex = 0);
    uint8_t GetViewportCount() const;

private:
    void Initialize();
    void Cleanup();
    void UpdateViewport(uint8_t viewportIndex);

    SharedPtr<Renderer> renderer_;
    ea::hash_map<uint8_t, SharedPtr<Viewport>> viewports_;
    ea::hash_set<uint8_t> enabledViewports_{};
};

} // namespace Radon::Engine::Graphics
