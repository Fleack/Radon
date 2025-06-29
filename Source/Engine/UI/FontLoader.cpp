#include "FontLoader.hpp"

#include "Engine/Core/Logger.hpp"

#include <ranges>

#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/RmlUI/RmlUI.h>

using namespace Radon::Engine::UI;

FontLoader::FontLoader(Urho3D::Context* context)
    : Object(context)
{
    rmlUI_ = GetSubsystem<Urho3D::RmlUI>();
    fileSystem_ = GetSubsystem<Urho3D::FileSystem>();
}

void FontLoader::LoadFontsFromDirectory(ea::string const& directory) const
{
    RADON_LOGDEBUG("FontLoader: Loading fonts from dir: {}", directory);
    using namespace Urho3D;

    ea::vector<ea::string> fonts;
    fileSystem_->ScanDir(fonts, "Data/" + directory, "*.*tf", SCAN_FILES);

    for (auto const& fontFile : fonts)
    {
        if (rmlUI_->LoadFont(directory + fontFile))
            RADON_LOGDEBUG("FontLoader: Loaded font: {}", fontFile);
        else
            RADON_LOGWARN("FontLoader: Failed to load font: {}", fontFile);
    }
}
