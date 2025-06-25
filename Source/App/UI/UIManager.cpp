#include "UIManager.hpp"

#include "App/Logger/Logger.hpp"

#include <RmlUi/Core/ElementDocument.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/RmlUI/RmlUI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

using namespace Radon::UI;

UIManager::UIManager(Urho3D::Context* context)
    : Object(context)
{
    RADON_LOGDEBUG("UIManager: constructor called");
}

UIManager::~UIManager()
{
    RADON_LOGDEBUG("UIManager: destructor called");
    ClearDocuments();
}

Rml::ElementDocument* UIManager::ShowDocument(ea::string const& name)
{
    RADON_LOGDEBUG("UIManager: ShowDocument('{}')", name);
    if (auto* doc = LoadDocument(name))
    {
        doc->Show();
        RADON_LOGINFO("UIManager: Document '{}' shown", name);
        return doc;
    }

    RADON_LOGERROR("Cannot show document, not loaded: {}", name);
    return nullptr;
}

void UIManager::HideDocument(ea::string const& name)
{
    RADON_LOGDEBUG("UIManager: HideDocument('{}')", name);
    if (auto* doc = GetDocument(name))
        doc->Hide();
}

Rml::ElementDocument* UIManager::GetDocument(ea::string const& name) const
{
    auto it = documents_.find(name);
    return it != documents_.end() ? it->second : nullptr;
}

Rml::ElementDocument* UIManager::LoadDocument(ea::string const& name)
{
    RADON_LOGDEBUG("UIManager: LoadDocument('{}')", name);
    auto* rmlui = GetSubsystem<Urho3D::RmlUI>();
    if (!rmlui)
    {
        RADON_LOGERROR("RmlUI subsystem not found");
        return nullptr;
    }

    auto it = documents_.find(name);
    if (it != documents_.end())
        return it->second;

    if (auto* doc  = rmlui->LoadDocument("UI/" + name + ".rml"))
    {
        documents_.emplace(name, doc);
        RADON_LOGINFO("UIManager: Document '{}' loaded", name);
        return doc;
    }

    RADON_LOGERROR("Failed to load RmlUI document: {}", name);
    return nullptr;
}

void UIManager::ClearDocuments()
{
    RADON_LOGDEBUG("UIManager: ClearDocuments called");
    if (auto* rmlui = GetSubsystem<Urho3D::RmlUI>())
    {
        for (auto& [name, doc] : documents_)
            if (doc)
            {
                rmlui->GetRmlContext()->UnloadDocument(doc);
                RADON_LOGINFO("UIManager: Document '{}' unloaded", name);
            }
        documents_.clear();
    }
}

void UIManager::UnloadDocument(ea::string const& name)
{
    RADON_LOGDEBUG("UIManager: UnloadDocument('{}')", name);
    auto* rmlui = GetSubsystem<Urho3D::RmlUI>();
    if (!rmlui)
    {
        RADON_LOGERROR("RmlUI subsystem not found");
        return;
    }

    auto it = documents_.find(name);
    if (it == documents_.end())
        return;

    if (auto* doc = it->second)
    {
        rmlui->GetRmlContext()->UnloadDocument(doc);
        RADON_LOGINFO("UIManager: Document '{}' unloaded", name);
    }
    documents_.erase(it);
}
