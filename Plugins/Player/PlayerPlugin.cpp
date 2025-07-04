#include "PlayerPlugin.hpp"

#include "Components/PlayerCamera.hpp"
#include "Components/PlayerHealth.hpp"
#include "Components/PlayerInputHandler.hpp"
#include "Components/PlayerMovement.hpp"

#include <Urho3D/Plugins/PluginApplication.h>

using namespace Urho3D;
using namespace Radon::Game::Plugins;

namespace
{

void RegisterPluginObjects(PluginApplication& plugin)
{
    plugin.RegisterObject<PlayerHealth>();
    plugin.RegisterObject<PlayerInputHandler>();
    plugin.RegisterObject<PlayerMovement>();
    plugin.RegisterObject<PlayerCamera>();
}

void UnregisterPluginObjects(PluginApplication& plugin)
{
}

} // namespace

URHO3D_DEFINE_PLUGIN_MAIN_SIMPLE(RegisterPluginObjects, UnregisterPluginObjects);
