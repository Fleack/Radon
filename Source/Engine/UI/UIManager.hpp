#pragma once

#include "Engine/UI/FontLoader.hpp"

#include <RmlUi/Core/ElementDocument.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/UI/Button.h>

namespace Radon::Engine::UI
{

class UIManager : public Urho3D::Object
{
    URHO3D_OBJECT(UIManager, Object)
public:
    explicit UIManager(Urho3D::Context* context);
    ~UIManager() override;

    Rml::ElementDocument* ShowDocument(ea::string const& name);
    void HideDocument(ea::string const& name);

    void UnloadDocument(ea::string const& name);

    Rml::ElementDocument* GetDocument(ea::string const& name) const;

private:
    Rml::ElementDocument* LoadDocument(ea::string const& name);
    void ClearDocuments();

private:
    FontLoader fontLoader_;
    ea::hash_map<ea::string, Rml::ElementDocument*> documents_;
};

} // namespace Radon::Engine::UI
