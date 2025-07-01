#pragma once

#include <Urho3D/Math/StringHash.h>

namespace Radon::Game::Events
{

// Movement events
extern Urho3D::StringHash const E_PLAYER_JUMPED;
extern Urho3D::StringHash const E_PLAYER_GROUNDED;
extern Urho3D::StringHash const E_PLAYER_STARTED_MOVING;
extern Urho3D::StringHash const E_PLAYER_STOPPED_MOVING;
extern Urho3D::StringHash const E_PLAYER_RUN_STATE_CHANGED;

// Camera events
extern Urho3D::StringHash const E_PLAYER_HEADBOB;
extern Urho3D::StringHash const E_PLAYER_CAMERA_DIRECTION_CHANGED;

// Health events
extern Urho3D::StringHash const E_PLAYER_DAMAGED;
extern Urho3D::StringHash const E_PLAYER_HEALED;
extern Urho3D::StringHash const E_PLAYER_DIED;
extern Urho3D::StringHash const E_PLAYER_RESPAWNED;

// Input events
extern Urho3D::StringHash const E_PLAYER_INTERACTED;

// Event parameter names
namespace P
{
// Movement params
inline auto DIRECTION = "Direction";
inline auto SPEED = "Speed";
inline auto IS_RUNNING = "IsRunning";
inline auto JUMP_HEIGHT = "JumpHeight";

// Camera params
inline auto FORWARD = "Forward";
inline auto RIGHT = "Right";
inline auto PITCH = "Pitch";
inline auto YAW = "Yaw";

// Health params
inline auto HEALTH = "Health";
inline auto PREVIOUS_HEALTH = "PreviousHealth";
inline auto AMOUNT = "Amount";
inline auto SOURCE = "Source";
} // namespace P

} // namespace Radon::Game::Events
