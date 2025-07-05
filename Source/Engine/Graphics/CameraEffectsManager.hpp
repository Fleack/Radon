#pragma once

#include "Engine/Graphics/ICameraEffect.hpp"

#include <Urho3D/Core/Object.h>

namespace Radon::Engine::Graphics
{

class CameraEffectsManager final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraEffectsManager, Object);

public:
    explicit CameraEffectsManager(Urho3D::Context* context);
    ~CameraEffectsManager() override;

    void Initialize();
    void Shutdown();

    CameraEffectResult Update(float timeStep);

    void AddEffect(Urho3D::SharedPtr<ICameraEffect> effect);
    void RemoveEffect(ICameraEffect const* effect);

    void ClearEffects();
    void SetAllEffectsEnabled(bool enabled);

    /// All effects intensity
    void SetGlobalIntensity(float intensity);
    [[nodiscard]] float GetGlobalIntensity() const { return globalIntensity_; }

    template <typename T>
    Urho3D::WeakPtr<T> GetEffect() const
    {
        for (auto& effect : effects_)
        {
            if (auto* typedEffect = dynamic_cast<T*>(effect.Get()))
                return typedEffect;
        }
        return nullptr;
    }

    [[nodiscard]] ea::vector<Urho3D::SharedPtr<ICameraEffect>> const& GetEffects() const { return effects_; }

private:
    ea::vector<Urho3D::SharedPtr<ICameraEffect>> effects_;
    float globalIntensity_{1.0f};
    bool initialized_{false};
};

} // namespace Radon::Engine::Graphics
