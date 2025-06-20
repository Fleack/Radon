#include "GameApplication.hpp"

#include "States/MenuState.hpp"
#include "Urho3D/Core/CoreEvents.h"

#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/IO/Log.h>

using namespace Radon;

void GameApplication::Setup()
{
    // Window settings
    engineParameters_[EP_WINDOW_TITLE] = "Radon";
    engineParameters_[EP_WINDOW_WIDTH] = 1024;
    engineParameters_[EP_WINDOW_HEIGHT] = 768;
    engineParameters_[EP_FULL_SCREEN] = false;
    engineParameters_[EP_WINDOW_RESIZABLE] = true;
    engineParameters_[EP_VSYNC] = true;

    // Logs settings
    engineParameters_[EP_LOG_QUIET] = false;
    engineParameters_[EP_LOG_LEVEL] = LOG_DEBUG;
    engineParameters_[EP_LOG_NAME] = "Radon.log";
}

void GameApplication::Start()
{
    itemRegistry_.LoadAll(context_, "Items");

    stateManager_ = MakeShared<GameStateManager>(context_);
    stateManager_->PushState<MenuState>();
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(GameApplication, HandleUpdate));
}

void GameApplication::HandleUpdate(StringHash, VariantMap& eventData) const
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    if (stateManager_)
        stateManager_->Update(timeStep);
}

void GameApplication::Stop()
{
    URHO3D_LOGINFO("Radon shutting down");
}
