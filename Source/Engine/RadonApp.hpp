#pragma once

#include "Urho3D/Scene/Scene.h"

#include <Urho3D/Engine/Application.h>

using namespace Urho3D;

class RadonApp final : public Application
{
    URHO3D_OBJECT(RadonApp, Application);

public:
    explicit RadonApp(Context* context);
    void Setup() override;
    void Start() override;
    void Stop() override;

private:
    SharedPtr<Scene> m_scene;
    SharedPtr<Node> m_cameraNode;
};
