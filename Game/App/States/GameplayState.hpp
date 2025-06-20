#pragma once

#include "GameState.hpp"

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

private:
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
    void HandleUpdate(StringHash eventType, VariantMap& eventData);

private:
    Node* cameraNode_{};
    float yaw_{0.0f}, pitch_{0.0f};
    float const moveSpeed_{10.0f};
    float const lookSensitivity_{0.1f};
};

} // namespace Radon
