#pragma once
#include "stdafx.h"

namespace RNEngine {
	class TextureResource;
	class RTVBuffer;
	class DSVBuffer;

	class RenderTarget {
		shared_ptr<TextureResource> m_RenderTargetTexture;
		unique_ptr<RTVBuffer> m_Rtv;
		unique_ptr<DSVBuffer> m_Dsv;

		float m_Width;
		float m_Height;

		DXGI_FORMAT m_Format{};
		array<float, 4> m_ClearColor;
	public:
     /// <summary>
		/// コンストラクタ
		/// </summary>
		RenderTarget();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~RenderTarget();
		/// <summary>
		/// レンダーターゲットを作成する。
		/// </summary>
		/// <param name="renderSize">レンダーサイズ（幅, 高さ）</param>
		/// <param name="format">使用するピクセルフォーマット</param>
		/// <param name="clearColor">クリアカラー（RGBA）</param>
		void Create(Vector2 renderSize, DXGI_FORMAT format, array<float, 4> clearColor = { 1.0f,1.0f,1.0f,0.0f });

		/// <summary>
		/// 描画開始処理（レンダーターゲットへの描画を始める）。
		/// </summary>
		/// <param name="cmdList">コマンドリストのポインタ</param>
		void DrawBegin(ID3D12GraphicsCommandList* cmdList);
		/// <summary>
		/// 描画終了処理（レンダーターゲットへの描画を終了する）。
		/// </summary>
		/// <param name="cmdList">コマンドリストのポインタ</param>
		void DrawEnd(ID3D12GraphicsCommandList* cmdList);
		/// <summary>
		/// 指定されたレンダラー群を用いてレンダーターゲットへ描画を行う。
		/// </summary>
		/// <param name="renderers">描画対象のレンダラーコンポーネントの配列</param>
		void Draw(vector<shared_ptr<RendererComponent>>& renderers);

		/// <summary>
		/// レンダーターゲットのクリアカラーを設定する。
		/// </summary>
		/// <param name="color">設定するカラー値</param>
		void SetClearColor(const Color& color);

		/// <summary>
		/// 内部の RTV バッファを取得する。
		/// </summary>
		/// <returns>RTVBuffer の生ポインタを返します（所有権は渡しません）。</returns>
		RTVBuffer* GetRTVBuffer() { return m_Rtv.get(); }

		/// <summary>
		/// レンダーターゲット用のテクスチャリソースを取得する。
		/// </summary>
		/// <returns>レンダーターゲットのテクスチャを shared_ptr で返します。</returns>
		shared_ptr<TextureResource> GetRenderTargetTexture();
	};
}


