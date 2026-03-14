#include "stdafx.h"
#include "ReflectionGenerater.h"

namespace RNEngine {

	template<class T>
	ReflectionField& ReflectionGenerater::Generate(const string& className) {
		string refFileName = className + ".ref.cpp";
		string jsonFileName = className + ".json";

		ifstream jsonFile(jsonFileName);
		if(!jsonFile.is_open()) {
			//ここでjsonを自動生成できたらいいな...
			cerr << "JSONファイルを開くのに失敗しました: " << jsonFileName << endl;
			return;
		}
		if(!Json::accept(jsonFile)){
			//jsonの形式が不正
			cerr << "JSONのフォーマットが間違っています: " << jsonFileName << endl;
			return;
		}
		//acceptで進んだ位置を先頭にリセット
		jsonFile.seekg(0, std::ios::beg);

		Json json = Json::parse(jsonFile);
		ReflectionField reflectionField = json.get<ReflectionField>();

		CreateRefFile(refFileName, reflectionField);

		return reflectionField;
	}
	void ReflectionGenerater::CreateRefFile(const string& filename, const ReflectionField& field) {
		ofstream refFile(filename);
		if (refFile.is_open()) {
			cerr << "refファイルを開くのに失敗しました: " << filename << endl;
			return;
		}
		string structName = field.name + "Reflect";

		refFile << "#include \"../../" << field.filename << "\"" << endl;
		refFile << "#include \"../../ReflectionGenerater.h\"" << endl;
		refFile << endl;
		refFile << "namespace RNEngine {" << endl;
		refFile << "	struct " << structName << " {" << endl;
		refFile << "		" << structName << "() { " << endl;
		refFile << "			ReflectionTable::Register(" << "typeid(" << field.name << "), {" << endl;

	}


	void from_json(const Json& j, ReflectionField& field) {
		field.name = j.at("class").get<string>();
		field.filename = j.at("file").get<string>();
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