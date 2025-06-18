#pragma once

#include "App/GameState.hpp"

namespace Radon
{

class GameplayState final : public GameState
{
    URHO3D_OBJECT(GameplayState, GameState);

public:
    explicit GameplayState(Context* context);
    ~GameplayState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float timeStep) override;

private:
    void SetupScene();
    void SetupViewport();
    void CreateHUD();
};

} // namespace Radon
