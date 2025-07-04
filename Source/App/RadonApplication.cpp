#include "RadonApplication.hpp"

#include "Engine/Config/ConfigManager.hpp"
#include "Engine/Core/Logger.hpp"
#include "Engine/Graphics/ViewportManager.hpp"
#include "Engine/Input/CameraManager.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Engine/StateMachine/GameStateManager.hpp"
#include "Engine/UI/UIManager.hpp"
#include "Game/States/MenuState.hpp"

#include <Urho3D/Engine/EngineDefs.h>

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
}

void RadonApplication::Start()
{
    RADON_LOGINFO("RadonApplication: Start called");
    RegisterSubsystems();
    GetSubsystem<Engine::StateMachine::GameStateManager>()->PushState(MakeShared<Game::States::MenuState>(context_));
}

void RadonApplication::Stop()
{
    RADON_LOGINFO("RadonApplication: Stop called");
}

void RadonApplication::RegisterSubsystems()
{
    using namespace Engine;

    RADON_LOGDEBUG("RadonApplication: RegisterSubsystems called");
    context_->RegisterSubsystem(MakeShared<Scene::SceneManager>(context_));
    context_->RegisterSubsystem(MakeShared<StateMachine::GameStateManager>(context_));
    context_->RegisterSubsystem(MakeShared<UI::UIManager>(context_));
    context_->RegisterSubsystem(MakeShared<Input::CameraManager>(context_));
    context_->RegisterSubsystem(MakeShared<Graphics::ViewportManager>(context_));
    RADON_LOGINFO("RadonApplication: All subsystems registered");
}
