#pragma once
#include "stdafx.h"
namespace RNEngine
{
	class DescriptorHeap;
	class Model;
	class ConstBuffer;
	class Image;


	class RendererComponent {
		struct ConstantBufferData {
			void* m_Data;
			size_t m_DataSize;
		};
	protected:
		vector<unique_ptr<ConstBuffer>> m_ConstantBuffers;
		Matrix m_Matrix{};

		vector<ConstantBufferData> m_ConstantDatas;
	public:
		RendererComponent(){}
		RendererComponent(const Matrix& matrix) : m_Matrix(matrix){}
		~RendererComponent(){}

		void Init(XMFLOAT3 eye, XMFLOAT3 target);

		void Update();

		void UpdateWorldMatrix(XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap){}

		void RegisterConstantBuffer(void* data, size_t size);
	};

	class ModelRenderer : public RendererComponent{
		shared_ptr<Model> m_Model;
	public:
		ModelRenderer(){}
		~ModelRenderer() {}

		void SetModel(const string& filename);

		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;
	};

	class ImageRenderer : public RendererComponent {
		shared_ptr<Image> m_Image;
	public:
		ImageRenderer() {
			m_Image = make_shared<Image>();
		}
		~ImageRenderer(){}

		void Init(XMFLOAT3 eye, XMFLOAT3 target);
		void SetTexture(const string& filename);
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;
	};
}