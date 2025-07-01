#pragma once

#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Game::Components
{

class PlayerInputHandler;
class PlayerMovement;
class PlayerHealth;
class PlayerCameraBinding;

class PlayerComponent final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerComponent, LogicComponent);

public:
    explicit PlayerComponent(Urho3D::Context* context);
    ~PlayerComponent() override;

    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void DelayedStart() override;
    void Update(float timeStep) override;

    [[nodiscard]] PlayerInputHandler* GetInputHandler() const { return inputHandler_; }
    [[nodiscard]] PlayerMovement* GetMovement() const { return movement_; }
    [[nodiscard]] PlayerHealth* GetHealth() const { return health_; }
    [[nodiscard]] PlayerCameraBinding* GetCameraBinding() const { return cameraBinding_; }

    void SetInputHandler(PlayerInputHandler* handler);
    void SetMovement(PlayerMovement* movement);
    void SetHealth(PlayerHealth* health);
    void SetCameraBinding(PlayerCameraBinding* cameraBinding);

    static Urho3D::StringHash const EVENT_PLAYER_DAMAGED;
    static Urho3D::StringHash const EVENT_PLAYER_HEALED;
    static Urho3D::StringHash const EVENT_PLAYER_DIED;
    static Urho3D::StringHash const EVENT_PLAYER_JUMPED;
    static Urho3D::StringHash const EVENT_PLAYER_STARTED_MOVING;
    static Urho3D::StringHash const EVENT_PLAYER_STOPPED_MOVING;
    static Urho3D::StringHash const EVENT_PLAYER_INTERACTED;

private:
    void SubscribeToComponentEvents();

private:
    PlayerInputHandler* inputHandler_{nullptr};
    PlayerMovement* movement_{nullptr};
    PlayerHealth* health_{nullptr};
    PlayerCameraBinding* cameraBinding_{nullptr};

    bool initialized_{false};
};

} // namespace Radon::Game::Components
