#pragma once
#include "stdafx.h"
#include "Object.h"
namespace RNEngine {
	class Component;
	class Scene;
	class RendererComponent;
	class Transform;

	class GameObject : public Object {
		wstring m_Name;
		weak_ptr<Scene> m_OwnerScene;
		shared_ptr<Transform> m_Transform;

		vector<shared_ptr<Component>> m_Components;
		vector<string> m_Tags;
		int m_Layer;
	public:
       /// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="ptr">このオブジェクトが所属するシーンの共有ポインタ</param>
		GameObject(const shared_ptr<Scene>& ptr);
		/// <summary>
		/// 名前を指定したコンストラクタ
		/// </summary>
		/// <param name="ptr">オーナーシーンの共有ポインタ</param>
		/// <param name="name">オブジェクト名</param>
		GameObject(const shared_ptr<Scene>& ptr, const wstring& name);
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~GameObject();

		/// <summary>
		/// ゲームオブジェクトの開始処理
		/// </summary>
		virtual void Start()override;
		/// <summary>
		/// 毎フレームの更新処理
		/// </summary>
		virtual void Update()override;
		/// <summary>
		/// フレーム末の更新処理
		/// </summary>
		virtual void LastUpdate()override;
		/// <summary>
		/// 描画処理
		/// </summary>
		virtual void Draw()override;

    /// <summary>
		/// オブジェクト名を設定する。
		/// </summary>
		/// <param name="name">設定する名前</param>
		void SetName(const wstring& name) {
			m_Name = name;
		}
		/// <summary>
		/// オブジェクト名を取得する。
		/// </summary>
		/// <returns>オブジェクト名を返します。</returns>
		wstring GetName() {
			return m_Name;
		}
        /// <summary>
		/// タグを追加する。
		/// </summary>
		/// <param name="tag">追加するタグ文字列</param>
		void AddTag(const string& tag) {
			m_Tags.push_back(tag);
		}
		/// <summary>
		/// 指定したタグが存在するか判定する。
		/// </summary>
		/// <param name="tag">判定するタグ</param>
		/// <returns>存在すれば true を返します。</returns>
		bool HasTag(const string& tag) {
			return find(m_Tags.begin(), m_Tags.end(), tag) != m_Tags.end();
		}
		/// <summary>
		/// 複数タグのうちいずれかを持っているか判定する。
		/// </summary>
		/// <param name="tags">判定するタグの配列</param>
		/// <returns>いずれかのタグを持っていれば true を返します。</returns>
		bool HasTags(const vector<string>& tags) {
			for (auto& tag : tags) {
				if (HasTag(tag)) return true;
			}
			return false;
		}
		/// <summary>
		/// 指定したタグを削除する。
		/// </summary>
		/// <param name="tag">削除するタグ</param>
		void RemoveTag(const string& tag) {
			auto it = find(m_Tags.begin(), m_Tags.end(), tag);
			if (it != m_Tags.end()) {
				m_Tags.erase(it);
			}
		}
		/// <summary>
		/// レイヤーを整数で設定する。
		/// </summary>
		/// <param name="layer">設定するレイヤー番号</param>
		void SetLayer(int layer) {
			m_Layer = layer;
		}
		/// <summary>
		/// レイヤーを名前から設定する（定義により解決される）。
		/// </summary>
		/// <param name="layerName">レイヤー名</param>
		void SetLayer(const wstring& layerName);

		int GetLayer()const {
			return m_Layer;
		}
		bool CompareLayer(int layer)const {
			return m_Layer == layer;
		}
		bool CompareLayer(const vector<int>& layers) {
			for (auto& layer : layers) {
				if (CompareLayer(layer)) return true;
			}
			return false;
		}

     /// <summary>
		/// 所属するシーンの共有ポインタを取得する。
		/// </summary>
		/// <returns>オーナーシーンの shared_ptr を返します。</returns>
		shared_ptr<Scene> GetOwnerScene()const;
		/// <summary>
		/// Transform コンポーネントを取得する。
		/// </summary>
		/// <returns>Transform の shared_ptr を返します。</returns>
		shared_ptr<Transform> GetTransform()const;

        /// <summary>
		/// 指定した型のコンポーネントをこのオブジェクトに追加する。
		/// </summary>
		/// <returns>追加されたコンポーネントの shared_ptr を返します。</returns>
		template<class T,class... Param>
		shared_ptr<T> AddComponent(Param&&... params) {
			shared_ptr<T> ptr = make_shared<T>(GetThis<GameObject>(),params...);
			ptr->Start();
			m_Components.push_back(ptr);
			return ptr;
		}
       /// <summary>
		/// 指定した型のコンポーネントを取得する。
		/// </summary>
		/// <returns>見つかった場合は shared_ptr を返し、存在しなければ nullptr を返します。</returns>
		template<class T>
		shared_ptr<T> GetComponent() {
			auto it = m_Components.begin();
			for (; it != m_Components.end(); it++) {
				auto ptr = dynamic_pointer_cast<T>(*it);
				if (ptr) {
					return ptr;
				}
			}
			return nullptr;
		}
       /// <summary>
		/// 指定型のコンポーネントを削除する。
		/// </summary>
		template<class T>
		void RemoveComponent() {
			auto it = m_Components.begin();
			for (; it != m_Components.end(); it++) {
				if (dynamic_pointer_cast<T>(*it)) {
					m_Components.erase(it);
					break;
				}
			}
		}
     /// <summary>
		/// アタッチされているすべてのコンポーネントを取得する。
		/// </summary>
		/// <returns>コンポーネントの配列を返します。</returns>
		vector<shared_ptr<Component>> GetComponents() {
			return m_Components;
		}

		vector<shared_ptr<RendererComponent>> GetRendererComponent();
	};
}
