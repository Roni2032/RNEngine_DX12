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
			XMFLOAT2 m_Float2;
		};

	};
	struct InputActionContext {
		bool m_IsPressed;
		bool m_IsHold;
		bool m_IsReleased;
		InputValue m_Value;

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

		static void ExecuteActions(vector<function<void(InputActionContext&)>>& actions, InputActionContext& context);
	public:
		static void Init();
		
		static void Update();
		
		static void RegisterInput(const string& actionName, int key, InputMode mode);

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

		static bool IsPressed(const string& actionName);
		static bool IsHeld(const string& actionName);

		static XMFLOAT2 GetMouseOffset() {
			return { (float)m_OffsetMousePoint.x,(float)m_OffsetMousePoint.y };
		}
		static XMFLOAT2 GetMousePosition() {
			return { (float)m_CurrentMousePoint.x,(float)m_CurrentMousePoint.y };
		}
		static XMFLOAT2 GetBeforeMousePosition() {
			return { (float)m_BeforeMousePoint.x,(float)m_BeforeMousePoint.y };
		}
	};
}

