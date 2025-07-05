#include "PlayerEvents.hpp"

namespace Radon::Game::Events
{

// Movement events
Urho3D::StringHash const E_PLAYER_JUMPED("PlayerJumped");
Urho3D::StringHash const E_PLAYER_GROUNDED("PlayerGrounded");
Urho3D::StringHash const E_PLAYER_STARTED_MOVING("PlayerStartedMoving");
Urho3D::StringHash const E_PLAYER_STOPPED_MOVING("PlayerStoppedMoving");
Urho3D::StringHash const E_PLAYER_RUN_STATE_CHANGED("PlayerRunStateChanged");

// Camera events
Urho3D::StringHash const E_PLAYER_HEADBOB("PlayerHeadbob");
Urho3D::StringHash const E_PLAYER_CAMERA_DIRECTION_CHANGED("PlayerCameraDirectionChanged"); // TODO: Проверить какие ивенты можно унести из плагина

// Health events
Urho3D::StringHash const E_PLAYER_DAMAGED("PlayerDamaged");
Urho3D::StringHash const E_PLAYER_HEALED("PlayerHealed");
Urho3D::StringHash const E_PLAYER_DIED("PlayerDied");
Urho3D::StringHash const E_PLAYER_RESPAWNED("PlayerRespawned");

// Input events
Urho3D::StringHash const E_PLAYER_INTERACTED("PlayerInteracted");

} // namespace Radon::Game::Events
