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
		Held
	};
	struct InputData {
		char key;
		InputMode mode;
	};
	struct InputValue {
		enum class ValueType {
			Bool, Float2
		};
		ValueType m_Type;
		union {
			bool m_Bool;
			struct {
				float x;
				float y;
			} m_Float2;
		};

	};
	class InputAction {
		vector<function<void()>> m_Actions;
		
		vector<InputData> m_Inputs;
		InputState m_State;

		InputValue m_Value;
	public:
		template<typename T>
		T GetValue()const;

		bool IsPressed()const { m_State == InputState::Pressed; }
		bool IsReleased()const { m_State == InputState::Released; }
		bool IsHeld()const { m_State == InputState::Held; }
	};
	class Input {
		static unordered_map<string, InputData> m_InputMap;
	public:
		//static void RegisterInput(const string& actionName, char key, InputMode mode);

		//static bool IsPressed(const string& actionName);
	};
}

