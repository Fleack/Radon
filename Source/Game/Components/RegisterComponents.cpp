#include "Game/Components/Player/PlayerCameraBinding.hpp"
#include "Game/Components/Player/PlayerComponent.hpp"
#include "Game/Components/Player/PlayerHealth.hpp"
#include "Game/Components/Player/PlayerInputHandler.hpp"
#include "Game/Components/Player/PlayerMovement.hpp"

#include <Urho3D/Core/Context.h>

namespace Radon::Game::Components
{

void RegisterPlayerComponents(Urho3D::Context* context)
{
    PlayerComponent::RegisterObject(context);
    PlayerInputHandler::RegisterObject(context);
    PlayerMovement::RegisterObject(context);
    PlayerHealth::RegisterObject(context);
    PlayerCameraBinding::RegisterObject(context);
}

} // namespace Radon::Game::Components
