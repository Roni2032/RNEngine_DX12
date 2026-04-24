#pragma once
namespace RNEngine {
	struct AnimationClip {
		float m_TickPerSeconds;
		float m_Duration;


	};
	

	class Animation
	{

		XMMATRIX m_GlobalTransformMatrix;
		vector<XMMATRIX> m_OffsetMatrices;

		float m_CurrentAnimationTime;
	public:
		
	};
}
