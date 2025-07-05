#include "RadonApplication.hpp"

#include "Engine/Config/ConfigManager.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/ViewportManager.hpp"

#include "Engine/Input/InputHandler.hpp"
#include "Engine/Input/PlayerCameraService.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/GameStateManager.hpp"
#include "Engine/UI/DebugHUD.hpp"
#include "Engine/UI/UIManager.hpp"
#include "Game/States/MenuState.hpp"
#include "Player/PlayerPlugin.hpp"
#include "Urho3D/Plugins/PluginManager.h"

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Plugins/PluginApplication.h>
#include <Urho3D/SystemUI/DebugHud.h>

using namespace Radon::App;

RadonApplication::RadonApplication(Urho3D::Context* context)
    : Application(context)
{
    RADON_LOGDEBUG("RadonApplication: constructor called");
}

void RadonApplication::Setup()
{
    engineParameters_[Urho3D::EP_APPLICATION_NAME] = "Radon";
    engineParameters_[Urho3D::EP_LOG_LEVEL] = Urho3D::LOG_DEBUG;
    engineParameters_[Urho3D::EP_LOG_QUIET] = false;
    engineParameters_[Urho3D::EP_LOG_NAME] = "Radon.log";
    engine_->SetTimeStepSmoothing(5);
    engine_->SetPauseMinimized(false);
    engine_->SetMaxInactiveFps(144);
}

void RadonApplication::Start()
{
    RADON_LOGINFO("RadonApplication: Start called");
    LoadPlugins();
    RegisterComponents();
    RegisterSubsystems();

    // Initialize PlayerCameraService
    GetSubsystem<Engine::Input::PlayerCameraService>()->Initialize();

    GetSubsystem<Engine::StateMachine::GameStateManager>()->PushState(MakeShared<Game::States::MenuState>(context_));
}

void RadonApplication::Stop()
{
    RADON_LOGINFO("RadonApplication: Stop called");
}

void RadonApplication::LoadPlugins()
{
    RADON_LOGDEBUG("RadonApplication: LoadPlugins called");

    auto* pluginManager = context_->GetSubsystem<Urho3D::PluginManager>();
    if (!pluginManager)
    {
        RADON_LOGERROR("RadonApplication: PluginManager not found");
        return;
    }

    // Plugins loading
    Urho3D::StringVector const plugins = {
        Game::Plugins::PlayerPlugin::GetStaticPluginName()};
    pluginManager->SetPluginsLoaded(plugins);

    RADON_LOGINFO("RadonApplication: Plugins loaded successfully");
}

void RadonApplication::RegisterComponents()
{
    RADON_LOGDEBUG("RadonApplication: RegisterComponents called");
    Engine::UI::DebugHUD::RegisterObject(context_);
    RADON_LOGINFO("RadonApplication: Components registered successfully");
}

void RadonApplication::RegisterSubsystems()
{
    using namespace Engine;

    RADON_LOGDEBUG("RadonApplication: RegisterSubsystems called");
    context_->RegisterSubsystem(MakeShared<StateMachine::GameStateManager>(context_));
    context_->RegisterSubsystem(MakeShared<Scene::SceneManager>(context_));
    context_->RegisterSubsystem(MakeShared<Graphics::ViewportManager>(context_));
    context_->RegisterSubsystem(MakeShared<Input::PlayerCameraService>(context_));
    context_->RegisterSubsystem(MakeShared<Input::InputHandler>(context_));
    context_->RegisterSubsystem(MakeShared<UI::UIManager>(context_));
    RADON_LOGINFO("RadonApplication: All subsystems registered");
}
