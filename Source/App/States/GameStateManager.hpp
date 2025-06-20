#pragma once

#include "GameState.hpp"

#include <stack>

#include <Urho3D/Core/Object.h>

namespace Radon
{
using namespace Urho3D;

template <typename T>
concept GameStateDerived = std::derived_from<T, GameState>;

class GameStateManager final : public Object
{
    URHO3D_OBJECT(GameStateManager, Object);

public:
    explicit GameStateManager(Context* context);
    ~GameStateManager() override = default;

    void Update(float timeStep);

    template <typename GameStateDerived, typename... Args>
    void PushState(Args&&... args);

    template <typename GameStateDerived, typename... Args>
    void ReplaceState(Args&&... args);

    void PopState();
    void ClearStates();

    [[nodiscard]] bool HasStates() const { return !states_.empty(); }
    [[nodiscard]] SharedPtr<GameState> CurrentState() const { return states_.empty() ? nullptr : states_.top(); }

private:
    std::stack<SharedPtr<GameState>> states_;
};

} // namespace Radon
