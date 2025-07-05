#pragma once

#include <Urho3D/Core/Context.h>

namespace Radon::Game::Player
{

/// Player system manager responsible for registering all player components
class PlayerManager final
{
public:
    static void RegisterComponents(Urho3D::Context* context);
};

} // namespace Radon::Game::Player 