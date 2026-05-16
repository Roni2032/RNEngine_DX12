#pragma once
#include "stdafx.h"
#include "Reflection.h"
namespace RNEngine
{
	class GameObject;

	class Component : public Object,public ReflectInterface
	{
		weak_ptr<GameObject> m_Owner;
	public:
		Component(const shared_ptr<GameObject>& ptr) :m_Owner(ptr), Object(){}
		virtual ~Component() {}

     /// <summary>
		/// このコンポーネントが所属するオーナー GameObject の共有ポインタを取得する。
		/// </summary>
		/// <returns>オーナーの shared_ptr を返します。</returns>
		shared_ptr<GameObject> GetOwner()const;

		virtual void OnCollisionEnter(const shared_ptr<GameObject>& object){}
		virtual void OnCollisionExecute(const shared_ptr<GameObject>& object){}
		virtual void OnCollisionExit(const shared_ptr<GameObject>& object){}

		REGISTER_NAME(Component)
		BEGIN_REFLECT()
		END_REFLECT()
	};
	class Transform : public Component {
		Vector3 m_Position;
		Vector3 m_Scale;
		Quaternion m_Quaternion;
	public:
       /// <summary>
		/// Transform コンポーネントのコンストラクタ。
		/// </summary>
		/// <param name="ptr">オーナー GameObject の共有ポインタ</param>
		Transform(const shared_ptr<GameObject>& ptr);
		/// <summary>
		/// Transform コンポーネントのデストラクタ。
		/// </summary>
		virtual ~Transform();

        /// <summary>
		/// ワールド空間での位置を設定する。
		/// </summary>
		/// <param name="position">設定する位置ベクトル</param>
		void SetPosition(Vector3 position) {
			m_Position = position;
		}
        /// <summary>
		/// 現在の位置を取得する。
		/// </summary>
		/// <returns>位置ベクトルを返します。</returns>
		Vector3 GetPosition() const{
			return m_Position;
		}
      /// <summary>
		/// スケールを設定する。
		/// </summary>
		/// <param name="scale">設定するスケールベクトル</param>
		void SetScale(Vector3 scale) {
			m_Scale = scale;
		}
		/// <summary>
		/// 現在のスケールを取得する。
		/// </summary>
		/// <returns>スケールベクトルを返します。</returns>
		Vector3 GetScale() const{
			return m_Scale;
		}
        /// <summary>
		/// 回転を設定する（オイラー角）。
		/// </summary>
		/// <param name="rotation">回転を表すベクトル（度ではなくラジアン）</param>
		void SetRotation(Vector3 rotation) {
			m_Quaternion = Quaternion(rotation).Normalize();
		}
		/// <summary>
		/// 現在の回転をオイラー角で取得する。
		/// </summary>
		/// <returns>オイラー角（Roll, Pitch, Yaw）を返します。</returns>
		Vector3 GetRotation() const{
			return m_Quaternion.ConvertToRollPitchYaw();
		}
		/// <summary>
		/// 内部で保持しているクォータニオンを取得する。
		/// </summary>
		/// <returns>クォータニオンを返します。</returns>
		Quaternion GetQuaternion() {
			return m_Quaternion;
		}
		/// <summary>
		/// 内部のクォータニオンを設定する。
		/// </summary>
		/// <param name="quaternion">設定するクォータニオン</param>
		void SetQuaternion(Quaternion quaternion) {
			m_Quaternion = quaternion;
		}

       /// <summary>
		/// 前方方向ベクトルを取得する。
		/// </summary>
		/// <returns>前方方向の単位ベクトルを返します。</returns>
		Vector3 GetForward();

		/// <summary>
		/// 指定したターゲット方向を向くように回転を設定する。
		/// </summary>
		/// <param name="target">注視点のワールド座標</param>
		void LookAt(const Vector3& target);

		INSPECTOR_COMPONENT(Transform)
		REGISTER_NAME(Transform)
		BEGIN_REFLECT()
			REGISTER_REFLECT(m_Position.x, FieldInfo::Type::Vec3, HeaderAttribute("Position"))
			REGISTER_REFLECT(m_Quaternion.x, FieldInfo::Type::Vec3, HeaderAttribute("Rotation"),
				ConvertToAttribute(
					[](void* internalPtr, void* displayPtr) {
						float* displayF = reinterpret_cast<float*>(displayPtr);
						float* internalF = reinterpret_cast<float*>(internalPtr);
						Vector3 rot = Quaternion(internalF[0],internalF[1],internalF[2],internalF[3]).ConvertToRollPitchYaw();

						displayF[0] = XMConvertToDegrees(rot.x);
						displayF[1] = XMConvertToDegrees(rot.y);
						displayF[2] = XMConvertToDegrees(rot.z);
					},
					[](void* displayPtr, void* internalPtr) {
						float* displayF = reinterpret_cast<float*>(displayPtr);
						float* internalF = reinterpret_cast<float*>(internalPtr);
						Quaternion quat = Quaternion(Vector3(
							XMConvertToRadians(displayF[0]),
							XMConvertToRadians(displayF[1]),
							XMConvertToRadians(displayF[2])));
						quat.Normalize();

						internalF[0] = quat.x;
						internalF[1] = quat.y;
						internalF[2] = quat.z;
						internalF[3] = quat.w;
					}))
			REGISTER_REFLECT(m_Scale.x, FieldInfo::Type::Vec3, HeaderAttribute("Scale   "))
		END_REFLECT()
	};
}

