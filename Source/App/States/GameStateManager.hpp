#pragma once

#include "App/States/IGameState.hpp"

#include <stack>

namespace Radon::States
{

class GameStateManager : public Urho3D::Object
{
    URHO3D_OBJECT(GameStateManager, Object)
public:
    explicit GameStateManager(Urho3D::Context* context);
    void Update(float timeStep);
    void PushState(Urho3D::SharedPtr<IGameState> state);
    void ReplaceState(Urho3D::SharedPtr<IGameState> state);
    void PopState();

private:
    std::stack<Urho3D::SharedPtr<IGameState>> states_;
};

} // namespace Radon::States
