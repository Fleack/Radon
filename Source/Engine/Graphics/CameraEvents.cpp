#include "CameraEvents.hpp"

namespace Radon::Engine::Graphics::Events
{

/// Camera effects
Urho3D::StringHash const E_CAMERA_EFFECT_APPLIED{"CameraEffectApplied"};
Urho3D::StringHash const E_CAMERA_SHAKE_REQUESTED{"CameraShakeRequested"};
Urho3D::StringHash const E_CAMERA_RECOIL_REQUESTED{"CameraRecoilRequested"};

/// Camera states
Urho3D::StringHash const E_CAMERA_BASE_TRANSFORM_CHANGED{"CameraBaseTransformChanged"};
Urho3D::StringHash const E_CAMERA_FOV_CHANGED{"CameraFovChanged"};

} // namespace Radon::Engine::Graphics::Events 