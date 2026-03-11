#pragma once
#include "stdafx.h"

namespace RNEngine
{
	struct AttributeField {
		string type;
	};
	struct HeaderField : public AttributeField {
		string name;
	};
	struct ConvertField : public AttributeField {
		string toDisplayFuncName;
		string toInternalFuncName;
	};

	struct VariableField {
		string name;
		string type;
		vector<shared_ptr<AttributeField>> attributes;
	};
	struct ReflectionField {
		string name;
		vector<VariableField> variables;
	};
	

#define GET_CLASSNAME(x) #x

	class ReflectionGenerater
	{
	public:
		ReflectionGenerater(){}
		~ReflectionGenerater(){}

		template<class T>
		void Generate(const string& className);

	};

	class ReflectionTable {
		static auto& GetMap() {
			static unordered_map<type_info, vector<ReflectionField>> reflectionMap = {};
			return reflectionMap;
		}
	public:
		static void Register(const type_info& type, const vector<ReflectionField>& fields) {
			GetMap()[type] = fields;
		}
		static vector<ReflectionField> GetFields(const type_info& type) {
			auto it = GetMap().find(type);
			if (it != GetMap().end()) {
				return (*it).second;
			}
			return {};
		}
	};


	

	void from_json(const Json& j, ReflectionField& field);
	void from_json(const Json& j, VariableField& field);
	void from_json(const Json& j, HeaderField& field);
	void from_json(const Json& j, ConvertField& field);
}
