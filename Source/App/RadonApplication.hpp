#pragma once

#include <Urho3D/Engine/Application.h>

namespace Radon::App
{

class RadonApplication : public Urho3D::Application
{
    URHO3D_OBJECT(RadonApplication, Urho3D::Application)
public:
    explicit RadonApplication(Urho3D::Context* context);
    void Setup() override;
    void Start() override;
    void Stop() override;

private:
    void RegisterSubsystems();
};

} // namespace Radon::App

URHO3D_DEFINE_APPLICATION_MAIN(Radon::App::RadonApplication);
