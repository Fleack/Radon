#include "RadonApplication.hpp"

#include "App/Config/ConfigManager.hpp"
#include "App/Graphics/ViewportManager.hpp"
#include "App/Input/CameraController.hpp"
#include "App/Logger/Logger.hpp"
#include "App/Scene/SceneManager.hpp"
#include "App/States/GameStateManager.hpp"
#include "App/States/MenuState.hpp"
#include "App/UI/UIManager.hpp"

#include <Urho3D/Engine/EngineDefs.h>

using namespace Radon;

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
#ifdef RADON_DEBUG
    engineParameters_[Urho3D::EP_LOG_NAME] = "Radon.log";
#else
    engineParameters_[Urho3D::EP_LOG_NAME] = "conf://Radon.log";
#endif
}

void RadonApplication::Start()
{
    RADON_LOGINFO("RadonApplication: Start called");
    RegisterSubsystems();
    GetSubsystem<States::GameStateManager>()->PushState(MakeShared<States::MenuState>(context_));
}

void RadonApplication::Stop()
{
    RADON_LOGINFO("RadonApplication: Stop called");
}

void RadonApplication::RegisterSubsystems()
{
    RADON_LOGDEBUG("RadonApplication: RegisterSubsystems called");
    context_->RegisterSubsystem(MakeShared<Scene::SceneManager>(context_));
    context_->RegisterSubsystem(MakeShared<States::GameStateManager>(context_));
    context_->RegisterSubsystem(MakeShared<UI::UIManager>(context_));
    context_->RegisterSubsystem(MakeShared<Input::CameraController>(context_));
    context_->RegisterSubsystem(MakeShared<Graphics::ViewportManager>(context_));
    RADON_LOGINFO("RadonApplication: All subsystems registered");
}
