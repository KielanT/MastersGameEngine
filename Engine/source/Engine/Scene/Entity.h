#pragma once

#include "entt/entt.hpp"
#include "Engine/Scene/Scene.h"
//#include "Engine/Interfaces/IScene.h"
#include "Components.h"

namespace Engine
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, std::shared_ptr<Scene> scene);
		Entity(const Entity& other) = default;
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			// TODO: Check here if compoent alread exists
			T& comp = m_Scene->GetEntityRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return comp;
		}

		template<typename T>
		bool HasComponent()
		{
			if (m_EntityHandle == entt::null) return false;
			return m_Scene->GetEntityRegistry().any_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->GetEntityRegistry().get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->GetEntityRegistry().remove<T>(m_EntityHandle);
		}

		entt::entity GetEntityHandle()
		{
			return m_EntityHandle;
		}

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle/* && m_Scene == other.m_Scene*/;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		std::shared_ptr<Scene> m_Scene = nullptr;
		
	};
}