#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "ComponentRegistry.h"

namespace RNEngine {
#define INSPECTOR_COMPONENT(name)\
private:\
	static bool g_AutoRegisterFlag;\
public:\
	static bool AutoRegisterComponent(){\
		ComponentRegistry::Register(#name,[](shared_ptr<GameObject> object){object->AddComponent<name>();});\
		return true;\
	}

#define INSPECTOR_COMPONENT_CPP(name)\
	bool name::g_AutoRegisterFlag = name::AutoRegisterComponent();

#define REGISTER_NAME(name)\
virtual string GetComponentName()override{\
	return #name;\
}\

#define BEGIN_REFLECT() \
virtual vector<FieldInfo> GetReflection()override{ \
	vector<FieldInfo> fields;

#define REGISTER_REFLECT(name,type, ...) \
{\
	FieldInfo f{ #name, offsetof(remove_reference_t<decltype(*this)>,name), type, {} };\
	AddAttributes(f, __VA_ARGS__);\
	fields.push_back(move(f));\
}

#define END_REFLECT() \
	return fields;\
}

#define OFFSET(type,name) offsetof(type,name)

	struct Attribute {
		virtual ~Attribute() = default;
	};
	/// <summary>
	/// 表示名を設定する
	/// </summary>
	struct HeaderAttribute : public Attribute {
		wstring m_Header;
		HeaderAttribute(const wstring& header) :m_Header(header) {}
	};
	/// <summary>
	/// インスペクターに隠しながらシリアライズの対象にする
	/// </summary>
	struct HideAttribute : public Attribute {
		HideAttribute() {}
	};

	/// <summary>
	/// インスペクターに表示・代入する値を変換する
	/// </summary>
	struct ConvertToAttribute : public Attribute {
		function<void(void* internalPtr, void* displayPtr)> m_ConvertToDisplay;
		function<void(void* displayPtr, void* internalPtr)> m_ConvertToInternal;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="convertToDisplay">値から表示値への変換</param>
		/// <param name="convertToInternal">表示値から値への変換</param>
		ConvertToAttribute(
			const function<void(void* internalPtr, void* displayPtr)>& convertToDisplay,
			const function<void(void* displayPtr, void* internalPtr)>& convertToInternal) :
			m_ConvertToDisplay(convertToDisplay), m_ConvertToInternal(convertToInternal) {
		}
	};

	struct FieldInfo {
		string m_Name;
		size_t m_Offset;
		enum class Type {
			Int, Float, Bool, Vec3
		}m_Type;
		vector<unique_ptr<Attribute>> m_Attribute;
	};

	template<typename... Param>
	void AddAttributes(FieldInfo& f, Param... params) {
		(f.m_Attribute.push_back(make_unique<Param>(params)), ...);
	}


	class ReflectInterface
	{
	public:
		~ReflectInterface() = default;

		virtual vector<FieldInfo> GetReflection() = 0;
		virtual string GetComponentName() = 0;

		string GetName() {
			return "";
		}
		vector<FieldInfo> GetFields() {
			return {};
		}

		static size_t GetVariableOffset(const string& name) {
			return 0;
		}
	};
}
