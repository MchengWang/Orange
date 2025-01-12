#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Orange/Scene/SceneSerializer.h"

#include "Orange/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Orange/Math/Math.h"

namespace Orange
{

	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		:Layer("Sandbox2D"), o_CameraController(1280.0f / 720.0f), o_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		o_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGET, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		o_Framebuffer = Framebuffer::Create(fbSpec);

		o_ActiveScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(o_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		o_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
#if 0
		// Entity
		auto square = o_ActiveScene->CreateEntity("Puzzle Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.52f, 0.21f, 0.52f, 1.0f });

		auto redSquare = o_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		o_SquareEntity = square;

		o_CameraEntity = o_ActiveScene->CreateEntity("Camera A");
		o_CameraEntity.AddComponent<CameraComponent>();

		o_SecondCamera = o_ActiveScene->CreateEntity("Camera B");
		auto& cc = o_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
				auto& translation = GetComponent<TransformComponent>().GetTransform();
				translation[3][0] = rand() % 10 - 5.0f;
			}

			virtual void OnDestroy() override
			{

			}

			virtual void OnUpdate(Timestep timestep) override
			{
				auto& translation = GetComponent<TransformComponent>().Translation;

				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					translation.x -= speed * timestep;
				if (Input::IsKeyPressed(Key::D))
					translation.x += speed * timestep;
				if (Input::IsKeyPressed(Key::W))
					translation.y += speed * timestep;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= speed * timestep;
			}

		};

		o_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		o_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
		o_SceneHierarchyPanel.SetContext(o_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		HZ_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = o_Framebuffer->GetSpecification();
			o_ViewportSize.x > 0.0f && o_ViewportSize.y > 0.0f &&
			(spec.Width != o_ViewportSize.x || spec.Height != o_ViewportSize.y))
		{
			o_Framebuffer->Resize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
			o_CameraController.OnResize(o_ViewportSize.x, o_ViewportSize.y);

			o_EditorCamera.SetViewportSize(o_ViewportSize.x, o_ViewportSize.y);
			o_ActiveScene->OnViewportResize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
		}

		// 更新
		if (o_ViewportFocused)
			o_CameraController.OnUpdate(timestep);

		o_EditorCamera.OnUpdate(timestep);

		// 渲染
		Renderer2D::ResetStats();
		o_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// 将我们的实体 ID 附件清除为 -1
		o_Framebuffer->ClearAttachment(1, -1);

		// 更新场景
		o_ActiveScene->OnUpdateEditor(timestep, o_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= o_ViewportBounds[0].x;
		my -= o_ViewportBounds[0].y;
		glm::vec2 viewportSize = o_ViewportBounds[1] - o_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = o_Framebuffer->ReadPixel(1, mouseX, mouseY);
			o_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, o_ActiveScene.get());
		}

		o_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		// 注意：将此项切换为 true 可启用 dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// 我们使用 ImGuiWindowFlags_NoDocking 标志使父窗口无法停靠到
		// 因为两个对接目标彼此之间会令人困惑。
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		//当使用 ImGuiDockNodeFlags_PassthruCentralNode 时，DockSpace（） 将渲染我们的背景并处理直通孔，因此我们要求 Begin（） 不渲染背景。
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		// 重要提示：请注意，即使 Begin（） 返回 false（即 window 已折叠），我们也会继续。
		// 这是因为我们希望保持 DockSpace（） 处于活动状态。如果 DockSpace（） 处于非活动状态，则
		// 停靠到其中的所有活动窗口都将丢失其父窗口并变为未停靠状态。
		// 否则，我们无法保留活动窗口和非活动停靠之间的停靠关系
		// DockSpace / 设置的任何更改都会导致 Windows 陷入困境并且永远不可见。
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// 禁用全屏将允许将窗口移动到其他窗口的前面。
				// 如果没有更精细的窗口深度 / z 控制，我们目前无法撤消。
				// ImGui：：MenuItem（“Fullscreen”， NULL， & opt_fullscreen_persistant）;

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		o_SceneHierarchyPanel.OnImGuiRender();
		o_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Status");

		std::string name = "None";
		if (o_HoveredEntity)
			name = o_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		o_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		o_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		o_ViewportFocused = ImGui::IsWindowFocused();
		o_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!o_ViewportFocused && !o_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		o_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = o_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{o_ViewportSize.x, o_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}

			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = o_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && o_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(o_ViewportBounds[0].x, o_ViewportBounds[0].y, o_ViewportBounds[1].x - o_ViewportBounds[0].x, o_ViewportBounds[1].y - o_ViewportBounds[0].y);

			//Runtime Camera from entity
			// auto cameraEntity = o_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = o_EditorCamera.GetProjection();
			glm::mat4 cameraView = o_EditorCamera.GetViewMatrix();;

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping 
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // 捕捉到 0.5m 进行平移/缩放
			// 捕捉到 45 度进行旋转
			if (o_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)o_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		o_CameraController.OnEvent(event);
		o_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(OG_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(OG_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		// 快捷键
		if (event.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (event.GetKeyCode())
		{
			case Key::N :
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();

				break;
			}

			// Gizmos
			case Key::Q:
				o_GizmoType = -1;
				break;
			case Key::W:
				o_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				o_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				o_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (o_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				o_SceneHierarchyPanel.SetSelectedEntity(o_HoveredEntity);
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		o_ActiveScene = CreateRef<Scene>();
		o_ActiveScene->OnViewportResize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
		o_SceneHierarchyPanel.SetContext(o_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Orange Scene (*.orange)\0*.orange\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		o_ActiveScene = CreateRef<Scene>();
		o_ActiveScene->OnViewportResize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
		o_SceneHierarchyPanel.SetContext(o_ActiveScene);

		SceneSerializer serializer(o_ActiveScene);
		serializer.Deserialize(path.string());
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Orange Scene (*.orange)\0*.orange\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(o_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

}
