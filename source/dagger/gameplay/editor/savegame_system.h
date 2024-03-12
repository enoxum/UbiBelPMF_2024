#pragma once 

#include "core/core.h"

template<typename Archetype>
struct SaveGame
{
	Archetype archetype;
};

template<typename Archetype>
struct LoadGame
{
    Archetype archetype;
};

template<typename Archetype>
struct SaveLoadProcess
{
    virtual Archetype Save(Entity entity_, JSON::json& saveTo_) = 0;
    virtual void Load(Archetype archetype_, Entity entity_, JSON::json& loadFrom_) = 0;
};

template<typename Archetype>
struct SaveGameSystem : public dagger::System
{
    using SaveRequest = AssetLoadRequest<SaveGame<Archetype>>;
    using LoadRequest = AssetLoadRequest<LoadGame<Archetype>>;

    SaveGameSystem(SaveLoadProcess<Archetype>* process)
        : m_Process{ process }
    {}

    String SystemName() override { return "SaveGame"; }

    void SpinUp() override
    {
        Engine::Dispatcher().sink<SaveRequest>().template connect<&SaveGameSystem::OnSaveRequested>(this);
        Engine::Dispatcher().sink<LoadRequest>().template connect<&SaveGameSystem::OnLoadRequested>(this);
    }

    void WindDown() override
    {
        Engine::Dispatcher().sink<LoadRequest>().template disconnect<&SaveGameSystem::OnLoadRequested>(this);
        Engine::Dispatcher().sink<SaveRequest>().template disconnect<&SaveGameSystem::OnSaveRequested>(this);
    }

    void OnSaveRequested(SaveRequest request_)
    {
        auto& engine = Engine::Instance();
        auto& registry = engine.Registry();

        JSON::json save{};

        registry.view<SaveGame<Archetype>>().each([&](Entity entity_, SaveGame<Archetype>& save_)
            {
                JSON::json item{};
                
                item["metadata"] = (SInt32) this->m_Process->Save(entity_, item);
                save.push_back(item);
            });

        std::ofstream file{ request_.path.c_str() };
        file << save;
        file.close();
    }

    void OnLoadRequested(LoadRequest request_)
    {
        auto& engine = Engine::Instance();
        auto& registry = engine.Registry();

        JSON::json load{};
        std::ifstream file{ request_.path.c_str() };
        file >> load;
        file.close();

        for (auto item : load)
        {
            Entity entity = registry.create();
            SaveGame<Archetype>& meta = registry.emplace<SaveGame<Archetype>>(entity);
            meta.archetype = (Archetype)item["metadata"];
            this->m_Process->Load(meta.archetype, entity, item);
        }
    }

private:
    SaveLoadProcess<Archetype>* m_Process;
};