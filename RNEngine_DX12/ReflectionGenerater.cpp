#include "stdafx.h"
#include "ReflectionGenerater.h"

namespace RNEngine {

	template<class T>
	void ReflectionGenerater::Generate(const string& className) {
		//static const string REGISTER_MACRO_
		ComponentRegistry
	}



	void from_json(const Json& j, ReflectionField& field) {
		field.name = j.at("name").get<string>();
		field.variables = j.at("variables").get<vector<VariableField>>();
	}
	void from_json(const Json& j, VariableField& field) {
		field.name = j.at("name").get<string>();
		field.type = j.at("type").get<string>();
		if (j.contains("attributes")) {
			auto& attributes = j.at("attributes");
			for (auto& attribute : attributes) {
				string type = j.at("attributes").at("type");
				if (type == "Header") {
					field.attributes.push_back(make_shared<HeaderField>(j.at("attributes").get<HeaderField>()));
				}
				else if (type == "Convert") {
					field.attributes.push_back(make_shared<ConvertField>(j.at("attributes").get<ConvertField>()));
				}
			}
		}
	}
	void from_json(const Json& j, HeaderField& field) {
		field.type = j.at("type").get<string>();
		field.name = j.at("name").get<string>();
	}
	void from_json(const Json& j, ConvertField& field) {
		field.type = j.at("type").get<string>();
		auto funcNames = j.at("func").get<vector<string>>();
		if(funcNames.size() >= 2) {
			field.toDisplayFuncName = funcNames[0];
			field.toInternalFuncName = funcNames[1];
		}
		else {
			field.toDisplayFuncName = "";
			field.toInternalFuncName = "";
		}
	}


}