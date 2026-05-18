#include "stdafx.h"
#include "RendererHeader.h"

namespace RNEngine {

	DefaultModelTransform::DefaultModelTransform() :DefaultModelTransform(1.0f, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }) {}
	DefaultModelTransform::DefaultModelTransform(float scale, Vector3 rotation, Vector3 position)
		:m_Scale(scale), m_Position(position), m_Rotation(rotation) {
	}

	void Model::CalcLocalBoundingBox() {
		Vector3 min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3 max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (auto& mesh : m_Meshes) {
			for(auto& vertex : mesh.m_Vertices){
				Vector3 adjustPosition = vertex.m_Position.Transform(m_AdjustMatrix);
				min = min.Min(adjustPosition);
				max = max.Max(adjustPosition);
			}
		}

		m_LocalBoundingBox = AABB(max, min);
	}
	int Model::FindBone(const string& name) {
		if (m_BoneIndexMap.count(name) > 0) {
			return m_BoneIndexMap[name];
		}
		return -1;
	}
}