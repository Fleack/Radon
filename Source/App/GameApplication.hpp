#pragma once

#include "App/States/GameStateManager.hpp"
#include "Data/ItemData.hpp"
#include "Data/Registry.hpp"

#include <Urho3D/Engine/Application.h>

using namespace Urho3D;

namespace Radon
{
class GameApplication final : public Application
{
    URHO3D_OBJECT(GameApplication, Application);

public:
    using Application::Application;

    void Setup() override;
    void Start() override;
    void Stop() override;

    GameStateManager* GetStateManager() const { return stateManager_.Get(); }

private:
    void HandleUpdate(StringHash eventType, VariantMap& eventData) const;

private:
    Registry<ItemData> itemRegistry_;
    SharedPtr<GameStateManager> stateManager_;
};
} // namespace Radon
