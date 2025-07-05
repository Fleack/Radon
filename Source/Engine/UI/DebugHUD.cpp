#include "DebugHUD.hpp"
#include "Engine/Core/Logger.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

#include <ImGui/imgui.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Drawable.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/SystemUI/SystemUI.h>

using namespace Urho3D;
using namespace Radon::Engine::UI;

namespace
{
// Helper function to format float with precision // TODO try replace by existing
ea::string FormatFloat(float const value, int const precision = 2)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str().c_str();
}
} // namespace

DebugHUD::DebugHUD(Context* context)
    : LogicComponent(context)
{
    SetUpdateEventMask(USE_UPDATE);
    RADON_LOGDEBUG("DebugHUD: constructor called");
}

void DebugHUD::RegisterObject(Context* context)
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    context->RegisterFactory<DebugHUD>();
    URHO3D_ATTRIBUTE("Visible", bool, visible_, false, AM_DEFAULT);
}

void DebugHUD::Start()
{
    RADON_LOGDEBUG("DebugHUD: Start called");

    // Subscribe to events
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(DebugHUD, HandleKeyDown));
    SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(DebugHUD, HandleKeyUp));

    // Initialize history vectors
    fpsHistory_.reserve(MAX_HISTORY_SIZE);
    frameTimeHistory_.reserve(MAX_HISTORY_SIZE);
    consoleMessages_.reserve(MAX_CONSOLE_LINES);

    // Initialize stats
    fpsTimer_.Reset();

    AddLogMessage("Advanced Debug Console initialized", "INFO");
    RADON_LOGINFO("DebugHUD: Started successfully");
}

void DebugHUD::Update(float const timeStep)
{
    if (!visible_)
        return;

    frameTime_ = timeStep;
    UpdatePerformanceStats();
    UpdateRendererStats();
    UpdateSceneStats();
    UpdateSystemStats();

    RenderMainWindow();
}

void DebugHUD::HandleKeyDown(StringHash, VariantMap& eventData)
{
    int const key = eventData[KeyDown::P_KEY].GetInt();

    // F4 to toggle console visibility
    if (key == KEY_F4)
    {
        visible_ = !visible_;
        UpdateMouseMode();
        RADON_LOGINFO("DebugHUD: Toggled visibility: {}", visible_ ? "ON" : "OFF");
    }

    // Right ALT key to toggle mouse cursor visibility
    if (key == KEY_RALT)
    {
        cursorEnabled_ = !cursorEnabled_;
        UpdateMouseMode();
        RADON_LOGINFO("DebugHUD: Mouse cursor toggled: {}", cursorEnabled_ ? "ON" : "OFF");
    }
}

void DebugHUD::HandleKeyUp(StringHash, VariantMap& eventData)
{
    // No longer needed since we use toggle instead of hold
}

void DebugHUD::UpdateMouseMode() const
{
    auto* input = GetSubsystem<Input>();
    if (cursorEnabled_)
    {
        input->SetMouseMode(MM_FREE);
        input->SetMouseVisible(true);
    }
    else
    {
        input->SetMouseMode(MM_RELATIVE);
        input->SetMouseVisible(false);
    }
}

void DebugHUD::RenderMainWindow()
{
    if (!ImGui::Begin("Advanced Debug Console", &visible_, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tabs"))
        {
            ImGui::MenuItem("Performance", nullptr, &showPerformanceTab_);
            ImGui::MenuItem("Renderer", nullptr, &showRendererTab_);
            ImGui::MenuItem("Scene", nullptr, &showSceneTab_);
            ImGui::MenuItem("System", nullptr, &showSystemTab_);
            ImGui::MenuItem("Player", nullptr, &showPlayerTab_);
            ImGui::MenuItem("Console", nullptr, &showConsoleTab_);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::MenuItem("Auto-scroll Console", nullptr, &autoScrollConsole_);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Tabs
    if (ImGui::BeginTabBar("DebugTabs"))
    {
        if (showPerformanceTab_ && ImGui::BeginTabItem("Performance"))
        {
            RenderPerformanceTab();
            ImGui::EndTabItem();
        }

        if (showRendererTab_ && ImGui::BeginTabItem("Renderer"))
        {
            RenderRendererTab();
            ImGui::EndTabItem();
        }

        if (showSceneTab_ && ImGui::BeginTabItem("Scene"))
        {
            RenderSceneTab();
            ImGui::EndTabItem();
        }

        if (showSystemTab_ && ImGui::BeginTabItem("System"))
        {
            RenderSystemTab();
            ImGui::EndTabItem();
        }

        if (showPlayerTab_ && ImGui::BeginTabItem("Player"))
        {
            RenderPlayerTab();
            ImGui::EndTabItem();
        }

        if (showConsoleTab_ && ImGui::BeginTabItem("Console"))
        {
            RenderConsoleTab();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void DebugHUD::RenderPerformanceTab()
{
    // Performance metrics
    ImGui::Text("FPS: %.1f (Min: %.1f, Max: %.1f, Avg: %.1f)", currentFPS_, minFPS_, maxFPS_, avgFPS_);
    ImGui::Text("Frame Time: %.3f ms (Min: %.3f, Max: %.3f)", frameTime_ * 1000.0f, minFrameTime_ * 1000.0f, maxFrameTime_ * 1000.0f);

    ImGui::Separator();

    // FPS Graph with better formatting
    if (!fpsHistory_.empty())
    {
        ImGui::Text("FPS History (Last %d samples)", static_cast<int>(fpsHistory_.size()));

        // Calculate dynamic range for better visualization
        float const minFpsInHistory = *std::ranges::min_element(fpsHistory_);
        float const maxFpsInHistory = *std::ranges::max_element(fpsHistory_);
        float const range = maxFpsInHistory - minFpsInHistory;
        float const minScale = std::max(0.0f, minFpsInHistory - range * 0.1f);
        float const maxScale = maxFpsInHistory + range * 0.1f;

        // Create overlay text with current FPS
        ea::string const overlayText = "Current: " + ea::to_string(static_cast<int>(currentFPS_)) + " FPS";

        ImGui::PlotLines("##FPSGraph", fpsHistory_.data(), static_cast<int>(fpsHistory_.size()), 0, overlayText.c_str(), minScale, maxScale, ImVec2(-1, 120));

        // Add axis labels
        ImGui::Text("Time: %d sec ago <- -> Now", static_cast<int>(fpsHistory_.size()));
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);
        ImGui::Text("Range: %.0f-%.0f", minScale, maxScale);
    }

    ImGui::Spacing();

    // Frame Time Graph with better formatting
    if (!frameTimeHistory_.empty())
    {
        ImGui::Text("Frame Time History (Last %d samples)", static_cast<int>(frameTimeHistory_.size()));

        ea::vector<float> frameTimeMs;
        frameTimeMs.reserve(frameTimeHistory_.size());
        for (float const ft : frameTimeHistory_)
            frameTimeMs.push_back(ft * 1000.0f);

        // Calculate dynamic range
        float const minFrameTime = *std::ranges::min_element(frameTimeMs);
        float const maxFrameTime = *std::ranges::max_element(frameTimeMs);
        float const range = maxFrameTime - minFrameTime;
        float const minScale = std::max(0.0f, minFrameTime - range * 0.1f);
        float const maxScale = maxFrameTime + range * 0.1f;

        // Create overlay text
        ea::string const overlayText = "Current: " + FormatFloat(frameTime_ * 1000.0f, 2) + " ms";

        ImGui::PlotLines("##FrameTimeGraph", frameTimeMs.data(), static_cast<int>(frameTimeMs.size()), 0, overlayText.c_str(), minScale, maxScale, ImVec2(-1, 120));

        // Add axis labels
        ImGui::Text("Time: %d sec ago <- -> Now", static_cast<int>(frameTimeMs.size()));
        ImGui::SameLine(ImGui::GetWindowWidth() - 120);
        ImGui::Text("Range: %.1f-%.1f ms", minScale, maxScale);
    }

    ImGui::Separator();

    // Performance warnings based on sustained periods
    if (lowFpsTime_ >= WARNING_THRESHOLD_TIME)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "WARNING: FPS below 25 for %.1f seconds!", lowFpsTime_);
    }
    else if (currentFPS_ < 55.0f && currentFPS_ >= 25.0f)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "NOTICE: FPS below 55");
    }
    else if (currentFPS_ >= 55.0f)
    {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "GOOD: Performance is optimal");
    }

    if (highFrameTimeTime_ >= WARNING_THRESHOLD_TIME)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "WARNING: Frame time exceeds 55fps target for %.1f seconds!", highFrameTimeTime_);
    }
}

void DebugHUD::RenderRendererTab() const
{
    ImGui::Text("Geometries: %u", geometries_);
    ImGui::Text("Lights: %u", lights_);
    ImGui::Text("Shadow Maps: %u", shadowMaps_);
    ImGui::Text("Occluders: %u", occluders_);
    ImGui::Text("Views: %u", views_);

    ImGui::Separator();

    // Renderer settings
    if (auto* renderer = GetSubsystem<Renderer>())
    {
        ImGui::Text("Renderer Settings:");

        // Note: Some advanced renderer settings may not be available in current RBFX version
        ImGui::Text("Texture Quality: %d", static_cast<int>(renderer->GetTextureQuality()));

        int textureQuality = static_cast<int>(renderer->GetTextureQuality());
        if (ImGui::SliderInt("Texture Quality", &textureQuality, 0, 2))
            renderer->SetTextureQuality(static_cast<MaterialQuality>(textureQuality));

        ImGui::Text("Advanced settings not available in current RBFX API");
    }
}

void DebugHUD::RenderSceneTab()
{
    ImGui::Text("Total Nodes: %u", nodeCount_);
    ImGui::Text("Total Components: %u", componentCount_);
    ImGui::Text("Drawable Objects: %u", drawableCount_);

    ImGui::Separator();

    if (auto const* scene = GetScene())
    {
        ImGui::Text("Scene Name: %s", scene->GetName().c_str());
        ImGui::Text("Scene ID: %u", scene->GetID());

        if (ImGui::Button("Log Scene Info"))
        {
            AddLogMessage("=== SCENE INFO ===", "INFO");
            AddLogMessage("Scene: " + scene->GetName(), "INFO");
            AddLogMessage("Nodes: " + ea::to_string(nodeCount_), "INFO");
            AddLogMessage("Components: " + ea::to_string(componentCount_), "INFO");
        }
    }
}

void DebugHUD::RenderSystemTab() const
{
    ImGui::Text("GPU: %s", gpuName_.c_str());
    ImGui::Text("Resolution: %s", resolution_.c_str());
    ImGui::Text("VSync: %s", vsync_ ? "ON" : "OFF");
    ImGui::Text("Fullscreen: %s", fullscreen_ ? "YES" : "NO");

    ImGui::Separator();

    if (auto const* graphics = GetSubsystem<Graphics>())
    {
        ImGui::Text("Graphics Settings:");
        ImGui::Text("Multisample: %d", graphics->GetMultiSample());

        ImGui::Separator();
        ImGui::Text("Note: Real-time graphics settings modification");
        ImGui::Text("may not be available in current RBFX version");
    }
}

void DebugHUD::RenderPlayerTab()
{
    // Update player position/rotation with smoothing (every 0.1 seconds)
    float currentTime = static_cast<float>(fpsTimer_.GetMSec(false)) / 1000.0f;
    if (currentTime - lastPlayerUpdateTime_ >= 0.1f)
    {
        if (auto const* scene = GetScene())
        {
            if (auto const* playerNode = scene->GetChild("Player", true))
            {
                smoothedPlayerPosition_ = playerNode->GetWorldPosition();
                
                // Try to get camera rotation from player's camera child
                if (auto const* cameraNode = playerNode->GetChild("Camera", true))
                {
                    smoothedPlayerRotation_ = cameraNode->GetWorldRotation().EulerAngles();
                }
            }
        }
        lastPlayerUpdateTime_ = currentTime;
    }
    
    // Display smoothed values
    if (auto const* scene = GetScene())
    {
        if (auto const* playerNode = scene->GetChild("Player", true))
        {
            ImGui::Text("Position: %.2f, %.2f, %.2f", smoothedPlayerPosition_.x_, smoothedPlayerPosition_.y_, smoothedPlayerPosition_.z_);
            
            // Try to get camera rotation from player's camera child
            if (auto const* cameraNode = playerNode->GetChild("Camera", true))
            {
                ImGui::Text("Rotation: %.2f, %.2f, %.2f", smoothedPlayerRotation_.x_, smoothedPlayerRotation_.y_, smoothedPlayerRotation_.z_);
            }
            else
            {
                ImGui::Text("Rotation: Camera not found in player");
            }
        }
        else
        {
            ImGui::Text("Position: Player not found in scene");
            ImGui::Text("Rotation: Player not found in scene");
        }
    }
    else
    {
        ImGui::Text("Position: No scene loaded");
        ImGui::Text("Rotation: No scene loaded");
    }

    ImGui::Separator();

    ImGui::Text("Health: %.1f", playerHealth_);
    ImGui::ProgressBar(playerHealth_ / 100.0f, ImVec2(0, 0), (ea::to_string(static_cast<int>(playerHealth_)) + "/100").c_str());

    ImGui::Text("Stamina: %.1f", playerStamina_);
    ImGui::ProgressBar(playerStamina_ / 100.0f, ImVec2(0, 0), (ea::to_string(static_cast<int>(playerStamina_)) + "/100").c_str());

    ImGui::Separator();

    if (ImGui::Button("Heal Player"))
    {
        const_cast<DebugHUD*>(this)->playerHealth_ = 100.0f;
        const_cast<DebugHUD*>(this)->AddLogMessage("Player healed to full health", "INFO");
    }

    ImGui::SameLine();
    if (ImGui::Button("Restore Stamina"))
    {
        const_cast<DebugHUD*>(this)->playerStamina_ = 100.0f;
        const_cast<DebugHUD*>(this)->AddLogMessage("Player stamina restored", "INFO");
    }
}

void DebugHUD::RenderConsoleTab()
{
    // Console output
    ImGui::BeginChild("ConsoleOutput", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

    for (auto const& message : consoleMessages_)
    {
        ImGui::TextWrapped("%s", message.c_str());
    }

    if (autoScrollConsole_ && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();

    // Console input
    ImGui::Separator();
    if (ImGui::InputText("Command", consoleInput_, sizeof(consoleInput_), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        AddLogMessage(ea::string("> ") + consoleInput_, "CMD");

        // Process command (simplified)
        if (ea::string const cmd = consoleInput_; cmd == "clear")
        {
            consoleMessages_.clear();
        }
        else if (cmd == "fps")
        {
            AddLogMessage("Current FPS: " + ea::to_string(currentFPS_), "INFO");
        }
        else if (cmd == "help")
        {
            AddLogMessage("Available commands: clear, fps, help", "INFO");
        }
        else
        {
            AddLogMessage("Unknown command: " + cmd, "ERROR");
        }

        consoleInput_[0] = '\0';
        ImGui::SetKeyboardFocusHere(-1); // Return focus to input field
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear"))
    {
        consoleMessages_.clear();
    }
}

void DebugHUD::UpdatePerformanceStats()
{
    frameCount_++;

    // Update frame time stats
    if (frameTime_ > 0.0f)
    {
        minFrameTime_ = Min(minFrameTime_, frameTime_);
        maxFrameTime_ = Max(maxFrameTime_, frameTime_);

        frameTimeHistory_.push_back(frameTime_);
        if (frameTimeHistory_.size() > MAX_HISTORY_SIZE)
            frameTimeHistory_.erase(frameTimeHistory_.begin());

        // Track high frame time duration
        constexpr float targetFrameTime = 1.0f / 55.0f; // 55 FPS target in seconds
        if (frameTime_ > targetFrameTime)
        {
            highFrameTimeTime_ += frameTime_;
        }
        else
        {
            highFrameTimeTime_ = 0.0f; // Reset counter
        }
    }

    // Update FPS every second
    if (fpsTimer_.GetMSec(false) >= 1000)
    {
        currentFPS_ = static_cast<float>(frameCount_) / (static_cast<float>(fpsTimer_.GetMSec(true)) / 1000.0f);

        if (currentFPS_ > 0.0f)
        {
            minFPS_ = Min(minFPS_, currentFPS_);
            maxFPS_ = Max(maxFPS_, currentFPS_);
            fpsAccumulator_ += currentFPS_;
            fpsSamples_++;
            avgFPS_ = fpsAccumulator_ / static_cast<float>(fpsSamples_);

            fpsHistory_.push_back(currentFPS_);
            if (fpsHistory_.size() > MAX_HISTORY_SIZE)
                fpsHistory_.erase(fpsHistory_.begin());

            // Track low FPS duration
            if (currentFPS_ < 25.0f)
            {
                lowFpsTime_ += 1.0f; // Add 1 second
            }
            else
            {
                lowFpsTime_ = 0.0f; // Reset counter
            }
        }

        frameCount_ = 0;
        fpsTimer_.Reset();
    }
}

void DebugHUD::UpdateRendererStats()
{
    auto const* renderer = GetSubsystem<Renderer>();
    if (!renderer)
        return;

    geometries_ = renderer->GetNumGeometries();
    lights_ = renderer->GetNumLights();
    shadowMaps_ = renderer->GetNumShadowMaps();
    occluders_ = renderer->GetNumOccluders();
    views_ = renderer->GetNumViews();
}

void DebugHUD::UpdateSceneStats()
{
    auto const* scene = GetScene();
    if (!scene)
        return;

    // Count nodes recursively
    nodeCount_ = 0;
    componentCount_ = 0;
    drawableCount_ = 0;

    // Count all nodes and components recursively
    ea::vector<Node*> nodes;
    scene->GetChildren(nodes, true);
    nodeCount_ = nodes.size();

    for (auto const* node : nodes)
    {
        componentCount_ += node->GetNumComponents();
        if (node->GetComponent<Drawable>())
            drawableCount_++;
    }
}

void DebugHUD::UpdateSystemStats()
{
    auto const* graphics = GetSubsystem<Graphics>();
    if (!graphics)
        return;

    gpuName_ = graphics->GetApiName();
    resolution_ = ea::to_string(graphics->GetWidth()) + "x" + ea::to_string(graphics->GetHeight());
    vsync_ = graphics->GetVSync();
    fullscreen_ = graphics->GetFullscreen();
}

void DebugHUD::AddLogMessage(ea::string const& message, ea::string const& level)
{
    float const timeInSeconds = static_cast<float>(fpsTimer_.GetMSec(false)) / 1000.0f;
    ea::string const timestamp = "[" + ea::to_string(timeInSeconds).substr(0, 5) + "s]";
    ea::string const fullMessage = timestamp + " [" + level + "] " + message;

    consoleMessages_.push_back(fullMessage);

    // Keep console messages within limit
    if (consoleMessages_.size() > MAX_CONSOLE_LINES)
        consoleMessages_.erase(consoleMessages_.begin());
}
