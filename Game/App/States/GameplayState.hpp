#pragma once

#include "GameState.hpp"

#include "App/Camera/CameraController.hpp"
#include "App/Scene/SceneBuilder.hpp"
#include "App/UI/HUDBuilder.hpp"
#include "App/Graphics/ViewportManager.hpp"

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
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);

private:
    SharedPtr<SceneBuilder> sceneBuilder_;
    SharedPtr<CameraController> cameraController_;
    SharedPtr<HUDBuilder> hudBuilder_;
    SharedPtr<ViewportManager> viewportManager_;
    Node* cameraNode_{};
};

} // namespace Radon
