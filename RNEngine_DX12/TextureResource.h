#pragma once
#include "stdafx.h"
#include "RendererHeader.h"

namespace RNEngine {
	class DescriptorHeap;
	class TextureBuffer;
	class ConstantBuffer;
	class Camera;
	class Transform;
	class PipelineState;

	class TextureResource
	{
		Mesh m_Mesh;
		shared_ptr<TextureBuffer> m_Texture;
		shared_ptr<PipelineState> m_PipelineState;

		Matrix m_Matrix;
		vector<shared_ptr<ConstantBuffer>> m_ConstantBuffers;
		vector<ConstantBufferData> m_ConstantDates;
	public:
		TextureResource();
		~TextureResource(){}

		void SetPipelineState(const wstring& key);
		void SetTexture(const wstring& key);
		void SetTexture(const shared_ptr<TextureBuffer>& buffer);
		shared_ptr<TextureBuffer> GetTexture()const { return m_Texture; }

		void SetCamera(const shared_ptr<Camera>& camera);
		void UpdateWorldMatrix(const shared_ptr<Transform>& transform);
		void UpdateWorldMatrix(const Vector3& position, const Vector3& scale, const Vector3& rotation);
		void RegisterConstantBuffer(void* data, size_t size);
		void CopyToGPU(shared_ptr<ConstantBuffer>& buffer, ConstantBufferData& data);

		void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, DescriptorHeap* heap);
	};
}


