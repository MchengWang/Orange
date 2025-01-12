#pragma once

#include "Orange/Core/Timestep.h"
#include "Orange/Core/UUID.h"
#include "Orange/Renderer/EditorCamera.h"

#include "entt.hpp"

class b2World;

namespace Orange
{

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep timestep);
		void OnUpdateEditor(Timestep timestep, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		template <typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry o_Registry;
		uint32_t o_ViewportWidth = 0, o_ViewportHeight = 0;

		b2World* o_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
