#pragma once

#include "Entity.h"

namespace Orange
{

	class ScriptableEntity
	{
	public:
		template <typename T>
		T& GetComponent()
		{
			return o_Entity.GetComponent<T>();
		}

	private:
		Entity o_Entity;
		friend class Scene;
	};

}