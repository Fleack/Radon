#pragma once

#include "Urho3D/Plugins/PluginApplication.h"

namespace Radon::Game::Plugins
{

class PlayerPlugin final : public Urho3D::PluginApplication
{
    URHO3D_OBJECT(PlayerPlugin, PluginApplication);
    URHO3D_MANUAL_PLUGIN("Plugin.Radon.Player");

public:
    explicit PlayerPlugin(Urho3D::Context* context);

    void Load() override;
    void Unload() override;
};

} // namespace Radon::Game::Plugins
