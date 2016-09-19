#ifndef ENTITY_HPP
#define ENTITY_HPP

class EntitiesManager;

struct ID {
public:
    ID() : m_id(0) {};
    explicit ID(std::uint64_t id) : m_id(id) {}
    
    std::uint64_t id() const { return m_id; }
    
    bool operator == (const ID &other) const { return m_id == other.m_id; }
    bool operator != (const ID &other) const { return m_id != other.m_id; }
    bool operator < (const ID &other) const { return m_id < other.m_id; }
    
private:
    std::uint64_t m_id; 
}

class Entity {
public:
    static const ID INVALID;
    
public:
    Entity() = default;
    
    ID id() const { return m_id; }
private:
    EntitiesManager* m_entitiesManager = nullptr;
    ID m_id = INVALID;
        
};

#endif