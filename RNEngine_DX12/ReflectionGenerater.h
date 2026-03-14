#pragma once
#include "stdafx.h"
#include "Reflection.h"
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
		string filename;
		vector<VariableField> variables;
	};
	

#define GET_CLASSNAME(x) #x

	class ReflectionGenerater
	{
	public:
		ReflectionGenerater(){}
		~ReflectionGenerater(){}

		template<class T>
		ReflectionField& Generate(const string& className);

		void CreateRefFile(const string& filename, const ReflectionField& field);

	};

	class ReflectionTable {
		static auto& GetMap() {
			static unordered_map<std::type_index, vector<FieldInfo>> reflectionMap = {};
			return reflectionMap;
		}
	public:
		static void Register(const type_index& type, vector<FieldInfo>& fields) {
			GetMap()[type] = move(fields);
		}
		static vector<FieldInfo>& GetFields(const type_index& type) {
			auto& map = GetMap();
			auto it = map.find(type);
			if (it != map.end()) {
				return (*it).second;
			}

			vector<FieldInfo> empty;
			return empty;
		}
	};


	

	void from_json(const Json& j, ReflectionField& field);
	void from_json(const Json& j, VariableField& field);
	void from_json(const Json& j, HeaderField& field);
	void from_json(const Json& j, ConvertField& field);
}
