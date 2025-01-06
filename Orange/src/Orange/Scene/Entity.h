#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Orange
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			OG_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return o_Scene->o_Registry.emplace<T>(o_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			OG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return o_Scene->o_Registry.get<T>(o_EntityHandle);
		}

		template <typename T>
		bool HasComponent()
		{
			// 最新中 entt 没有 has 方法
			// 替代方法 all_of (检查实体是否是所有给定存储的一部分。) entt.hpp 中标注
			return o_Scene->o_Registry.all_of<T>(o_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			OG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			o_Scene->o_Registry.remove<T>(o_EntityHandle);
		}

		operator bool() const { return o_EntityHandle != entt::null; }

	private:
		entt::entity o_EntityHandle{ 0 };
		Scene* o_Scene = nullptr;
	};

}