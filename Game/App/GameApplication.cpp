#include "GameApplication.hpp"

#include "Urho3D/IO/Log.h"
#include "Urho3D/Resource/ResourceCache.h"

void GameApplication::Setup()
{
    engineParameters_["WindowTitle"] = "Radon MVP";
    engineParameters_["FullScreen"] = false;
    engineParameters_["WindowWidth"] = 1280;
    engineParameters_["WindowHeight"] = 720;
}

void GameApplication::Start()
{
    context_->GetSubsystem<Log>()->SetLevel(LOG_DEBUG);

    URHO3D_LOGINFO("Radon application started");

    // Загрузка ресурсов
    auto* cache = GetSubsystem<ResourceCache>();
    //cache->AddResourceDir("Assets");
    //cache->AddResourceDir("Data");

    // Тут будет дальнейшая инициализация игры: сцена, UI, и т.п.
}

void GameApplication::Stop()
{
    URHO3D_LOGINFO("Radon shutting down");
}
