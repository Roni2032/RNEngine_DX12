#pragma once
#include "stdafx.h"
#include "Component.h"

namespace RNEngine
{
	class Camera;
	class GameObject;
	class DescriptorHeap;

	class RendererComponent : public Component{
	protected:
		weak_ptr<Camera> m_TargetCamera;
		vector<string> m_RenderTargetTag;
	public:
       /// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="ptr">オーナー GameObject の共有ポインタ</param>
		RendererComponent(const shared_ptr<GameObject>& ptr);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~RendererComponent(){}

		/// <summary>
		/// カメラを指定して初期化する。
		/// </summary>
		/// <param name="camera">このレンダラーが描画対象とするカメラの共有ポインタ</param>
		void Init(const shared_ptr<Camera>& camera);

		/// <summary>
		/// 毎フレームの更新処理（必要に応じてオーバーライド）
		/// </summary>
		virtual void Update()override{}
		/// <summary>
		/// 描画処理（コマンドリストとディスクリプタヒープを受け取る）。
		/// </summary>
		/// <param name="cmdList">描画に使用するコマンドリスト</param>
		/// <param name="heap">使用するディスクリプタヒープ</param>
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap){}

		/// <summary>
		/// レンダーターゲットのタグを追加する。
		/// </summary>
		/// <param name="tag">追加するタグ文字列</param>
		void AddRenderTargetTag(const string& tag) {
			m_RenderTargetTag.push_back(tag);
		}
		/// <summary>
		/// 登録されているレンダーターゲットタグの一覧を取得する。
		/// </summary>
		/// <returns>タグ文字列の配列を返します。</returns>
		vector<string> GetRenderTargetTag()const { return m_RenderTargetTag; }
	};
}