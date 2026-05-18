#pragma once
#include "stdafx.h"
#include "RendererComponent.h"

namespace RNEngine
{
	class DescriptorHeap;
	class TextureResource;
	class TextureBuffer;
	class GameObject;
	
	enum class Anchor {
		   TopLeft,    Top,    TopRight,
		      Left, Center,       Right,
		ButtomLeft, Buttom, ButtomRight
	};
	class ImageRenderer : public RendererComponent {
		shared_ptr<TextureResource> m_Texture;
		Vector2 m_Pivot;
	public:
       /// <summary>
		/// コンストラクタ
		/// </summary>
		ImageRenderer(const shared_ptr<GameObject>& ptr);
		/// <summary>
		/// デストラクタ
		/// </summary>
		~ImageRenderer();
		/// <summary>
		/// 画像ファイルを指定してテクスチャを設定する。
		/// </summary>
		/// <param name="filename">テクスチャファイル名（ワイド文字列）</param>
		void SetTexture(const wstring& filename);
		/// <summary>
		/// 既存のテクスチャバッファを設定する。
		/// </summary>
		/// <param name="texture">テクスチャバッファの shared_ptr</param>
		void SetTexture(const shared_ptr<TextureBuffer>& texture);

		/// <summary>
		/// 設定されているテクスチャリソースを取得する。
		/// </summary>
		/// <returns>TextureResource の生ポインタ</returns>
		TextureResource* GetTexture()const;
		/// <summary>
		/// 毎フレームの更新処理
		/// </summary>
		virtual void Update()override;
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="cmdList">コマンドリスト</param>
		/// <param name="heap">ディスクリプタヒープ</param>
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;

		/// <summary>
		/// ピボットを直接設定する。
		/// </summary>
		/// <param name="pivot">ピボット位置（0..1）</param>
		void SetPivot(const Vector2& pivot);
		/// <summary>
		/// 既定のアンカーからピボットを設定する。
		/// </summary>
		/// <param name="anchor">アンカー位置</param>
		void SetPivot(Anchor anchor);
		/// <summary>
		/// 現在のピボットを取得する。
		/// </summary>
		/// <returns>ピボット座標を返します。</returns>
		Vector2 GetPivot();

		/// <summary>
		/// 指定したアンカーの正規化座標を取得する。
		/// </summary>
		/// <param name="anchor">アンカー位置</param>
		/// <returns>アンカーの正規化座標を返します。</returns>
		Vector2 GetAnchorNormalize(Anchor anchor);
	};
}