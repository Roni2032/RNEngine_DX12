#include "stdafx.h"
#include "Input.h"
namespace RNEngine {
	template<>
	bool InputActionContext::GetValue<bool>() {
		if (m_Value.m_Type == InputValue::ValueType::Bool) {
			return m_Value.m_Bool;
		}
		return false;
	}

	template<>
	XMFLOAT2 InputActionContext::GetValue<XMFLOAT2>() {
		if (m_Value.m_Type == InputValue::ValueType::Float2) {
			return m_Value.m_Float2;
		}
		return { 0,0 };
	}

	unordered_map<string, InputAction> Input::m_ActionMap = {};
	POINT Input::m_CurrentMousePoint = {};
	POINT Input::m_BeforeMousePoint = {};
	POINT Input::m_OffsetMousePoint = {};

	void Input::ExecuteActions(vector<function<void(InputActionContext&)>>& actions, InputActionContext& context) {
		for (auto& prossess : actions) {
			prossess(context);
		}
	}

	void Input::Init() {
		m_OffsetMousePoint = { 0,0 };
		GetCursorPos(&m_CurrentMousePoint);
		m_BeforeMousePoint = m_CurrentMousePoint;
	}

	void Input::Update() {
		m_BeforeMousePoint = m_CurrentMousePoint;
		GetCursorPos(&m_CurrentMousePoint);

		m_OffsetMousePoint.x = m_CurrentMousePoint.x - m_BeforeMousePoint.x;
		m_OffsetMousePoint.y = m_CurrentMousePoint.y - m_BeforeMousePoint.y;

		for (auto& inputAction : m_ActionMap) {
			InputAction& action = inputAction.second;
			InputActionContext context{};
			bool isAnyKey = false;
			for (auto& input : action.m_Inputs) {
				switch (input.mode) {
				case InputMode::Keyboard:
					if (GetAsyncKeyState(input.key)) {
						isAnyKey = true;
					}
					break;
				case InputMode::Gamepad:
					break;
				}
			}
			if (isAnyKey) {
				action.m_State = action.m_State == InputState::None ? InputState::Pressed : InputState::Held;
			}
			else {
				if (action.m_State == InputState::Held || action.m_State == InputState::Pressed) {
					action.m_State = InputState::Released;
				}
				else {
					action.m_State = InputState::None;
				}
			}
			if (action.m_State != InputState::None) {
				ExecuteActions(action.m_Actions, context);
			}
		}
	}

	void Input::RegisterInput(const string& actionName, int key, InputMode mode) {
		auto it = m_ActionMap.find(actionName);
		if (it != m_ActionMap.end()) {
			m_ActionMap[actionName].m_Inputs.push_back({ key,mode });
			return;
		}

		m_ActionMap[actionName] = {};
		m_ActionMap[actionName].m_Inputs.push_back({ key,mode });
	}
	void Input::BindAction(const string& actionName, function<void(InputActionContext&)> action) {
		auto it = m_ActionMap.find(actionName);
		if (it != m_ActionMap.end()) {
			m_ActionMap[actionName].m_Actions.push_back(action);
			return;
		}

		m_ActionMap[actionName] = {};
		m_ActionMap[actionName].m_Actions.push_back(action);
	}

	bool Input::IsPressed(const string& actionName) {
		auto it = m_ActionMap.find(actionName);
		if (it != m_ActionMap.end()) {
			return false;
		}

		return m_ActionMap[actionName].IsPressed();
	}

	bool Input::IsHeld(const string& actionName) {
		auto it = m_ActionMap.find(actionName);
		if (it == m_ActionMap.end()) {
			return false;
		}

		return m_ActionMap[actionName].IsHeld();
	}
}