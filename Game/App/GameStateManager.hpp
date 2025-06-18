#pragma once

#include "GameState.hpp"

#include <stack>

#include <Urho3D/Core/Object.h>

namespace Radon
{
using namespace Urho3D;

class GameStateManager final : public Object
{
    URHO3D_OBJECT(GameStateManager, Object);

public:
    explicit GameStateManager(Context* context);
    ~GameStateManager() override = default;

    void Update(float timeStep);

    template <typename T, typename... Args>
    void PushState(Args&&... args)
    {
        if (!states_.empty())
            states_.top()->Exit();

        auto newState = MakeShared<T>(context_, std::forward<Args>(args)...);
        states_.push(newState);
        newState->Enter();
    }

    void PopState();
    void ClearStates();

    bool HasStates() const { return !states_.empty(); }
    SharedPtr<GameState> CurrentState() const { return states_.empty() ? nullptr : states_.top(); }

private:
    std::stack<SharedPtr<GameState>> states_;
};

} // namespace Radon
