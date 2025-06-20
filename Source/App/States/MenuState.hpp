#pragma once

#include "App/States/GameState.hpp"
#include "App/Scene/SceneBuilder.hpp"
#include "App/Scene/MenuSceneAnimator.hpp"
#include "App/UI/MenuBuilder.hpp"
#include "App/Graphics/ViewportManager.hpp"

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
    void HandlePlayPressed(StringHash, VariantMap&);
    void HandleExitPressed(StringHash, VariantMap&);

private:
    SharedPtr<SceneBuilder> sceneBuilder_;
    SharedPtr<MenuBuilder> menuBuilder_;
    SharedPtr<ViewportManager> viewportManager_;
    SharedPtr<MenuSceneAnimator> sceneAnimator_;
    Node* cameraNode_{};
};

} // namespace Radon
