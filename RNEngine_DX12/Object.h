#pragma once
//----------------------------------------------------------------
//	Object ヘッダ
// 
//  ゲーム内で使用するオブジェクトの基底クラス
//  ここからGameObjectやComponentなどが派生していく
//
//----------------------------------------------------------------
#include "stdafx.h"

namespace RNEngine {

	class Object : public enable_shared_from_this<Object>
	{
	public:
		Object(){}
		virtual ~Object() {}

		/// <summary>
		/// 作成されたフレームで一度だけ呼ばれる
		/// </summary>
		virtual void Start(){}

		/// <summary>
		/// 毎フレーム呼ばれる
		/// </summary>
		virtual void Update() {}

		/// <summary>
		/// すべてのUpdate()が終わった後に呼ばれる
		/// </summary>
		virtual void LateUpdate() {}

		/// <summary>
		/// 描画処理の際に呼ばれる
		/// </summary>
		virtual void Draw() {}

		/// <summary>
		/// 破棄の際に呼ばれる
		/// </summary>
		virtual void Destroy() {}

		template<class T>
		shared_ptr<T> GetThis() {
			auto ptr = dynamic_pointer_cast<T>(shared_from_this());
			return ptr;
		}
	};

}
