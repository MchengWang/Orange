#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Orange
{

	EditorLayer::EditorLayer()
		:Layer("Sandbox2D"), o_CameraController(1280.0f / 720.0f), o_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		o_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		o_Framebuffer = Framebuffer::Create(fbSpec);

		o_ActiveScene = CreateRef<Scene>();

		// Entity
		auto square = o_ActiveScene->CreateEntity("Puzzle Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.52f, 0.21f, 0.52f, 1.0f });

		o_SquareEntity = square;

		o_CameraEntity = o_ActiveScene->CreateEntity("Camera Entity");
		o_CameraEntity.AddComponent<CameraComponent>();

		o_SecondCamera = o_ActiveScene->CreateEntity("Clip-Space Entity");
		auto& cc = o_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;
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
			(spec.width != o_ViewportSize.x || spec.height != o_ViewportSize.y))
		{
			o_Framebuffer->Resize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
			o_CameraController.OnResize(o_ViewportSize.x, o_ViewportSize.y);

			o_ActiveScene->OnViewportResize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
		}

		// ����
		if (o_ViewportFocused)
			o_CameraController.OnUpdate(timestep);


		// ��Ⱦ
		Renderer2D::ResetStats();
		o_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		// update Scene �����³�����
		o_ActiveScene->OnUpdate(timestep);

		o_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		// ע�⣺�������л�Ϊ true ������ dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// ����ʹ�� ImGuiWindowFlags_NoDocking ��־ʹ�������޷�ͣ����
		// ��Ϊ�����Խ�Ŀ��˴�֮�����������
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

		//��ʹ�� ImGuiDockNodeFlags_PassthruCentralNode ʱ��DockSpace���� ����Ⱦ���ǵı���������ֱͨ�ף��������Ҫ�� Begin���� ����Ⱦ������
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		// ��Ҫ��ʾ����ע�⣬��ʹ Begin���� ���� false���� window ���۵���������Ҳ�������
		// ������Ϊ����ϣ������ DockSpace���� ���ڻ״̬����� DockSpace���� ���ڷǻ״̬����
		// ͣ�������е����л���ڶ�����ʧ�丸���ڲ���Ϊδͣ��״̬��
		// ���������޷���������ںͷǻͣ��֮���ͣ����ϵ
		// DockSpace / ���õ��κθ��Ķ��ᵼ�� Windows ��������������Զ���ɼ���
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// ����ȫ���������������ƶ����������ڵ�ǰ�档
				// ���û�и���ϸ�Ĵ������ / z ���ƣ�����Ŀǰ�޷�������
				// ImGui����MenuItem����Fullscreen���� NULL�� & opt_fullscreen_persistant��;
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		if (o_SquareEntity)
		{
			ImGui::Separator();
			auto& tag = o_SquareEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("%s", tag.c_str());
			auto& squareColor = o_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Transform",
			glm::value_ptr(o_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Camera A", &o_PrimaryCamera))
		{
			o_CameraEntity.GetComponent<CameraComponent>().Primary = o_PrimaryCamera;
			o_SecondCamera.GetComponent<CameraComponent>().Primary = !o_PrimaryCamera;
		}

		{
			auto& camera = o_SecondCamera.GetComponent<CameraComponent>().Camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		o_ViewportFocused = ImGui::IsWindowFocused();
		o_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!o_ViewportFocused || !o_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		o_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = o_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ o_ViewportSize.x, o_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		o_CameraController.OnEvent(event);
	}

}