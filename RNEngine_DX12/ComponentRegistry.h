#pragma once
#include "stdafx.h"
namespace RNEngine {
	class GameObject;
	class Component;

	class ComponentRegistry {
		static auto& GetMap() {
			static unordered_map<string, function<void(shared_ptr<GameObject>)>> componentMap = {};
			return componentMap;
		}
	public:
       /// <summary>
		/// コンポーネント生成関数を登録する。
		/// </summary>
		/// <param name="name">コンポーネント名</param>
		/// <param name="fuc">GameObject に対してコンポーネントを追加する関数</param>
		static void Register(const string& name, function<void(shared_ptr<GameObject>)> fuc);

		/// <summary>
		/// 名前に対応するコンポーネントを指定の GameObject に追加する。
		/// </summary>
		/// <param name="name">コンポーネント名</param>
		/// <param name="object">追加先の GameObject</param>
		static void AddComponent(const string& name, shared_ptr<GameObject>& object);
	};
}
