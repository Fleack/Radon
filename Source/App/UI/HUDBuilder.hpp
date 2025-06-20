#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/UI/Text.h>

namespace Radon
{
using namespace Urho3D;

class HUDBuilder : public Object
{
    URHO3D_OBJECT(HUDBuilder, Object);

public:
    explicit HUDBuilder(Context* context);
    ~HUDBuilder() override = default;

    void CreateGameplayHUD();

private:
    SharedPtr<Text> CreateInfoText(const ea::string& content);
};

} // namespace Radon
