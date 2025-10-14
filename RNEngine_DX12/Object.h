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

	class Object
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
		virtual void Update(float delta) {}

		/// <summary>
		/// すべてのUpdate()が終わった後に呼ばれる
		/// </summary>
		virtual void LateUpdate(float delta) {}

		/// <summary>
		/// 描画処理の際に呼ばれる
		/// </summary>
		virtual void Draw() {}

		/// <summary>
		/// 破棄の際に呼ばれる
		/// </summary>
		virtual void Destroy() {}
	};

}
