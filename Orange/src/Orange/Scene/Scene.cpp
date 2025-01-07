#include "ogpch.h"
#include "Scene.h"

#include "Components.h"
#include "Orange/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Orange
{

	static void DoMath(const glm::mat4& transform)
	{

	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{

	}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = o_Registry.create();
		o_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		o_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		if (o_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = o_Registry.get<TransformComponent>(entity);
		auto view = o_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}
		auto group = o_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { o_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		// Update scripts
		{
			o_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.Instance)
					{
						nsc.InstantiateFunction();
						nsc.Instance->o_Entity = Entity{ entity, this };
						if (nsc.OnCreateFunction)
							nsc.OnCreateFunction(nsc.Instance);
					}
					if (nsc.OnUpdateFunction)
						nsc.OnUpdateFunction(nsc.Instance, timestep);
				});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = o_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = o_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		o_ViewportWidth = width;
		o_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = o_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

}