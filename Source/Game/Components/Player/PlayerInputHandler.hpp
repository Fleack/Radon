#pragma once

#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Engine::Input
{
class InputHandler;
}

namespace Radon::Game::Components
{

class PlayerInputHandler final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerInputHandler, LogicComponent);

public:
    explicit PlayerInputHandler(Urho3D::Context* context);
    ~PlayerInputHandler() override;

    static void RegisterObject(Urho3D::Context* context);

    void Start() override;
    void Update(float timeStep) override;

    [[nodiscard]] bool GetMoveForward() const { return moveForward_; }
    [[nodiscard]] bool GetMoveBack() const { return moveBack_; }
    [[nodiscard]] bool GetMoveLeft() const { return moveLeft_; }
    [[nodiscard]] bool GetMoveRight() const { return moveRight_; }
    [[nodiscard]] bool GetJump() const { return jump_; }
    [[nodiscard]] bool GetRun() const { return run_; }
    [[nodiscard]] bool GetInteract() const { return interact_; }
    [[nodiscard]] float GetMouseYaw() const { return mouseYaw_; }
    [[nodiscard]] float GetMousePitch() const { return mousePitch_; }

private:
    Radon::Engine::Input::InputHandler* inputHandler_{nullptr};

    bool moveForward_{false};
    bool moveBack_{false};
    bool moveLeft_{false};
    bool moveRight_{false};
    bool jump_{false};
    bool run_{false};
    bool interact_{false};
    float mouseYaw_{0.0f};
    float mousePitch_{0.0f};
    float mouseSensitivity_{0.1f};
    bool initialized_{false};
};

} // namespace Radon::Game::Components
