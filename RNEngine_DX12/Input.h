#pragma once

#include "stdafx.h"
namespace RNEngine
{
	enum class InputMode {
		Keyboard,
		Gamepad
	};
	enum class InputState {
		Pressed,
		Released,
		Held,
		None
	};
	struct InputData {
		int key;
		InputMode mode;
	};
	struct InputValue {
		enum class ValueType {
			Bool, Float2
		};
		ValueType m_Type;
		union {
			bool m_Bool;
			Vector2 m_Float2;
		};

	};
	struct InputActionContext {
		bool m_IsPressed;
		bool m_IsHold;
		bool m_IsReleased;
		InputValue m_Value;

        /// <summary>
		/// バインドされたアクションに渡される入力コンテキストから値を取得するテンプレート関数。
		///
		/// 内部に保持された InputValue の型に応じて、指定した型で値を返します。
		/// </summary>
		/// <returns>要求した型の値を返します。型が一致しない場合は既定値を返します。</returns>
		template<typename T>
		T GetValue() { return T{}; }
	};

	
	struct InputAction {
		vector<function<void(InputActionContext&)>> m_Actions;
		
		vector<InputData> m_Inputs;
		InputState m_State = InputState::None;

		bool IsPressed()const { return m_State == InputState::Pressed; }
		bool IsReleased()const { return m_State == InputState::Released; }
		bool IsHeld()const { return m_State == InputState::Held; }
	};
	class Input {
		static unordered_map<string, InputAction> m_ActionMap;
		static POINT m_CurrentMousePoint;
		static POINT m_BeforeMousePoint;
		static POINT m_OffsetMousePoint;

      /// <summary>
		/// 登録されたアクションコールバック群を実行する内部ヘルパー。
		/// </summary>
		/// <param name="actions">実行するアクションコールバックの配列</param>
		/// <param name="context">各コールバックに渡す入力コンテキスト</param>
		static void ExecuteActions(vector<function<void(InputActionContext&)>>& actions, InputActionContext& context);
	public:
     /// <summary>
		/// 入力システムを初期化する。
		///
		/// マウス位置などの初期状態を設定します。
		/// </summary>
		static void Init();

		/// <summary>
		/// 毎フレーム呼び出される入力更新処理。
		///
		/// キーやマウスの状態を集計し、アクションの状態遷移を更新します。
		/// </summary>
		static void Update();

		/// <summary>
		/// アクション名に対してキーを登録する。
		/// </summary>
		/// <param name="actionName">アクションの名前</param>
		/// <param name="key">対応するキーコード</param>
		/// <param name="mode">入力モード（キーボード/ゲームパッド）</param>
		static void RegisterInput(const string& actionName, int key, InputMode mode);

		/// <summary>
		/// 指定したアクション名にコールバックをバインドする。
		/// </summary>
		/// <param name="actionName">アクションの名前</param>
		/// <param name="action">アクション呼び出し時に実行されるコールバック</param>
		static void BindAction(const string& actionName, function<void(InputActionContext&)> action);

		template<class T>
		static void BindAction(const string& actionName, void(T::* method)(InputActionContext&), T* instance) {
			function<void(InputActionContext&)> func = [=](InputActionContext& context) { (instance->*method)(context); };
			BindAction(actionName, func);
		}
		template<class T>
		static void BindAction(const string& actionName, void(T::* method)(InputActionContext&), shared_ptr<T> instance) {
			function<void(InputActionContext&)> func = [=](InputActionContext& context) { (instance.get()->*method)(context); };
			BindAction(actionName, func);
		}

        /// <summary>
		/// 指定したアクションが押された瞬間かどうかを判定する。
		/// </summary>
		/// <param name="actionName">アクションの名前</param>
		/// <returns>押された瞬間であれば true を返します。</returns>
		static bool IsPressed(const string& actionName);
		/// <summary>
		/// 指定したアクションがホールド状態かどうかを判定する。
		/// </summary>
		/// <param name="actionName">アクションの名前</param>
		/// <returns>ホールド状態であれば true を返します。</returns>
		static bool IsHeld(const string& actionName);

       /// <summary>
		/// フレーム間のマウス移動量を取得する。
		/// </summary>
		/// <returns>マウスのオフセット（x,y）を返します。</returns>
		static Vector2 GetMouseOffset() {
			return { (float)m_OffsetMousePoint.x,(float)m_OffsetMousePoint.y };
		}
		/// <summary>
		/// 現在のマウス位置を取得する（スクリーン座標）。
		/// </summary>
		/// <returns>マウスのスクリーン座標（x,y）を返します。</returns>
		static Vector2 GetMousePosition() {
			return { (float)m_CurrentMousePoint.x,(float)m_CurrentMousePoint.y };
		}
		/// <summary>
		/// 現在のマウス位置を POINT 型で取得する。
		/// </summary>
		/// <returns>POINT 型のマウス座標を返します。</returns>
		static POINT GetMousePointPosition() {
			return m_CurrentMousePoint;
		}
		/// <summary>
		/// 前フレーム時点のマウス位置を取得する。
		/// </summary>
		/// <returns>前フレームのマウス座標（x,y）を返します。</returns>
		static Vector2 GetBeforeMousePosition() {
			return { (float)m_BeforeMousePoint.x,(float)m_BeforeMousePoint.y };
		}
		/// <summary>
		/// マウス位置からワールド空間上のレイ方向を取得する。
		/// </summary>
		/// <returns>ワールド空間でのレイの方向ベクトル（正規化済み）を返します。</returns>
		static Vector3 GetMouseWorldRayDirection();
	};
}

