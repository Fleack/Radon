#include "RadonApplication.hpp"

#include "Engine/Config/ConfigManager.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/CameraEffectsService.hpp"
#include "Engine/Graphics/ViewportManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/AppStateManager.hpp"
#include "Engine/UI/DebugHUD.hpp"
#include "Engine/UI/UIManager.hpp"
#include "Game/Player/PlayerManager.hpp"
#include "Game/States/MenuState.hpp"

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
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
    RegisterComponents();
    RegisterSubsystems();

    GetSubsystem<Engine::StateMachine::AppStateManager>()->PushState(MakeShared<Game::States::MenuState>(context_));
}

void RadonApplication::Stop()
{
    RADON_LOGINFO("RadonApplication: Stop called");
}

void RadonApplication::RegisterComponents()
{
    RADON_LOGDEBUG("RadonApplication: RegisterComponents called");
    Engine::UI::DebugHUD::RegisterObject(context_);
    Game::Player::PlayerManager::RegisterComponents(context_);
    RADON_LOGINFO("RadonApplication: Components registered successfully");
}

void RadonApplication::RegisterSubsystems()
{
    using namespace Engine;

    RADON_LOGINFO("RadonApplication: Registering subsystems");

    context_->RegisterSubsystem(MakeShared<Scene::SceneManager>(context_));
    context_->RegisterSubsystem(MakeShared<UI::UIManager>(context_));
    context_->RegisterSubsystem(MakeShared<StateMachine::AppStateManager>(context_));
    context_->RegisterSubsystem(MakeShared<Graphics::CameraEffectsService>(context_));
    context_->RegisterSubsystem(MakeShared<Graphics::ViewportManager>(context_));

    RADON_LOGINFO("RadonApplication: All subsystems registered");
}
