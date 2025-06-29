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

    bool GetForwardMove() const { return forwardMove_; }
    bool GetBackMove() const { return backMove_; }
    bool GetLeftMove() const { return leftMove_; }
    bool GetRightMove() const { return rightMove_; }
    bool GetUpMove() const { return upMove_; }
    bool GetDownMove() const { return downMove_; }
    float GetMouseDeltaX() const { return mouseDeltaX_; }
    float GetMouseDeltaY() const { return mouseDeltaY_; }

    void SetForwardKey(Urho3D::Key key) { forwardKey_ = key; }
    void SetBackKey(Urho3D::Key key) { backKey_ = key; }
    void SetLeftKey(Urho3D::Key key) { leftKey_ = key; }
    void SetRightKey(Urho3D::Key key) { rightKey_ = key; }
    void SetUpKey(Urho3D::Key key) { upKey_ = key; }
    void SetDownKey(Urho3D::Key key) { downKey_ = key; }

private:
    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    bool forwardMove_{false};
    bool backMove_{false};
    bool leftMove_{false};
    bool rightMove_{false};
    bool upMove_{false};
    bool downMove_{false};
    float mouseDeltaX_{0.0f};
    float mouseDeltaY_{0.0f};

    Urho3D::Key forwardKey_{Urho3D::KEY_W};
    Urho3D::Key backKey_{Urho3D::KEY_S};
    Urho3D::Key leftKey_{Urho3D::KEY_A};
    Urho3D::Key rightKey_{Urho3D::KEY_D};
    Urho3D::Key upKey_{Urho3D::KEY_SPACE};
    Urho3D::Key downKey_{Urho3D::KEY_SHIFT};

    bool initialized_{false};
};

} // namespace Radon::Engine::Input
