#pragma once

#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/JSONFile.h>
#include <Urho3D/Resource/ResourceCache.h>

namespace Radon
{
using namespace Urho3D;

template <typename T>
class Registry
{
public:
    void LoadAll(Context const* context, ea::string const& directory)
    {
        auto* cache = context->GetSubsystem<ResourceCache>();
        auto const* fs = context->GetSubsystem<FileSystem>();

        ea::string fullDirPath = "Data/" + directory;
        ea::vector<ea::string> files;
        fs->ScanDir(files, fullDirPath, "*.json", SCAN_FILES);

        URHO3D_LOGINFO("Scanning directory: {}", fullDirPath);
        URHO3D_LOGINFO("Found {} JSON files", files.size());

        for (ea::string const& relPath : files)
        {
            auto const file = cache->GetResource<JSONFile>(directory + "/" + relPath);
            if (!file)
            {
                URHO3D_LOGERROR("Failed to load JSON: {}/{}", directory, relPath);
                continue;
            }

            T item;
            item.FromJSON(file->GetRoot());
            items_[item.id_] = item;

            URHO3D_LOGINFO("Loaded [{}] '{}' from {}", T::TypeName(), item.id_, relPath);
        }

        URHO3D_LOGINFO("Total {} {} loaded from {}", items_.size(), T::TypeName(), directory);
    }

    T* Get(ea::string const& id)
    {
        auto it = items_.find(id);
        return it != items_.end() ? &it->second : nullptr;
    }

private:
    ea::hash_map<ea::string, T> items_;
};
} // namespace Radon
