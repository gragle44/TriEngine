#pragma once

#include "Core/Base/Application.h"
#include "Core/Base/Layer.h"
#include "Core/Base/Deltatime.h"
#include "Core/Base/Random.h"
#include "Core/Base/Log.h"
#include "Core/Base/Assert.h"
#include "Core/Base/Profiler.h"
#include "Core/Base/AssetLibrary.h"

#include "Utils/PlatformUtils.h"

//Graphics
#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Renderer/CameraController.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Buffers.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/ImGui/ImGuiLayer.h"

//Input
#include "Core/Base/Input.h"
#include "Core/Base/MouseButtonCodes.h"
#include "Core/Base/KeyCodes.h"