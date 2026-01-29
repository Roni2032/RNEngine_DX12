#include "stdafx.h"
#include "project.h"

namespace RNEngine {
	RendererComponent::RendererComponent(const shared_ptr<GameObject>& ptr)
		: Component(ptr) { }

	void RendererComponent::Init(const shared_ptr<Camera>& camera) {
		m_TargetCamera = camera;
	}
}