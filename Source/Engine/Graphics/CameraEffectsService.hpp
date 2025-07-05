#pragma once

#include "Engine/Graphics/CameraEffectsManager.hpp"

#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Node.h>

// Forward declarations
namespace Radon::Engine::Graphics
{
class HeadBobEffect;
}

namespace Radon::Engine::Graphics
{

/// Сервис для интеграции системы эффектов движка с компонентами камеры
class CameraEffectsService final : public Urho3D::Object
{
    URHO3D_OBJECT(CameraEffectsService, Object);

public:
    explicit CameraEffectsService(Urho3D::Context* context);
    ~CameraEffectsService() override;

    /// Инициализация сервиса
    void Initialize();

    /// Завершение работы
    void Shutdown();

    /// Получить менеджер эффектов
    [[nodiscard]] CameraEffectsManager* GetEffectsManager() const { return effectsManager_.Get(); }

    /// Получить HeadBob эффект для настройки
    template<typename T>
    Urho3D::WeakPtr<T> GetEffect() const { return effectsManager_ ? effectsManager_->GetEffect<T>() : nullptr; }

    /// Начать наблюдение за камерой игрока
    void StartObservingPlayerCamera();

    /// Прекратить наблюдение за камерой игрока
    void StopObservingPlayerCamera();

    /// Включить/выключить интеграцию с внешними камерами
    void SetIntegrationEnabled(bool enabled);
    [[nodiscard]] bool IsIntegrationEnabled() const { return integrationEnabled_; }

    /// Применить эффекты к камере (публичный метод для внешнего использования)
    void ApplyEffectsToCamera(Urho3D::Node* cameraNode, Urho3D::Camera* camera, const Urho3D::Vector3& basePosition, float timeStep);

private:
    void HandlePlayerCameraEvent(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::SharedPtr<CameraEffectsManager> effectsManager_;

    Urho3D::WeakPtr<Urho3D::Node> observedCameraNode_;
    Urho3D::WeakPtr<Urho3D::Camera> observedCamera_;
    Urho3D::Vector3 cachedBasePosition_{Urho3D::Vector3::ZERO};

    bool integrationEnabled_{false};
    bool observingPlayerCamera_{false};
    bool initialized_{false};
};

} // namespace Radon::Engine::Graphics
