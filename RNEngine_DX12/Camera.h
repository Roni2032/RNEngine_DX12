#pragma once
#include "Component.h"

namespace RNEngine {
	class RendererComponent;
	class RenderTarget;

	class Camera : public Component {
       // カメラの注視点（カメラが向いている位置）
		Vector3 m_Target;
       // カメラの上方向（ワールド上での上向きベクトル）
		Vector3 m_Up;

        // ビュー行列と射影行列
		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;

        // 画面幅（ピクセルまたは仮想幅。射影設定に利用）
		float m_Width;
		// 画面高さ（ピクセルまたは仮想高さ。射影設定に利用）
		float m_Height;

		// 正射影（orthographic）かどうかのフラグ
		bool m_IsOrthographic;

		// カメラが描画対象とするレイヤーのインデックスリスト
		vector<int> m_RenderingLayers;
        // カメラに映すレンダラーコンポーネントのリスト
		vector<shared_ptr<RendererComponent>> m_RendererObjects;
		// レンダリングターゲット（描画先）
		shared_ptr<RenderTarget> m_RenderTarget;

       /// <summary>
		/// 内部でビュー行列を更新する（位置・注視点・上方向から計算）。
		///
		/// ビュー行列は内部の eye/target/up 情報から計算され、
		/// 描画に使用される行列を更新します。
		/// </summary>
		void UpdateViewMatrix();
	public:
		/// <summary>
		/// カメラコンポーネントのコンストラクタ。
		///
		/// オーナーとなる GameObject の共有ポインタを受け取り、
		/// カメラを初期化します。
		/// </summary>
		/// <param name="ptr">オーナーとなる GameObject の共有ポインタ</param>
		Camera(const shared_ptr<GameObject>& ptr);
        /// <summary>
		/// カメラコンポーネントのデストラクタ。
		///
		/// 必要に応じて内部リソースを解放します。
		/// </summary>
		virtual ~Camera();

		/// <summary>
		/// コンポーネント開始時に呼ばれる初期化処理。
		///
		/// シーン開始やオブジェクト生成直後に必要な初期化を行います。
		/// </summary>
		virtual void Start()override;
		/// <summary>
		/// 毎フレームの最後に呼ばれる更新処理。
		///
		/// 描画対象の整理やレンダラーリストの更新など、フレーム末処理を行います。
		/// </summary>
		virtual void LastUpdate()override;

        /// <summary>
		/// 現在設定されているレンダリングターゲットに対して描画処理を行う。
		///
		/// 登録されたレンダラーコンポーネントを使ってレンダリングを実行します。
		/// </summary>
		void DrawRenderTarget();

		/// <summary>
		/// カメラの注視点を設定する。
		///
		/// この値はビュー行列の計算に使用されます。
		/// </summary>
		/// <param name="target">注視点のワールド座標</param>
		void SetTarget(const Vector3& target);
		/// <summary>
		/// カメラの位置（視点）を設定する。
		///
		/// この値はビュー行列の計算に使用されます。
		/// </summary>
		/// <param name="eye">視点のワールド座標</param>
		void SetEye(const Vector3& eye);
		/// <summary>
		/// カメラの上方向ベクトルを設定する。
		///
		/// 通常は (0,1,0) のようなワールド上の上向きベクトルを指定します。
		/// </summary>
		/// <param name="up">上方向を表すベクトル</param>
		void SetUp(const Vector3& up);

		/// <summary>
		/// 現在の注視点を取得する。
		/// </summary>
		/// <returns>注視点のワールド座標を返します。</returns>
		Vector3 GetTarget()const;
        /// <summary>
		/// 現在の視点（カメラ位置）を取得する。
		/// </summary>
		/// <returns>視点のワールド座標を返します。</returns>
		Vector3 GetEye()const;
		/// <summary>
		/// 現在の上方向ベクトルを取得する。
		/// </summary>
		/// <returns>上方向を表すベクトルを返します。</returns>
		Vector3 GetUp()const;
		/// <summary>
		/// カメラが向いている方向ベクトル（正規化済み）を取得する。
		/// </summary>
		/// <returns>カメラの前向き方向（正規化済み）を返します。</returns>
		Vector3 GetDirection()const;

		/// <summary>
		/// 明示的にビュー行列を設定する。
		///
		/// 指定した視点（eye）、注視点（target）、上方向（up）からビュー行列を構築します。
		/// </summary>
		/// <param name="eye">視点のワールド座標</param>
		/// <param name="target">注視点のワールド座標</param>
		/// <param name="up">上方向を表すベクトル</param>
		void SetViewMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);
		/// <summary>
		/// 透視射影行列を設定する。
		///
		/// 指定したパラメータから射影行列を計算し内部に保持します。
		/// </summary>
		/// <param name="fovY">垂直方向の視野角（ラジアン）</param>
		/// <param name="aspectRatio">画面のアスペクト比（幅/高さ）</param>
		/// <param name="nearZ">近クリップ面の距離</param>
		/// <param name="farZ">遠クリップ面の距離</param>
		void SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

		/// <summary>
		/// ビュー行列を取得する。
		/// </summary>
		/// <returns>現在のビュー行列を返します。</returns>
		XMMATRIX GetViewMatrix();
        /// <summary>
		/// 射影行列を取得する。
		/// </summary>
		/// <returns>現在の射影行列を返します。</returns>
		XMMATRIX GetProjectionMatrix()const;
        /// <summary>
		/// ビュープロジェクション行列（View * Projection）を取得する。
		/// </summary>
		/// <returns>ビュー行列と射影行列を乗じた行列を返します。</returns>
		XMMATRIX GetViewProjectionMatrix();

		/// <summary>
		/// 正射影モードの切り替えを行う。
		/// </summary>
		/// <param name="flag">true の場合は正射影、false の場合は透視投影</param>
		void SetOrthographic(bool flag);
		/// <summary>
		/// 現在が正射影モードかどうかを確認する。
		/// </summary>
		/// <returns>正射影モードなら true、透視投影なら false を返します。</returns>
		bool IsOrthographic()const;

		/// <summary>
		/// 描画幅を設定する（正射影やリサイズ時に使用）。
		/// </summary>
		/// <param name="width">描画幅（ピクセルまたは仮想幅）</param>
		void SetWidth(float width);
		/// <summary>
		/// 描画高さを設定する（正射影やリサイズ時に使用）。
		/// </summary>
		/// <param name="height">描画高さ（ピクセルまたは仮想高さ）</param>
		void SetHeight(float height);

		/// <summary>
		/// 描画幅を取得する。
		/// </summary>
		/// <returns>設定されている描画幅を返します。</returns>
		float GetWidth()const;
		/// <summary>
		/// 描画高さを取得する。
		/// </summary>
		/// <returns>設定されている描画高さを返します。</returns>
		float GetHeight()const;

        /// <summary>
		/// 指定したワールド空間の方向を向くようにカメラを調整する。
		/// </summary>
		/// <param name="direction">向かせたいワールド空間での方向ベクトル（正規化されている必要はない）</param>
		void LookAtDirection(const Vector3& direction);

        /// <summary>
		/// レンダリング対象のレイヤーを名前で追加する。
		///
		/// 名前からレイヤーインデックスを解決して追加します。
		/// </summary>
		/// <param name="layer">追加するレイヤー名</param>
		void AddRenderingLayer(const string& layer);
		/// <summary>
		/// レンダリング対象のレイヤーをインデックスで追加する。
		/// </summary>
		/// <param name="layer">追加するレイヤーのインデックス</param>
		void AddRenderingLayer(const int layer);
		/// <summary>
		/// 登録されているレンダリングレイヤーの一覧を取得する。
		/// </summary>
		/// <returns>登録済みのレイヤーインデックスの配列を返します。</returns>
		vector<int> GetRenderingLayers()const;


     /// <summary>
		/// レンダラーコンポーネントをカメラに追加する（カメラが管理して描画する）。
		/// </summary>
		/// <param name="renderer">追加するレンダラーコンポーネントの共有ポインタ</param>
		void AddRenderObject(const shared_ptr<RendererComponent>& renderer);
       /// <summary>
		/// 現在設定されているレンダリングターゲットを取得する。
		/// </summary>
		/// <returns>現在のレンダリングターゲットの生ポインタを返します（所有権は渡しません）。</returns>
		RenderTarget* GetRenderTarget();

		INSPECTOR_COMPONENT(Camera)
			REGISTER_NAME(Camera)
			BEGIN_REFLECT()
			
			END_REFLECT()
	};

}
