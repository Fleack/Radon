#pragma once

#include <Urho3D/Engine/Application.h>

using namespace Urho3D;

class GameApplication : public Application
{
    URHO3D_OBJECT(GameApplication, Application);

public:
    using Application::Application;

    void Setup() override;
    void Start() override;
    void Stop() override;
};
