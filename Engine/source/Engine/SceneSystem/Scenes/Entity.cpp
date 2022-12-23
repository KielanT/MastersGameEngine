#include "epch.h"
#include "Entity.h"

namespace Engine
{
    Entity::Entity(entt::entity handle, std::shared_ptr<Scene> scene) :
        m_EntityHandle(handle), m_Scene(scene)
    {
    }
}