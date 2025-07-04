#pragma once

#include <Urho3D/Input/InputConstants.h>
#include <Urho3D/Scene/LogicComponent.h>

namespace Radon::Engine::Input
{
class InputHandler;
}

namespace Radon::Game::Plugins
{

class PlayerInputHandler final : public Urho3D::LogicComponent
{
    URHO3D_OBJECT(PlayerInputHandler, LogicComponent);

public:
    explicit PlayerInputHandler(Urho3D::Context* context);
    ~PlayerInputHandler() override;

    static void RegisterObject(Urho3D::Context* context);

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

    float GetMouseSensitivityX() const { return mouseSensitivityX_; }
    float GetMouseSensitivityY() const { return mouseSensitivityY_; }
    void SetMouseSensitivityX(float v) { mouseSensitivityX_ = v; }
    void SetMouseSensitivityY(float v) { mouseSensitivityY_ = v; }

    float GetMouseSmoothing() const { return mouseSmoothing_; }
    void SetMouseSmoothing(float v) { mouseSmoothing_ = v; }

    bool GetInvertMouseY() const { return invertMouseY_; }
    void SetInvertMouseY(bool v) { invertMouseY_ = v; }

    float GetMouseDeadzone() const { return mouseDeadzone_; }
    void SetMouseDeadzone(float v) { mouseDeadzone_ = v; }

    void SetForwardKey(Urho3D::Key k) { forwardKey_ = k; }
    void SetBackKey(Urho3D::Key k) { backKey_ = k; }
    void SetLeftKey(Urho3D::Key k) { leftKey_ = k; }
    void SetRightKey(Urho3D::Key k) { rightKey_ = k; }
    void SetJumpKey(Urho3D::Key k) { jumpKey_ = k; }
    void SetRunKey(Urho3D::Key k) { runKey_ = k; }
    void SetInteractKey(Urho3D::Key k) { interactKey_ = k; }
    Urho3D::Key GetForwardKey() const { return forwardKey_; }
    Urho3D::Key GetBackKey() const { return backKey_; }
    Urho3D::Key GetLeftKey() const { return leftKey_; }
    Urho3D::Key GetRightKey() const { return rightKey_; }
    Urho3D::Key GetJumpKey() const { return jumpKey_; }
    Urho3D::Key GetRunKey() const { return runKey_; }
    Urho3D::Key GetInteractKey() const { return interactKey_; }

private:
    // TODO Вынести в отдельный класс "Бинды"
    float mouseSensitivityX_{0.1f};
    float mouseSensitivityY_{0.1f};
    float mouseSmoothing_{0.5f};
    bool invertMouseY_{false};
    float mouseDeadzone_{0.01f};

    Urho3D::Key forwardKey_{Urho3D::KEY_W};
    Urho3D::Key backKey_{Urho3D::KEY_S};
    Urho3D::Key leftKey_{Urho3D::KEY_A};
    Urho3D::Key rightKey_{Urho3D::KEY_D};
    Urho3D::Key jumpKey_{Urho3D::KEY_SPACE};
    Urho3D::Key runKey_{Urho3D::KEY_SHIFT};
    Urho3D::Key interactKey_{Urho3D::KEY_E};

    float mouseYaw_{0.0f};
    float mousePitch_{0.0f};

    bool moveForward_{false};
    bool moveBack_{false};
    bool moveLeft_{false};
    bool moveRight_{false};
    bool jump_{false};
    bool run_{false};
    bool interact_{false};
};

} // namespace Radon::Game::Plugins
