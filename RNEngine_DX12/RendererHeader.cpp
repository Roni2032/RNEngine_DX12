#include "RendererHeader.h"

namespace RNEngine {

	DefaultModelTransform::DefaultModelTransform() :DefaultModelTransform(1.0f, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }) {}
	DefaultModelTransform::DefaultModelTransform(float scale, Vector3 rotation, Vector3 position)
		:m_Scale(scale), m_Position(position), m_Rotation(rotation) {
	}
}