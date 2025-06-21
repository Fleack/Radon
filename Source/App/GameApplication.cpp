#include "GameApplication.hpp"

#include "States/MenuState.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/IO/Log.h>

using namespace Radon;

void GameApplication::Setup()
{
    URHO3D_LOGINFO("Radon running");
}

void GameApplication::Start()
{
    InitGameModules();

    stateManager_->PushState<MenuState>();
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(GameApplication, HandleUpdate));
}

void GameApplication::Stop()
{
    UnsubscribeFromAllEvents();
    URHO3D_LOGINFO("Radon shutting down");
}

void GameApplication::InitGameModules()
{
    itemRegistry_ = MakeShared<Registry<ItemData>>(context_);
    context_->RegisterSubsystem(itemRegistry_);
    itemRegistry_->LoadAll("Items");

    stateManager_ = MakeShared<GameStateManager>(context_);
    context_->RegisterSubsystem(stateManager_);
}

void GameApplication::HandleUpdate(StringHash, VariantMap& eventData) const
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    if (stateManager_)
        stateManager_->Update(timeStep);
}
