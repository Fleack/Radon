#pragma once

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

private:
    Registry<ItemData> itemRegistry_;
};
} // namespace Radon
