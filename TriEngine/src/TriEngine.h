#pragma once

#include "Core/Base/Application.h"
#include "Core/Base/Layer.h"
#include "Core/Base/Deltatime.h"
#include "Core/Base/UUID.h"
#include "Core/Base/Random.h"
#include "Core/Base/Log.h"
#include "Core/Base/Assert.h"
#include "Core/Base/Profiler.h"
#include "Core/Base/AssetLibrary.h"

#include "Utils/PlatformUtils.h"

//Projects and Scenes
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Resource.h"
#include "Core/Projects/Project.h"
#include "Core/Projects/ProjectManager.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Script.h"
#include "Core/GameObjects/ScriptRegistry.h"
#include "Core/GameObjects/Scene.h"
#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/SceneSerializer.h"
#include "Core/GameObjects/EditorCamera.h"


//Graphics
#include "Core/Renderer/GameRenderer.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/OrthographicCamera.h"
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