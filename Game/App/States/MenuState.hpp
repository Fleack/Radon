#pragma once

#include "App/States/GameState.hpp"

namespace Radon
{

class MenuState final : public GameState
{
    URHO3D_OBJECT(MenuState, GameState);

public:
    explicit MenuState(Context* context);
    ~MenuState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float timeStep) override;

private:
    void CreateUI();
    void HandlePlayPressed(StringHash, VariantMap&);
    void HandleExitPressed(StringHash, VariantMap&);
};

} // namespace Radon
