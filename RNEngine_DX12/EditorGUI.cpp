#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	void GUIRenderer::Init(DescriptorHeap* srvHeap) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		ImGui::StyleColorsDark();
		io.Fonts->AddFontFromFileTTF("../Assets/Font/851H-kktt_004.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());


		auto window = Engine::GetWindow();
		io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding = ImVec2(0,0);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

		auto dev = Engine::GetID3D12Device();
		// 3. Platform + Renderer バックエンド初期化
		ImGui_ImplWin32_Init(window->GetHwnd());
		ImGui_ImplDX12_Init(
			dev,
			2,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			srvHeap->GetHeap(),
			srvHeap->GetCPUHandle(),
			srvHeap->GetGPUHandle()
		);
		//3つ分確保しておく
		for (int i = 0; i < 3; i++) {
			srvHeap->AddHeapCount();
		}
		io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

		io.DisplaySize = ImVec2(window->GetWidth(), window->GetHeight());

	}
	void GUIRenderer::UpdateRenderer(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* srvHeap) {
		// 開始
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// 描画
		for (auto& gui : m_Items) {
			gui.second->Draw();
		}

		// 描画コマンド発行
		ImGui::Render();

		cmdList->SetDescriptorHeaps(1, srvHeap->GetHeapAddress());
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
	}
	void GUIRenderer::Destroy() {
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void Inspector::DrawComponentInInspector(shared_ptr<Component>& component) {
		auto fields = component->GetReflection();
		if (ImGui::CollapsingHeader(component->GetComponentName().c_str())) return;
		char* base = reinterpret_cast<char*>(component.get());
		for (auto& field : fields) {
			void* addr = base + field.m_Offset;
			string name = field.m_Name;
			ConvertToAttribute* convert = nullptr;
			for (auto& attribute : field.m_Attribute) {
				if (auto header = dynamic_cast<HeaderAttribute*>(attribute.get())) {
					name = header->m_Header;
				}
				if(auto c = dynamic_cast<ConvertToAttribute*>(attribute.get())){
					convert = c;
				}
			}
			switch (field.m_Type) {
			case FieldInfo::Type::Int:
				DrawIntField(name, reinterpret_cast<int*>(addr), 100, convert);
				break;
			case FieldInfo::Type::Float:
				DrawFloatField(name, reinterpret_cast<float*>(addr), 100, convert);
				break;
			case FieldInfo::Type::Bool:
				DrawBoolField(name, reinterpret_cast<bool*>(addr), convert);
				break;
			case FieldInfo::Type::Vec3:
				DrawVec3Field(name, reinterpret_cast<float*>(addr), 300, convert);
				break;
			}
		}
	}
	void Inspector::DrawIntField(const string& name, int* value, int width, ConvertToAttribute* convert) {
		int display = *value;
		if (convert) {
			convert->m_ConvertToDisplay(value, &display);
		}

		ImGui::Text(name.c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(width);
		if (ImGui::InputScalar(("##" + name).c_str(), ImGuiDataType_S32, value)) {
			if (convert) {
				convert->m_ConvertToInternal(&display, value);
			}
			else {
				value = &display;
			}
		}
	}
	void Inspector::DrawFloatField(const string& name, float* value, int width, ConvertToAttribute* convert) {
		float display = *value;
		if(convert) {
			convert->m_ConvertToDisplay(value, &display);
		}
		ImGui::Text(name.c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(width);
		if (ImGui::InputScalar(("##" + name).c_str(), ImGuiDataType_Float, &display)) {
			if (convert) {
				convert->m_ConvertToInternal(&display, value);
			}
			else {
				value = &display;
			}
		}
	}
	void Inspector::DrawBoolField(const string& name, bool* value, ConvertToAttribute* convert) {
		ImGui::Text(name.c_str());
		ImGui::SameLine();
		ImGui::Checkbox(("##" + name).c_str(), value);
	}
	void Inspector::DrawVec3Field(const string& name, float* value, int width, ConvertToAttribute* convert) {
		float display[3] = { value[0], value[1], value[2] };
		if (convert) {
			convert->m_ConvertToDisplay(value, display);
		}

		ImGui::Text(name.c_str());
		ImGui::SameLine();
		ImGui::SetNextItemWidth(width);
		if (ImGui::DragFloat3(("##" + name).c_str(), display, 0.1f)) {
			if (convert) {
				convert->m_ConvertToInternal(display, value);
			}
			else {
				value[0] = display[0];
				value[1] = display[1];
				value[2] = display[2];
			}
		}
	}


	void Inspector::Draw() {
		GUI::Draw();
		if (auto gameObject = m_GameObject.lock()) {
			auto components = gameObject->GetComponents();
			for (auto& component : components) {
				DrawComponentInInspector(component);
			}
		}
		ImGui::End();
	}

	void Hierarchy::Draw() {
		GUI::Draw();
		if (auto scene = m_GameScene.lock()) {
			auto gameObjects = scene->GetGameObjects();
			for (auto& gameObject : gameObjects) {
				ImGui::PushID(gameObject.get());
				if (ImGui::Button(gameObject->GetName().c_str())) {
					auto renderer = Engine::GetGUIRenderer();
					auto inspector = renderer->GetGui<Inspector>("inspector");
					inspector->SetGameObject(gameObject);
				}
				ImGui::PopID();
			}
		}
		ImGui::End();
	}

	void ProjectView::Init() {
		auto renderer = Engine::GetRenderer();
		auto folderTexBuffer = ResourceManager::GetTextureBuffer("Editor/Texture/folder_icon.png");
		auto fileTexBuffer = ResourceManager::GetTextureBuffer("Editor/Texture/file_icon.png");

		auto folderSrvHandle = renderer->GetSRVDescriptorGPUHandle(folderTexBuffer->GetSRVHandle());
		auto fileSrvHandle = renderer->GetSRVDescriptorGPUHandle(fileTexBuffer->GetSRVHandle());

		m_FolderTexID = (ImTextureID)folderSrvHandle.ptr;
		m_FileTexID = (ImTextureID)fileSrvHandle.ptr;

		m_CurrentFolder = make_shared<Entry>(Entry::Type::Folder, L"origin", L"../");
		auto assetEntry = make_shared<Entry>(Entry::Type::Folder, L"Assets", L"../Assets/");
		assetEntry->m_IconID = m_FolderTexID;
		m_CurrentFolder->AddChild(assetEntry);

		auto sourceEntry = make_shared<Entry>(Entry::Type::Folder, L"Source", L"../RNEngine_DX12/");
		sourceEntry->m_IconID = m_FolderTexID;
		m_CurrentFolder->AddChild(sourceEntry);

		UpdateFoldersData();
	}
	void ProjectView::UpdateFoldersData() {
		UpdateFolderData(m_CurrentFolder->m_Children[0], File::GetDirectoryFile(L"../Assets/"));
		UpdateFolderData(m_CurrentFolder->m_Children[1], File::GetDirectoryFile(L"../RNEngine_DX12/"));		
	}
	void ProjectView::UpdateFolderData(shared_ptr<Entry>& parent, vector<filesystem::directory_entry> directories) {
		for (auto& directory : directories) {
			Entry::Type entryType = directory.is_directory() ? Entry::Type::Folder : Entry::Type::File;
			auto entry = make_shared<Entry>(entryType, directory.path().filename().wstring(),directory.path().wstring());
			if (entryType == Entry::Type::Folder) {
				UpdateFolderData(entry, File::GetDirectoryFile(directory.path().wstring()));
				entry->m_IconID = m_FolderTexID;
			}
			else {
				entry->m_IconID = m_FileTexID;
			}
			parent->AddChild(entry);
		}
		
	}
	void ProjectView::DrawProjectFolder(shared_ptr<Entry>& entry) {
		ImVec2 currentPosition = ImGui::GetCursorPos();
		float windowVisibleX = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		for (auto& entry : m_CurrentFolder->m_Children) {
			if (!entry) continue;
			ImGui::PushID(entry.get());

			float x = ImGui::GetItemRectMax().x;
			x += m_IconSize * 1.5f;
			if (x > windowVisibleX) {
				ImGui::NewLine();
				currentPosition = ImGui::GetCursorPos();
			}

			bool selected = (m_SelectedFolderAddr == entry.get());
			if (selected) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 1.0f, 0.3f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 1.0f, 0.0f));
			}
			ImGui::SetCursorPos(currentPosition);
			if (ImGui::Button("##iconButton", ImVec2(m_IconSize, m_IconSize))) {
				/*if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					
				}
				else {
					m_SelectedFolderAddr = entry.get();
				}*/
				if (entry->m_Type == Entry::Type::Folder) {
					MoveFolderChildren(entry);
					ImGui::PopStyleColor();
					ImGui::PopID();
					return;
				}
				else {
					auto renderer = Engine::GetGUIRenderer();
					auto inspector = renderer->GetGui<Inspector>("inspector");
					auto object = inspector->GetCurrentGameObject();
					ComponentRegistry::AddComponent(ConvertWstrToStr(entry->m_Name), object);
					/*cout << "Open File:" << ConvertWstrToStr(entry->m_Path).c_str() << endl;
					ShellExecuteA(NULL, "open", ConvertWstrToStr(entry->m_Path).c_str(), NULL, NULL, SW_SHOWNORMAL);*/
				}
			}
			
			ImGui::SetCursorPos(currentPosition);
			ImGui::Image(entry->m_IconID, ImVec2(m_IconSize, m_IconSize));
			ImGui::SetCursorPosX(currentPosition.x);
			string text = ConvertWstrToStr(entry->m_Name).c_str();
			if (text.size() > 20) {
				text = text.substr(0, 15);
				text += "...";
			}
			ImGui::PushTextWrapPos(currentPosition.x + m_IconSize);
			ImGui::TextWrapped(text.c_str());
			ImGui::PopTextWrapPos();

			ImGui::PopStyleColor();
			ImGui::PopID();
			currentPosition.x += m_IconSize * 1.5f;
		}
	}
	void ProjectView::DrawBackButton(shared_ptr<Entry>& entry) {
		ImVec2 currentPosition = ImGui::GetCursorPos();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 1.0f, 1.0f));
		if (ImGui::Button("<- Back", ImVec2(64, 16))) {
			MoveFolderParent();
			ImGui::PopStyleColor();
			return;
		}
		ImGui::PopStyleColor();
	}

	void ProjectView::MoveFolderChildren(shared_ptr<Entry>& entry) {
		m_CurrentFolderPath.push_back(m_CurrentFolder);
		m_CurrentFolder = entry;
	}
	void ProjectView::MoveFolderParent() {
		if (auto parent = m_CurrentFolder->m_Parent.lock()) {
			m_CurrentFolder = parent;
			m_CurrentFolderPath.pop_back();
		}
	}
	void ProjectView::Draw() {
		GUI::Draw();
		DrawBackButton(m_CurrentFolder);
		ImGui::Separator();
		DrawProjectFolder(m_CurrentFolder);
		ImGui::End();
	}



	vector<LogData> DebugLog::g_LogData = {};
	void DebugLog::Log(const string& log, LogData::Type type) {
		g_LogData.push_back({ log,type });
	}
	void DebugLog::Clear() {
		g_LogData.clear();
	}
	void DebugLog::Draw() {
		GUI::Draw();
		if (ImGui::Button("clear", ImVec2(64, 16))) {
			Clear();
		}
		ImGui::Separator();
		m_BeforeScrollY = ImGui::GetScrollY() / ImGui::GetScrollMaxY();
		string name = "Debug: ";
		for (auto& log : g_LogData) {
			switch (log.m_Type) {
			case LogData::Type::Debug:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			case LogData::Type::Warning:
				name = "Warning: ";
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
				break;
			case LogData::Type::Error:
				name = "Error: ";
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				break;
			}
			ImGui::TextWrapped((name + log.m_Log).c_str());
			ImGui::PopStyleColor();
		}
		if (m_BeforeScrollY == 1.0f) {
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::End();
	}

	void GameView::CreateSRV(shared_ptr<RenderTarget>& renderTarget) {
		m_RenderTarget = renderTarget;
	}
	void GameView::Draw() {
		GUI::Draw();
		auto renderer = Engine::GetRenderer();
		auto texture = m_RenderTarget->GetRenderTargetTexture();
		ImTextureID id = (ImTextureID)renderer->GetSRVDescriptorGPUHandle(texture->GetSRVHandle()).ptr;
		
		ImGui::Image(id, ImGui::GetWindowSize());
		ImGui::End();
	}
}