#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>

namespace Radon::Engine::Graphics
{

struct CameraEffectResult
{
    Urho3D::Vector3 positionOffset{Urho3D::Vector3::ZERO};
    Urho3D::Quaternion rotationOffset{Urho3D::Quaternion::IDENTITY};
    float fovMultiplier{1.0f};

    CameraEffectResult& operator+=(CameraEffectResult const& other)
    {
        positionOffset += other.positionOffset;
        rotationOffset = rotationOffset * other.rotationOffset;
        fovMultiplier *= other.fovMultiplier;
        return *this;
    }
};

class ICameraEffect : public Urho3D::Object
{
    URHO3D_OBJECT(ICameraEffect, Object);

public:
    explicit ICameraEffect(Urho3D::Context* context)
        : Object(context) {}
    ~ICameraEffect() override = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual CameraEffectResult Update(float timeStep) = 0;

    virtual void SetEnabled(bool enabled) { enabled_ = enabled; }
    [[nodiscard]] virtual bool IsEnabled() const { return enabled_; }

    virtual void SetIntensity(float intensity) { intensity_ = Urho3D::Clamp(intensity, 0.0f, 1.0f); }
    [[nodiscard]] virtual float GetIntensity() const { return intensity_; }

protected:
    bool enabled_{true};
    float intensity_{1.0f};
};

} // namespace Radon::Engine::Graphics
