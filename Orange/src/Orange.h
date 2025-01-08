#pragma once

// 供 Orange 应用使用
#include "Orange/Core/Base.h"

#include "Orange/Core/Application.h"
#include "Orange/Core/Layer.h"
#include "Orange/Core/Log.h"
#include "Orange/Core/Assert.h"

#include "Orange/Core/Timestep.h"

#include "Orange/Core/Input.h"
#include "Orange/Core/KeyCodes.h"
#include "Orange/Core/MouseCodes.h"
#include "Orange/Renderer/OrthographicCameraController.h"

#include "Orange/ImGui/ImGuiLayer.h"

#include "Orange/Scene/Scene.h"
#include "Orange/Scene/Entity.h"
#include "Orange/Scene/ScriptableEntity.h"
#include "Orange/Scene/Components.h"

// ---------渲染器----------
#include "Orange/Renderer/Renderer.h"
#include "Orange/Renderer/Renderer2D.h"
#include "Orange/Renderer/RenderCommand.h"

#include "Orange/Renderer/Buffer.h"
#include "Orange/Renderer/Shader.h"
#include "Orange/Renderer/Framebuffer.h"
#include "Orange/Renderer/Texture.h"
#include "Orange/Renderer/SubTexture2D.h"
#include "Orange/Renderer/VertexArray.h"

#include "Orange/Renderer/OrthographicCamera.h"
// -------------------------
