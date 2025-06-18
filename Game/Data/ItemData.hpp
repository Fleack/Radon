#pragma once

#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/JSONValue.h>

namespace Radon
{
using namespace Urho3D;

struct ItemData
{
    static ea::string_view TypeName() { return "Item"; }

    ea::string id_;
    ea::string name_;
    float weight_{0.0f};

    void FromJSON(JSONValue const& root)
    {
        id_ = root["id"].GetString();
        name_ = root["name"].GetString();
        weight_ = root["weight"].GetFloat();

        URHO3D_LOGDEBUG("Parsed ItemData: {} (weight={})", id_, weight_);
    }
};
} // namespace Radon
