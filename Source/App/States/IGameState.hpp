#pragma once

#include <Urho3D/Core/Object.h>

namespace Radon::States
{

class IGameState : public Urho3D::Object
{
    URHO3D_OBJECT(IGameState, Object)
public:
    explicit IGameState(Urho3D::Context* context)
        : Object(context) {}
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update(float timeStep) = 0;
};

} // namespace Radon::States
