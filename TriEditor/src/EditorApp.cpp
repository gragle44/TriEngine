#include "EditorApp.h"

#include "EditorUtils.h"

#include "Core/Scripting/GenerateDefinitions.h"

#include <filesystem>

EditorLayer::EditorLayer()
	:m_Data(new EditorData), m_FileMenu(m_Data), m_SceneModule(m_Data), m_DebugModule(m_Data)
{
	m_Data->Sink = std::make_shared<imgui_sink_mt>();
	TriEngine::Log::AddSink(m_Data->Sink);
}

EditorLayer::~EditorLayer()
{
	delete m_Data;
}

void EditorLayer::SetupImGuiStyle()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/droidsans/DroidSans.ttf", 18.0f);
	TRI_CORE_ASSERT(io.FontDefault != NULL, "Default TriEditor font was null!");

	SetFont<FontType::Bold>(io.Fonts->AddFontFromFileTTF("assets/fonts/droidsans/DroidSans-Bold.ttf", 18.0f));

	style.FontScaleDpi = 1.2f;
	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(6.0f, 6.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(12.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
	style.CellPadding = ImVec2(12.0f, 6.0f);
	style.IndentSpacing = 20.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 12.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0490588244497776f, 0.05690196123719215f, 0.07258823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}


void EditorLayer::OnAttach()
{
	m_Data->ViewPortSize = { 1280, 720 };

	m_Data->Camera = std::make_unique<EditorCamera>();
	m_Data->Renderer = std::make_shared<GameRenderer>();

	m_Data->RestoreScene = Scene::Create();

	m_Data->ActiveScene = m_Data->RestoreScene;

	TextureSettings iconTextureSettings;
	iconTextureSettings.Filter = TextureFilter::Linear;

	m_Data->PlayTexture = Texture2D::Create("assets/icons/playbutton.png", iconTextureSettings);
	m_Data->PauseTexture = Texture2D::Create("assets/icons/pausebutton.png", iconTextureSettings);
	m_Data->FolderTexture = Texture2D::Create("assets/icons/folder.png", iconTextureSettings);
	m_Data->ReloadTexture = Texture2D::Create("assets/icons/reload.png", iconTextureSettings);

	SetupImGuiStyle();
}

void EditorLayer::OnDetach()
{
	m_Data->ActiveScene->Stop();
	delete m_Data;
}

void EditorLayer::OnUpdate(float deltaTime)
{
	m_DebugModule.OnUpdate(deltaTime);
	if (m_Data->PrevViewPortSize.x != m_Data->ViewPortSize.x || m_Data->PrevViewPortSize.y != m_Data->ViewPortSize.y && m_Data->ViewPortSize.x > 0 && m_Data->ViewPortSize.y > 0) {
		m_Data->PrevViewPortSize = m_Data->ViewPortSize;

		m_Data->Camera->SetViewportSize(m_Data->ViewPortSize.x, m_Data->ViewPortSize.y);
		m_Data->Renderer->SetViewportSize((uint32_t)m_Data->ViewPortSize.x, (uint32_t)m_Data->ViewPortSize.y);
		m_Data->ActiveScene->OnViewportResized((uint32_t)m_Data->ViewPortSize.x, (uint32_t)m_Data->ViewPortSize.y);
	}

	if (m_Data->SceneRunning) {
		m_Data->ActiveScene->OnUpdate(deltaTime);
		m_Data->Renderer->RenderSceneEditor(m_Data->ActiveScene.get(), nullptr, {});
	}
	else if (!m_Data->SceneRunning) {
		if (m_Data->ViewPortHovered)
			m_Data->Camera->OnUpdate(deltaTime);

		m_Data->Renderer->RenderSceneEditor(m_Data->ActiveScene.get(), m_Data->Camera.get(), m_Data->SelectedItem);

		if (m_Data->ViewPortHovered && Input::IsMouseButtonPressed(TRI_MOUSE_BUTTON_LEFT)) {
			auto pos = ImGui::GetMousePos();

			if (pos.x >= m_Data->ViewPortBoundsMin.x && pos.y >= m_Data->ViewPortBoundsMin.y && pos.x <= m_Data->ViewPortBoundsMax.x && pos.y <= m_Data->ViewPortBoundsMax.y) {

				int X = (pos.x - m_Data->ViewPortBoundsMin.x);
				int Y = (pos.y - m_Data->ViewPortBoundsMin.y);
				Y = m_Data->ViewPortSize.y - Y;

				int32_t pixel = m_Data->Renderer->GetFinalFramebuffer()->ReadPixel(1, X, Y);

				if (pixel >= 0) {
					GameObject pickedObject = GameObject((entt::entity)pixel, m_Data->ActiveScene.get());
					m_Data->SelectedItem = pickedObject;
				}
				else {
					m_Data->SelectedItem = {};
				}
			}
		}
	}
}

void EditorLayer::OnRender(float deltaTime)
{

}

void EditorLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(TRI_BIND_EVENT_FN(EditorLayer::OnKeyPressed));

	if (m_Data->ViewPortHovered)
		m_Data->Camera->OnEvent(e);
	m_Data->ActiveScene->OnEvent(e);
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == TRI_KEY_Z) {
		if (Input::IsKeyPressed(TRI_KEY_LEFT_CONTROL) || Input::IsKeyPressed(TRI_KEY_RIGHT_CONTROL))
			m_Data->CmdHistory.Undo();
	}

	else if (e.GetKeyCode() == TRI_KEY_Y) {
		if (Input::IsKeyPressed(TRI_KEY_LEFT_CONTROL) || Input::IsKeyPressed(TRI_KEY_RIGHT_CONTROL))
			m_Data->CmdHistory.Redo();
	}
	return false;
}

void EditorLayer::StartScene()
{
	m_Data->SelectedItem = {};
	m_Data->RightSelectedItem = {};

	m_Data->SceneRunning = true;
	m_Data->SceneCurrentState = EditorState::Play;
	m_Data->RestoreScene = m_Data->ActiveScene->Copy();

	m_Data->ActiveScene->OnViewportResized(m_Data->ViewPortSize.x, m_Data->ViewPortSize.y);
	m_Data->ActiveScene->Start();
	TRI_CORE_ASSERT(*m_Data->ActiveScene == *m_Data->RestoreScene, "Active Scene and Restore Scene have different IDs")

	//todo: when making a new project, set the new scene as startup
}

void EditorLayer::StopScene()
{
	m_Data->SelectedItem = {};
	m_Data->RightSelectedItem = {};

	m_Data->SceneRunning = false;
	m_Data->SceneCurrentState = EditorState::Edit;
	m_Data->ActiveScene->Stop();
	m_Data->ActiveScene = m_Data->RestoreScene->Copy();
	TRI_CORE_ASSERT(*m_Data->ActiveScene == *m_Data->RestoreScene, "Active Scene and Restore Scene have different IDs")
}

void EditorLayer::LoadEmptyScene()
{
	StopScene();
	m_Data->ActiveScene = Scene::Create();
	m_Data->RestoreScene = m_Data->ActiveScene->Copy();

}

static void GenerateScriptDefinitions() {
	auto scriptDefinitionsPath = ProjectManager::GetCurrent()->GetAbsolutePath("as.predefined");
	auto& engine = ScriptEngine::Get();
	ScriptUtils::GenerateScriptPredefined(engine.GetASEngine(), scriptDefinitionsPath.string());
}

void EditorLayer::NewProject(const std::string& path) {
	TRI_CORE_ASSERT((path.ends_with(".tri")), "Attempting to create a new project with an invalid project file extension");

	LoadEmptyScene();

	TriEngine::SceneSerializer s(m_Data->ActiveScene);

	std::string scenePath = path;
	scenePath.erase(path.length()-3, 3);
	scenePath.append("tscn");

	// Create scene file so it can be imported by the resource manager
	s.Serialize(scenePath);

	ProjectManager::CreateNew(path);
	ResourceManager::Init();

	m_Data->ActiveScene = ResourceManager::Create<Scene>(scenePath);
	m_Data->RestoreScene = m_Data->ActiveScene->Copy();

	ProjectManager::GetCurrent()->GetProjectData().StartupSceneID = m_Data->ActiveScene->MetaData.ID;
	SaveProject(path);

	GenerateScriptDefinitions();

	m_Data->NoProjectLoaded = false;
}


void EditorLayer::LoadProject(const std::string& path)
{

	if (TriEngine::ProjectManager::GetCurrent() != nullptr) {
		//Unload project
		TriEngine::ResourceManager::Shutdown();
	}

	std::filesystem::path filePath = path;
	TriEngine::ProjectManager::Load(filePath);

	TriEngine::ResourceManager::Init();

	const TriEngine::ProjectData& projectData = TriEngine::ProjectManager::GetCurrent()->GetProjectData();
	if (ResourceManager::ResourceExists(projectData.StartupSceneID)) {
		LoadScene(projectData.StartupSceneID);
	}
	else {
		LoadEmptyScene();
	}

	GenerateScriptDefinitions();
	
	m_Data->NoProjectLoaded = false;
}

void EditorLayer::SaveProject(const std::string& path)
{
	ProjectManager::Save(path);
}

void EditorLayer::LoadScene(ResourceID id)
{
	StopScene();
	m_Data->ActiveScene = std::reinterpret_pointer_cast<Scene>(ResourceManager::Get(id));
}

void EditorLayer::SaveScene(const std::string& path)
{
	if (m_Data->SceneRunning)
		return;

	if (!ResourceManager::ResourceExists(m_Data->ActiveScene->MetaData.ID)) {
		//TODO: Resource manager needs existing file to create a resource, probably fix this later
		TriEngine::SceneSerializer s(m_Data->ActiveScene);
		s.Serialize(path);
		
		ResourceManager::Create<Scene>(path);
		return;
	}
	ResourceManager::SaveResource(m_Data->ActiveScene);
}

void EditorLayer::OnImGuiRender()
{
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
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

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("TriEditor DockSpace", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("TriSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	// Render modules
	if (ImGui::BeginMenuBar())
	{
		UpdateTitleBar();
		m_FileMenu.OnImGuiRender();
		RenderPlaybuttons();
		ImGui::EndMenuBar();
	}

	m_SceneModule.OnImGuiRender();
	m_DebugModule.OnImGuiRender();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Viewport")) {
		m_Data->SceneViewPaused = false;

		m_Data->ViewPortHovered = ImGui::IsWindowHovered();

		ImGui::SetNextFrameWantCaptureKeyboard(m_Data->SceneViewPaused);
		ImGui::SetNextFrameWantCaptureMouse(m_Data->SceneViewPaused);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_Data->ViewPortSize = { (uint32_t)viewportSize.x, (uint32_t)viewportSize.y };

		ImVec2 viewportMinRegion = ImGui::GetWindowContentRegionMin();
		ImVec2 viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		ImVec2 viewportOffset = ImGui::GetWindowPos();

		m_Data->ViewPortBoundsMin = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_Data->ViewPortBoundsMax = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImGui::Image((void*)(intptr_t)m_Data->Renderer->GetFinalFramebuffer()->GetColorAttachment(0), viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	PromptLoadProject();

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void EditorLayer::PromptLoadProject()
{
	if (m_Data->NoProjectLoaded) {
		if (!ImGui::IsPopupOpen("Load Project"))
			ImGui::OpenPopup("Load Project");

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Load Project", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

			if (ImGui::Button("Load", {60, 0})) {
				std::filesystem::path cwd = std::filesystem::current_path();
				std::filesystem::path path = OpenFileDialog((const char*)cwd.c_str(), "tri");
				LoadProject(path.string());
			}

			ImGui::SameLine();
			if (ImGui::Button("New", { 60, 0 })) {
				auto cwd = std::filesystem::current_path();
				auto path = SaveFileDialog((const char*)cwd.c_str(), "tri");
				NewProject(path.string());
			}
			ImGui::EndPopup();
		}
	}
}

void EditorLayer::RenderPlaybuttons()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	const float size = 24.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));


	Reference<Texture2D>& currentTexture = m_Data->SceneRunning ? m_Data->PauseTexture : m_Data->PlayTexture;

	if (ImGui::ImageButton("play_pause_button", (ImTextureID)currentTexture->GetID(), { size, size })) {
		m_Data->SceneRunning = !m_Data->SceneRunning;
		if (m_Data->SceneRunning)
			StartScene();
		else {
			StopScene();
		}
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);

}


void EditorLayer::UpdateTitleBar()
{
	if (ImGui::BeginMenu("File"))
	{
		// Disabling fullscreen would allow the window to be moved to the front of other windows, 
		// which we can't undo at the moment without finer window depth/z control.
		//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

		if (ImGui::MenuItem("Save project as")) {
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			std::string path = SaveFileDialog((const char*)cwd.c_str(), "tri").string();
			SaveProject(path);
		}

		if (ImGui::MenuItem("Load project")) {
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			std::string path = OpenFileDialog((const char*)cwd.c_str(), "tri").string();
			LoadProject(path);
		}

		if (ImGui::MenuItem("Save scene as")) {
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();

			std::string currentScenePath = m_Data->ActiveScene->MetaData.Filepath;

			std::string startingPath = !(currentScenePath.empty()) ? currentScenePath : cwd.string();

			std::string path = SaveFileDialog((const char*)startingPath.c_str(), "tscn").string();

			SaveScene(path);

		}

		if (ImGui::MenuItem("Load scene")) {
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			std::string path = OpenFileDialog((const char*)cwd.c_str(), "tscn").string();

			LoadScene(ResourceManager::GetIDFromPath(path));
		}

		if (ImGui::MenuItem("Exit")) Application::Get().Close();

		ImGui::EndMenu();

	}

}
