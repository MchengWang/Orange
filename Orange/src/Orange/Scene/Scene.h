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

	private:
		entt::registry o_Registry;

		friend class Entity;
	};

}