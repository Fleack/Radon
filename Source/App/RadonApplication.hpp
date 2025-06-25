#pragma once

#include <Urho3D/Engine/Application.h>

namespace Radon
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

} // namespace Radon
