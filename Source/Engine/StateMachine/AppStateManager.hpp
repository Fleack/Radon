#pragma once

#include "Engine/StateMachine/IGameState.hpp"

#include <EASTL/stack.h>

namespace Radon::Engine::StateMachine
{

class AppStateManager final : public Urho3D::Object
{
    URHO3D_OBJECT(AppStateManager, Object)
public:
    explicit AppStateManager(Urho3D::Context* context);
    ~AppStateManager() override;

    void PushState(Urho3D::SharedPtr<IGameState> state);
    void ReplaceState(Urho3D::SharedPtr<IGameState> state);
    void PopState();

private:
    void Initialize();
    void Shutdown();

    void OnUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    eastl::stack<Urho3D::SharedPtr<IGameState>> states_;
};

} // namespace Radon::Engine::StateMachine
