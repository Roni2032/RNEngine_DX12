#pragma once
#include "stdafx.h"
#include "Component.h"

namespace RNEngine
{
	class Camera;
	class GameObject;
	class DescriptorHeap;

	class RendererComponent : public Component{
	protected:
		weak_ptr<Camera> m_TargetCamera;
		vector<string> m_RenderTargetTag;
	public:
		RendererComponent(const shared_ptr<GameObject>& ptr);
		~RendererComponent(){}

		void Init(const shared_ptr<Camera>& camera);

		virtual void Update()override{}
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap){}

		void AddRenderTargetTag(const string& tag) {
			m_RenderTargetTag.push_back(tag);
		}
		vector<string> GetRenderTargetTag()const { return m_RenderTargetTag; }
	};
}