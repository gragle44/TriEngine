#include "EditorApp.h"

#include <nfd.h>
#include <filesystem>

class TestScript : public Script {
public:
	TestScript()
	{

	}

	void OnStop() final {
	}

	void OnUpdate(float deltaTime) final {
		m_TimeElapsed += deltaTime;

		if (m_TimeElapsed > 5.0f)
			GetScene()->Reset();
	}

	virtual void OnCollisionStart(GameObject collider) final {
	}

	virtual void OnCollisionEnd(GameObject collider) final {
	}
private:
	float m_TimeElapsed = 0.0f;
};


class NewScript : public Script {
public:
	void OnStart() final {
		TRI_TRACE("created");
	}

	void OnUpdate(float deltaTime) final {
		TRI_TRACE("NewScript");

		if (Random::Int(0, 600) < 1) {
			auto scene = GetScene();
			scene->Reset();
		}
	}
};

class BirdScript : public Script {
public:
	void OnStart() final {
	}

	void OnUpdate(float deltaTime) final {
		auto& rigidBody = GetComponent<RigidBody2DComponent>();
		auto& transform = GetComponent<Transform2DComponent>();

		if (Input::IsKeyPressed(TRI_KEY_SPACE) || Input::IsGamepadButtonPressed(TRI_JOYSTICK_1,TRI_GAMEPAD_BUTTON_A)) {
			m_UpVelocity += 200.0f;
			m_RotationVelocity = 575.0f;

		}
		else
			m_RotationVelocity = -175.0f;

		m_UpVelocity -= 45.0f;

		if (m_UpVelocity > 700.0f)
			m_UpVelocity = 700.0f;
		else if (m_UpVelocity < -1000.0f)
			m_UpVelocity = -1000.0f;

		if (transform.Rotation > 45.0f && m_RotationVelocity > 0.0f)
			m_RotationVelocity = 0.0f;
		else if (transform.Rotation < -90.0f && m_RotationVelocity < 0.0f)
			m_RotationVelocity = 0.0f;

		rigidBody.SetVelocity({ 0.0f, m_UpVelocity * deltaTime });
		rigidBody.SetAngularVelocity(m_RotationVelocity * deltaTime);

	}

	virtual void OnCollisionStart(GameObject collider) final {
		auto scene = GetScene();
		scene->Reset();
	}
private:
	float m_UpVelocity = 0.0f;
	float m_RotationVelocity = 0.0f;
};

class BackgroundScript : public Script {
public:
	void OnStart() final {
		for (auto [id, object] : GetScene()->GetAllObjects()) {
			if (object.GetComponent<TagComponent>().Tag == "Pillar") {
				m_Pillars.emplace_back(object);
			}
		}

		for (int i = 0; i < 8; i += 2) {
			float displacement = Random::Float(-1.25f, 1.25f);

			GameObject object1 = m_Pillars[i];
			object1.GetComponent<Transform2DComponent>().Position.y += displacement;

			GameObject object2 = m_Pillars[i+1];
			object2.GetComponent<Transform2DComponent>().Position.y += displacement;

		}

	}

	void OnUpdate(float deltaTime) final {
		for (int i = 0; i < 8; i++) {
			auto pillar = m_Pillars[i];

			auto& body = pillar.GetComponent<RigidBody2DComponent>();
			auto& transform = pillar.GetComponent<Transform2DComponent>();

			if (transform.Position.x < -15.6f) {
				auto& transform = pillar.GetComponent<Transform2DComponent>();
				auto& body = pillar.GetComponent<RigidBody2DComponent>();
				body.SetPosition({ transform.Position.x + 40.0f, transform.Position.y });
			}
			else
				body.SetPosition({ transform.Position.x -= m_MoveSpeed * deltaTime, transform.Position.y });
		}
	}
private:
	const float m_MoveSpeed = 7.0f;
	std::vector<GameObject> m_Pillars;
};

EditorLayer::EditorLayer()
{
	ScriptRegistry::Register<TestScript>();
	ScriptRegistry::Register<NewScript>();
	ScriptRegistry::Register<BirdScript>();
	ScriptRegistry::Register<BackgroundScript>();
}

void EditorLayer::SetupImGuiStyle()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/droidsans/DroidSans.ttf", 18.0f);
	TRI_CORE_ASSERT(io.FontDefault != NULL, "Default TriEditor font was null!");

	SetFont<FontType::Bold>(io.Fonts->AddFontFromFileTTF("assets/fonts/droidsans/DroidSans-Bold.ttf", 18.0f));

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
	style.TabMinWidthForCloseButton = 0.0f;
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
	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });

	m_ViewPortSize = { 1280, 720 };

	m_EditorScene = Scene::Create();
	m_ActiveScene = m_EditorScene;
	m_SceneModule.SetScene(m_EditorScene);

	m_Camera = std::make_shared<EditorCamera>();
	m_EditorScene->SetEditorCamera(m_Camera);

	TextureSettings playbuttonSettings;
	playbuttonSettings.Filter = TextureFilter::Linear;

	m_PlayTexture = Texture2D::Create("assets/playbutton.png", playbuttonSettings);
	m_PauseTexture = Texture2D::Create("assets/pausebutton.png", playbuttonSettings);

	SetupImGuiStyle();

}

void EditorLayer::OnDetach()
{
	m_ActiveScene->Stop();
}

void EditorLayer::OnUpdate(float deltaTime)
{
	m_DebugModule.OnUpdate(deltaTime);
	if (m_PrevViewPortSize.x != m_ViewPortSize.x || m_PrevViewPortSize.y != m_ViewPortSize.y && m_ViewPortSize.x > 0 && m_ViewPortSize.y > 0) {
		m_PrevViewPortSize = m_ViewPortSize;

		if (m_SceneRunning) {
			m_ActiveScene->OnViewportResized((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_ActiveScene->OnUpdate(deltaTime);
		}
		else {
			m_EditorScene->OnViewportResized((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_EditorScene->OnEditorUpdate(deltaTime);
		}
	}
	else if (m_SceneRunning)
		m_ActiveScene->OnUpdate(deltaTime);

	else if (!m_SceneRunning)
		m_EditorScene->OnEditorUpdate(deltaTime);
}

void EditorLayer::OnEvent(Event& e)
{
	m_Camera->OnEvent(e);
	if (m_SceneRunning)
		m_ActiveScene->OnEvent(e);
	else
		m_EditorScene->OnEvent(e);
}

void EditorLayer::StartScene()
{
	m_SceneRunning = true;
	m_ActiveScene = m_EditorScene->Copy();
	m_ActiveScene->Start();
}

void EditorLayer::StopScene()
{
	m_SceneRunning = false;
	m_ActiveScene->Stop();
}

void EditorLayer::LoadEmptyScene()
{
	StopScene();
	m_EditorScene = Scene::Create();
	m_EditorScene->SetEditorCamera(m_Camera);
	m_ActiveScene = m_EditorScene;
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
	if (!projectData.StartupScene.empty()) {
		std::filesystem::path fullStartupScenePath = TriEngine::ProjectManager::GetCurrent()->GetAbsolutePath(projectData.StartupScene.string());

		LoadScene(fullStartupScenePath.string());
	}
	else {
		LoadEmptyScene();
	}
}

void EditorLayer::SaveProject(const std::string& path)
{
	ProjectManager::Save(path);
}

void EditorLayer::LoadScene(const std::string& path)
{
	StopScene();
	TriEngine::SceneSerializer s(m_EditorScene);
	s.Deserialize(path);
}

void EditorLayer::SaveScene(const std::string& path)
{
	TriEngine::SceneSerializer s(m_EditorScene);
	s.Serialize(path);
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
	if(ImGui::Begin("Viewport")) {

		//TODO: allow modules to pause/unpause the viewport if needed
		m_SceneViewPaused = false;

		/*
		if (ImGui::IsWindowHovered()) {
			m_SceneViewPaused = false;
		}
		else {
			m_SceneViewPaused = true;
		}
		*/

		ImGui::CaptureMouseFromApp(m_SceneViewPaused);
		ImGui::CaptureKeyboardFromApp(m_SceneViewPaused);

		//Consider moving to scene
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_ViewPortSize = { (uint32_t)viewportSize.x, (uint32_t)viewportSize.y };

		if (m_SceneRunning)
			m_ActiveScene->OnEditorRender();
		else
			m_EditorScene->OnEditorRender();

	}

	PromptLoadProject();

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void EditorLayer::PromptLoadProject()
{
	if (m_NoProjectLoaded) {
		if (!ImGui::IsPopupOpen("Load Project"))
			ImGui::OpenPopup("Load Project");

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("Load Project", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {

			if (ImGui::Button("Load", {60, 0})) {
				nfdchar_t* output;
				auto cwd = std::filesystem::current_path();
				nfdresult_t result = NFD_OpenDialog("tri", cwd.string().c_str(), &output);

				if (result == NFD_OKAY) {
					LoadProject(output);
					m_NoProjectLoaded = false;
					delete output;
				}

				else if (result == NFD_CANCEL) {
					TRI_CORE_TRACE("Canceled file dialog");
				}

				else if (result == NFD_ERROR) {
					TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
				}

			}

			ImGui::SameLine();
			if (ImGui::Button("New", { 60, 0 })) {
				ProjectManager::CreateNew();
				LoadEmptyScene();
				m_NoProjectLoaded = false;
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


	Reference<Texture2D>& currentTexture = m_SceneRunning ? m_PauseTexture : m_PlayTexture;

	if (ImGui::ImageButton((ImTextureID)currentTexture->GetID(), { size, size })) {
		m_SceneRunning = !m_SceneRunning;
		if (m_SceneRunning)
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
			char* output;
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			nfdresult_t result = NFD_SaveDialog("tri", cwd.string().c_str(), &output);

			if (result == NFD_OKAY) {
				std::string path(output);
				if (!path.ends_with(".tri"))
					path.append(".tri");

				SaveProject(path);
				delete output;
			}

			else if (result == NFD_CANCEL) {
				TRI_CORE_TRACE("Canceled file dialog");
			}

			else if (result == NFD_ERROR) {
				TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
			}
		}

		if (ImGui::MenuItem("Load project")) {
			nfdchar_t* output;
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			nfdresult_t result = NFD_OpenDialog("tri", cwd.string().c_str(), &output);

			if (result == NFD_OKAY) {
				LoadProject(output);
				delete output;
			}

			else if (result == NFD_CANCEL) {
				TRI_CORE_TRACE("Canceled file dialog");
			}

			else if (result == NFD_ERROR) {
				TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
			}

		}

		if (ImGui::MenuItem("Save scene as")) {
			char* output;
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			nfdresult_t result = NFD_SaveDialog("tscn", cwd.string().c_str(), &output);


			if (result == NFD_OKAY) {
				std::string path(output);
				if (!path.ends_with(".tscn"))
					path.append(".tscn");

				SaveScene(path);
				delete output;
			}

			else if (result == NFD_CANCEL) {
				TRI_CORE_TRACE("Canceled file dialog");
			}

			else if (result == NFD_ERROR) {
				TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
			}
		}

		if (ImGui::MenuItem("Load scene")) {
			nfdchar_t* output;
			auto cwd = ProjectManager::GetCurrent()->GetWorkingDirectory();
			nfdresult_t result = NFD_OpenDialog("tscn", cwd.string().c_str(), &output);

			if (result == NFD_OKAY) {
				LoadScene(output);
				delete output;
			}

			else if (result == NFD_CANCEL) {
				TRI_CORE_TRACE("Canceled file dialog");
			}

			else if (result == NFD_ERROR) {
				TRI_CORE_ERROR("Error opening file dialog: {0}", NFD_GetError());
			}
		}

		if (ImGui::MenuItem("Exit")) Application::Get().Close();

		ImGui::EndMenu();

	}

}
