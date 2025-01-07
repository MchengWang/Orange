#pragma once

#include "entt.hpp"

#include "Orange/Core/Timestep.h"

namespace Orange
{

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep timestep);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry o_Registry;
		uint32_t o_ViewportWidth = 0, o_ViewportHeight = 0;

		friend class Entity;
	};

}