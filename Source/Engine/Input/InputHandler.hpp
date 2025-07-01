#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Input/Input.h>

namespace Radon::Engine::Input
{

class InputHandler final : public Urho3D::Object
{
    URHO3D_OBJECT(InputHandler, Object);

public:
    explicit InputHandler(Urho3D::Context* context);
    ~InputHandler() override;

    void Initialize();
    void Shutdown();

    [[nodiscard]] bool GetForwardMove() const { return forwardMove_; }
    [[nodiscard]] bool GetBackMove() const { return backMove_; }
    [[nodiscard]] bool GetLeftMove() const { return leftMove_; }
    [[nodiscard]] bool GetRightMove() const { return rightMove_; }
    [[nodiscard]] bool GetJumpKey() const { return jump_; }
    [[nodiscard]] bool GetRunKey() const { return run_; }
    [[nodiscard]] float GetMouseDeltaX() const { return mouseDeltaX_; }
    [[nodiscard]] float GetMouseDeltaY() const { return mouseDeltaY_; }
    [[nodiscard]] bool GetInteract() { return false; } // TODO: Real interaction

    void SetForwardKey(Urho3D::Key key) { forwardKey_ = key; }
    void SetBackKey(Urho3D::Key key) { backKey_ = key; }
    void SetLeftKey(Urho3D::Key key) { leftKey_ = key; }
    void SetRightKey(Urho3D::Key key) { rightKey_ = key; }
    void SetJumpKey(Urho3D::Key key) { jumpKey_ = key; }
    void SetRunKey(Urho3D::Key key) { runKey_ = key; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    bool forwardMove_{false};
    bool backMove_{false};
    bool leftMove_{false};
    bool rightMove_{false};
    bool jump_{false};
    bool run_{false};
    float mouseDeltaX_{0.0f};
    float mouseDeltaY_{0.0f};

    Urho3D::Key forwardKey_{Urho3D::KEY_W};
    Urho3D::Key backKey_{Urho3D::KEY_S};
    Urho3D::Key leftKey_{Urho3D::KEY_A};
    Urho3D::Key rightKey_{Urho3D::KEY_D};
    Urho3D::Key jumpKey_{Urho3D::KEY_SPACE};
    Urho3D::Key runKey_{Urho3D::KEY_SHIFT};

    bool initialized_{false};
};

} // namespace Radon::Engine::Input
