#pragma once

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Urho3D
{
class Node;
class Scene;
class Graphics;
class Renderer;
} // namespace Urho3D

namespace Radon::Engine::UI
{

class DebugHUD final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(DebugHUD, LogicComponent);

public:
    explicit DebugHUD(Urho3D::Context* context);
    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void Update(float timeStep) override;

    void SetVisible(bool visible) { visible_ = visible; }
    bool IsVisible() const { return visible_; }

private:
    void HandleKeyDown(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleKeyUp(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void UpdateMouseMode() const;

    void RenderMainWindow();
    void RenderPerformanceTab();
    void RenderRendererTab() const;
    void RenderSceneTab();
    void RenderSystemTab() const;
    void RenderPlayerTab();
    void RenderConsoleTab();

    void UpdatePerformanceStats();
    void UpdateRendererStats();
    void UpdateSceneStats();
    void UpdateSystemStats();

    void AddLogMessage(ea::string const& message, ea::string const& level = "INFO");

private:
    bool visible_{false};
    bool cursorEnabled_{false};
    
    // Performance stats
    Urho3D::Timer fpsTimer_;
    unsigned frameCount_{0};
    float currentFPS_{0.0f};
    float minFPS_{9999.0f};
    float maxFPS_{0.0f};
    float avgFPS_{0.0f};
    float fpsAccumulator_{0.0f};
    int fpsSamples_{0};
    ea::vector<float> fpsHistory_;
    
    // Performance warnings tracking
    float lowFpsTime_{0.0f};
    float highFrameTimeTime_{0.0f};
    static constexpr float WARNING_THRESHOLD_TIME = 2.0f; // Show warnings after 2 seconds

    // Frame timing
    float frameTime_{0.0f};
    float minFrameTime_{9999.0f};
    float maxFrameTime_{0.0f};
    ea::vector<float> frameTimeHistory_;

    // Renderer stats
    unsigned geometries_{0};
    unsigned lights_{0};
    unsigned shadowMaps_{0};
    unsigned occluders_{0};
    unsigned views_{0};

    // Scene stats
    unsigned nodeCount_{0};
    unsigned componentCount_{0};
    unsigned drawableCount_{0};

    // System stats
    ea::string gpuName_;
    ea::string resolution_;
    bool vsync_{false};
    bool fullscreen_{false};

    // Player stats
    float playerHealth_{100.0f};
    float playerStamina_{100.0f};
    
    // Smoothed player position/rotation for stable display
    mutable Urho3D::Vector3 smoothedPlayerPosition_{0.0f, 0.0f, 0.0f};
    mutable Urho3D::Vector3 smoothedPlayerRotation_{0.0f, 0.0f, 0.0f};
    mutable float lastPlayerUpdateTime_{0.0f};

    // Console
    ea::vector<ea::string> consoleMessages_;
    char consoleInput_[256]{};
    bool autoScrollConsole_{true};

    // UI state
    bool showPerformanceTab_{true};
    bool showRendererTab_{true};
    bool showSceneTab_{true};
    bool showSystemTab_{true};
    bool showPlayerTab_{true};
    bool showConsoleTab_{true};

    // History size
    static constexpr size_t MAX_HISTORY_SIZE = 100;
    static constexpr size_t MAX_CONSOLE_LINES = 500;
};

} // namespace Radon::Engine::UI
