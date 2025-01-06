#pragma once

#include "entt.hpp"

#include "Orange/Core/Timestep.h"

namespace Orange
{

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// ¡Ÿ ±
		entt::registry& Reg() { return o_Registry; }

		void OnUpdate(Timestep timestep);

	private:
		entt::registry o_Registry;
	};

}