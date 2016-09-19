#ifndef ENTITIESMANAGER_HPP
#define ENTITIESMANAGER_HPP

#include <Global.hpp>

#include <unordered_map>
#include <cstdint>

#include <ECS/Entity.hpp>

template <class Components>
class EntitiesManager {
public:
    EntitiesManager();
    
    Entity createDrawable();
    Entity createOther();
    
    
private:
    std::unordered_map<uint32_t, std::vector<Entity>> m_drawableEntities;
    std::vector<Entity> m_otherEntities;
};

#endif