#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Loders.h"

namespace RNEngine
{
	class DescriptorHeap;
	class PipelineState;
	class ConstantBuffer;
	class Transform;
	class Camera;

	class ModelResource
	{
		wstring m_Filename;
		Model m_ModelData;
		shared_ptr<PipelineState> m_PipelineState;

		DefaultModelTransform m_DefaultTransform;

		Matrix m_Matrix;
		vector<shared_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<ConstantBufferData> m_ConstantDates;

	public:
		ModelResource();
		~ModelResource(){}

		void Load(ID3D12Device* _dev, const string& filename);
		void Load(const Mesh& mesh);

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap);

		
		void SetDefaultTransform(const DefaultModelTransform& transform) { m_DefaultTransform = transform; }
		void SetDefaultPosition(const Vector3& position) { m_DefaultTransform.m_Position = position; }
		void SetDefaultScale(float scale) { m_DefaultTransform.m_Position = scale; }
		void SetDefaultRotation(const Vector3& rotation) { m_DefaultTransform.m_Position = rotation; }

		DefaultModelTransform& GetDefaultTransform() { return m_DefaultTransform; }
		Vector3 GetDefaultPosition() { return m_DefaultTransform.m_Position; }
		float GetDefaultScale() { return m_DefaultTransform.m_Scale; }
		Vector3 GetDefaultRotation() { return m_DefaultTransform.m_Rotation; }

		void SetMaterial(const string& key,int index = 0);
		Material& GetMaterial(int index);

		void SetPipelineState(const wstring& key);

		void SetCamera(const shared_ptr<Camera>& camera);
		void UpdateWorldMatrix(const shared_ptr<Transform>& transform);
		void UpdateWorldMatrix(const Vector3& position, const Vector3& scale, const Vector3& rotation);
		void RegisterConstantBuffer(void* data, size_t size);
		void CopyToGPU(shared_ptr<ConstantBuffer>& buffer, ConstantBufferData& data);


		shared_ptr<ModelResource> Clone() {
			shared_ptr<ModelResource> newModel = make_shared<ModelResource>();
			newModel->m_Filename = m_Filename;
			newModel->m_ModelData = m_ModelData;
			newModel->m_DefaultTransform = m_DefaultTransform;
			return newModel;
		}
	};


	
}