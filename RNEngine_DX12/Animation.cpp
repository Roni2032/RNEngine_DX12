#include "stdafx.h"
#include "Animation.h"

namespace RNEngine {

	XMMATRIX AnimationClip::CalcTransformMatrixPosition(float time) {
		float factor;
		int keyFrameIndex = FindKeyFrameFactor(m_PositionKeyFrame, time, factor);

		Vector3 start = m_PositionKeyFrame[keyFrameIndex].m_Value;
		Vector3 end = m_PositionKeyFrame[keyFrameIndex + 1].m_Value;
		Vector3 current = start + (end - start) * factor;
		return XMMatrixTranslation(current.x, current.y, current.z);
	}
	XMMATRIX AnimationClip::CalcTransformMatrixQuaternion(float time) {
		float factor;
		int keyFrameIndex = FindKeyFrameFactor(m_QuaternionKeyFrame, time, factor);

		Quaternion start = m_QuaternionKeyFrame[keyFrameIndex].m_Value;
		Quaternion end = m_QuaternionKeyFrame[keyFrameIndex + 1].m_Value;
		Quaternion current = (Quaternion)XMQuaternionSlerp(start, end, factor);
		current.Normalize();
		return XMMatrixRotationQuaternion(XMLoadFloat4(&current));
	}
	XMMATRIX AnimationClip::CalcTransformMatrixScaling(float time) {
		float factor;
		int keyFrameIndex = FindKeyFrameFactor(m_ScalingKeyFrame, time, factor);

		Vector3 start = m_ScalingKeyFrame[keyFrameIndex].m_Value;
		Vector3 end = m_ScalingKeyFrame[keyFrameIndex + 1].m_Value;
		Vector3 current = start + (end - start) * factor;
		return XMMatrixScaling(current.x, current.y, current.z);
	}

	XMMATRIX AnimationClip::CalcTransformMatrix(float time) {
		return CalcTransformMatrixScaling(time) * CalcTransformMatrixQuaternion(time) * CalcTransformMatrixPosition(time);
	}
}
