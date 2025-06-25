#pragma once

#include "App/Logger/Logger.hpp"

#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/ResourceCache.h>

namespace Radon
{
using namespace Urho3D;

template <typename T>
class Registry final : public Object
{
    URHO3D_OBJECT(Registry, Object);

public:
    explicit Registry(Context* context)
        : Object{context}, context_{context} {}

    void LoadAll(ea::string const& directory)
    {
        auto* cache = context_->GetSubsystem<ResourceCache>();
        auto const* fs = context_->GetSubsystem<FileSystem>();

        ea::string fullDirPath = "Data/" + directory;
        ea::vector<ea::string> files;
        fs->ScanDir(files, fullDirPath, "*.json", SCAN_FILES);

        RADON_LOGINFO("Found {} JSON files in {}", files.size(), fullDirPath);

        for (ea::string const& relPath : files)
        {
            auto const file = cache->GetResource<JSONFile>(directory + "/" + relPath);
            if (!file)
            {
                RADON_LOGERROR("Failed to load JSON: {}/{}", directory, relPath);
                continue;
            }

            T item;
            item.FromJSON(file->GetRoot());
            items_[item.id_] = item;

            fullDirPath("Loaded [{}] '{}' from {}", T::TypeName(), item.id_, relPath);
        }

        fullDirPath("Total {} {} loaded from {}", items_.size(), T::TypeName(), directory);
    }

    T* Get(ea::string const& id)
    {
        auto it = items_.find(id);
        return it != items_.end() ? &it->second : nullptr;
    }

private:
    Context* context_;
    ea::hash_map<ea::string, T> items_;
};
} // namespace Radon
