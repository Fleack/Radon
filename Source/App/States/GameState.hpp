#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>

namespace Radon
{
using namespace Urho3D;

class GameState : public Object
{
    URHO3D_OBJECT(GameState, Object);

public:
    explicit GameState(Context* context)
        : Object(context), scene_(MakeShared<Scene>(context)) {}
    ~GameState() override = default;

    virtual void Enter() {}
    virtual void Exit() {}
    virtual void Update(float timeStep) {}

    [[nodiscard]] SharedPtr<Scene> GetScene() const { return scene_; }

protected:
    SharedPtr<Scene> scene_;
};

} // namespace Radon
