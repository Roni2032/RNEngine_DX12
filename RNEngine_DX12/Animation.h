#pragma once
namespace RNEngine {
	template<typename T>
	struct KeyFrame {
		float m_Time = 0.0f;
		T m_Value = {};
	};
	struct AnimationClip {
		float m_TickPerSeconds = 0.0f;
		float m_Duration = 0.0f;

		vector<KeyFrame<Vector3>> m_PositionKeyFrame;
		vector<KeyFrame<Quaternion>> m_QuaternionKeyFrame;
		vector<KeyFrame<Vector3>> m_ScalingKeyFrame;

		template<typename T>
		int FindKeyFrameFactor(const vector<KeyFrame<T>>& keyFrames, float time, float& factor) {
			for (int i = 0; i < keyFrames.size() - 1; i++) {
				if (time < keyFrames[i].m_Time) {
					float delta = keyFrames[i + 1].m_Time - keyFrames[i].m_Time;
					factor = (time - keyFrames[i].m_Time) / delta;
					return i;
				}
			}
		}

		XMMATRIX CalcTransformMatrixPosition(float time);
		XMMATRIX CalcTransformMatrixQuaternion(float time);
		XMMATRIX CalcTransformMatrixScaling(float time);

		XMMATRIX CalcTransformMatrix(float time);
	};
	struct Animation {
		XMMATRIX m_GlobalInverseTransform;
		vector<AnimationClip> m_Clips;
	};
	

	class Animator
	{
		XMMATRIX m_GlobalTransformMatrix;

		float m_CurrentAnimationTime;
	public:
		
	};
}
