#include "epch.h"
#include "Entity.h"
#include "Engine/Scripting/Scripting.h"

namespace Engine
{
    Entity::Entity(entt::entity handle, std::shared_ptr<Scene> scene) :
        m_EntityHandle(handle), m_Scene(scene)
    {
    }

    void Entity::OnContact(Entity other)
    {
        if (other)
        {
            //Scripting::GetInstance()->OnContactEntity(other);
        }
    }

}