#include "PlayerManager.hpp"

#include "Components/PlayerCamera.hpp"
#include "Components/PlayerHealth.hpp"
#include "Components/PlayerInputHandler.hpp"
#include "Components/PlayerMovement.hpp"

namespace Radon::Game::Player
{

void PlayerManager::RegisterComponents(Urho3D::Context* context)
{
    PlayerHealth::RegisterObject(context);
    PlayerInputHandler::RegisterObject(context);
    PlayerMovement::RegisterObject(context);
    PlayerCamera::RegisterObject(context);
}

} // namespace Radon::Game::Player
