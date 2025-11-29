#pragma once
#include "stdafx.h"
#include "Reflection.h"
namespace RNEngine {
	class DescriptorHeap;
	class GUI;

	class GameObject;
	class Component;
	class Scene;

	class GUIRenderer {
		unique_ptr<DescriptorHeap> m_SrvDescriptorHeap;
		unordered_map<string, shared_ptr<GUI>> m_Items;
	public:
		GUIRenderer() {}
		~GUIRenderer() {}

		void Init(DescriptorHeap* srvHeap);
		void UpdateRenderer(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* srvHeap);
		void Destroy();

		void AddGui(const string& name,const shared_ptr<GUI>& gui) {
			m_Items[name] = gui;
		}

		template<typename T>
		shared_ptr<T> GetGui(const string& name) {
			auto it = m_Items.find(name);
			if (it != m_Items.end()) {
				return dynamic_pointer_cast<T>((*it).second);
			}
			return nullptr;
		}
	};

	class GUI {
		string m_WindowName;
	public:
		GUI(const string& windowName):m_WindowName(windowName){}
		~GUI(){}

		virtual void Draw() {
			ImGui::Begin(m_WindowName.c_str());
		}
	};

	class Inspector : public GUI {
		weak_ptr<GameObject> m_GameObject;

		void DrawComponentInInspector(shared_ptr<Component>& component);
	public:
		Inspector(const string& windowName):GUI(windowName){}
		~Inspector(){}

		virtual void Draw()override;

		void SetGameObject(const shared_ptr<GameObject>& gameObject) {
			m_GameObject = gameObject;
		}
		shared_ptr<GameObject> GetCurrentGameObject() {
			return m_GameObject.lock();
		}
	};
	class Hierarchy : public GUI {
		weak_ptr<Scene> m_GameScene;
	public:
		Hierarchy(const string& windowName) :GUI(windowName) {}
		~Hierarchy() {}

		virtual void Draw()override;

		void SetScene(const shared_ptr<Scene>& scene) {
			m_GameScene = scene;
		}
	};
	struct Entry : public enable_shared_from_this<Entry>{
		enum class Type{
			File,Folder
		}m_Type;
		wstring m_Name;
		wstring m_Path;
		weak_ptr<Entry> m_Parent;
		vector<shared_ptr<Entry>> m_Children;

		ImTextureID m_IconID;
		Entry(Entry::Type type, const wstring& name,const wstring& path) :m_Type(type), m_Name(name),m_Path(path), m_IconID{} {}

		void AddChild(shared_ptr<Entry> entry) {
			if (m_Type != Type::Folder) return;
			if (find(m_Children.begin(), m_Children.end(), entry) != m_Children.end()) return;
			m_Children.push_back(entry);
			entry->m_Parent = shared_from_this();
		}
	};
	class ProjectView : public GUI {
		shared_ptr<Entry> m_CurrentFolder;
		vector<shared_ptr<Entry>> m_CurrentFolderPath;
		Entry* m_SelectedFolderAddr;
		float m_IconSize;
		ImTextureID m_FolderTexID;
		ImTextureID m_FileTexID;

		void DrawProjectFolder(shared_ptr<Entry>& entry);
		void DrawBackButton(shared_ptr<Entry>& entry);
		void UpdateFoldersData();
		void UpdateFolderData(shared_ptr<Entry>& parent, vector<filesystem::directory_entry> directories);

		void MoveFolderChildren(shared_ptr<Entry>& entry);
		void MoveFolderParent();
	public:
		ProjectView(const string& windowName, float iconSize) :
			GUI(windowName), m_IconSize(iconSize),
			m_FolderTexID{}, m_FileTexID{}, m_SelectedFolderAddr{} {
		}
		~ProjectView() {}

		void Init();
		virtual void Draw()override;
	};

	struct LogData {
		string m_Log;
		enum class Type {
			Debug,Warning,Error
		}m_Type;
	};
	class DebugLog : public GUI {
		static vector<LogData> g_LogData;

		float m_BeforeScrollY;
	public:
		static void Log(const string& log, LogData::Type type = LogData::Type::Debug);
		static void Clear();

		DebugLog(const string& windowName) :GUI(windowName), m_BeforeScrollY(1.0f){}
		~DebugLog() {}


		virtual void Draw()override;
	};

	class SRVBuffer;
	class RTVBuffer;
	class GameView : public GUI {
		shared_ptr<RenderTarget> m_RenderTarget;
	public:
		GameView(const string& windowName) :GUI(windowName){}
		~GameView() {}

		virtual void Draw()override;

		void CreateSRV(shared_ptr<RenderTarget>& renderTarget);
	};
}

