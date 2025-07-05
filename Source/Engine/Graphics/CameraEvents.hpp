#pragma once

#include <Urho3D/Math/StringHash.h>

namespace Radon::Engine::Graphics::Events
{

/// Camera effects
extern Urho3D::StringHash const E_CAMERA_EFFECT_APPLIED;
extern Urho3D::StringHash const E_CAMERA_SHAKE_REQUESTED;
extern Urho3D::StringHash const E_CAMERA_RECOIL_REQUESTED;

/// Camera state
extern Urho3D::StringHash const E_CAMERA_BASE_TRANSFORM_CHANGED;
extern Urho3D::StringHash const E_CAMERA_FOV_CHANGED;

namespace P
{
// Effects
inline auto EFFECT_TYPE = "EffectType";
inline auto INTENSITY = "Intensity";
inline auto DURATION = "Duration";
inline auto POSITION_OFFSET = "PositionOffset";
inline auto ROTATION_OFFSET = "RotationOffset";
inline auto FOV_MULTIPLIER = "FovMultiplier";

// Shake
inline auto SHAKE_STRENGTH = "ShakeStrength";
inline auto SHAKE_FREQUENCY = "ShakeFrequency";

// Recoil
inline auto RECOIL_PITCH = "RecoilPitch";
inline auto RECOIL_YAW = "RecoilYaw";
inline auto RECOIL_RECOVERY_TIME = "RecoilRecoveryTime";

// Transform
inline auto BASE_POSITION = "BasePosition";
inline auto BASE_ROTATION = "BaseRotation";
inline auto NEW_FOV = "NewFov";
inline auto OLD_FOV = "OldFov";

// Direction
inline auto FORWARD = "Forward";
inline auto RIGHT = "Right";
inline auto PITCH = "Pitch";
inline auto YAW = "Yaw";
} // namespace P

} // namespace Radon::Engine::Graphics::Events
