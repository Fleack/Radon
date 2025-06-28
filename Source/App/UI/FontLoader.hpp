#pragma once

#include <Urho3D/Core/Object.h>

namespace Urho3D
{
class Context;
class RmlUI;
class FileSystem;
class ResourceCache;
} // namespace Urho3D

namespace Radon::UI
{
class FontLoader : public Urho3D::Object
{
    URHO3D_OBJECT(FontLoader, Object)

public:
    explicit FontLoader(Urho3D::Context* context);

    void LoadFontsFromDirectory(ea::string const& directory) const;

private:
    Urho3D::RmlUI* rmlUI_{};
    Urho3D::FileSystem* fileSystem_{};
    Urho3D::ResourceCache* resourceCache_{};
};

} // namespace Radon::UI
