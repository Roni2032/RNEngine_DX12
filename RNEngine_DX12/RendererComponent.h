#pragma once
#include "stdafx.h"
#include "RendererHeader.h"
#include "Component.h"
namespace RNEngine
{
	class DescriptorHeap;
	class Model;
	class ConstBuffer;
	class Image;
	class Camera;

	class RendererComponent : public Component{
		struct ConstantBufferData {
			void* m_Data;
			size_t m_DataSize;
		};
		shared_ptr<PipelineState> m_PipelineState;
	protected:
		weak_ptr<Camera> m_TargetCamera;
		vector<unique_ptr<ConstBuffer>> m_ConstantBuffers;
		Matrix m_Matrix;

		vector<ConstantBufferData> m_ConstantDatas;

		vector<string> m_RenderTargetTag;

		void UpdateConstantBuffers();

	public:
		RendererComponent(const shared_ptr<GameObject>& ptr);
		RendererComponent(const shared_ptr<GameObject>& ptr, const Matrix& matrix);
		~RendererComponent(){}

		void Init(const shared_ptr<Camera>& camera);

		virtual void Update()override;

		void UpdateWorldMatrix(Vector3 position, Vector3 scale, Vector3 rotation);

		void DrawMesh(ID3D12GraphicsCommandList* cmdList,Mesh& mesh);

		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap){}

		void RegisterConstantBuffer(void* data, size_t size);

		void AddRenderTargetTag(const string& tag) {
			m_RenderTargetTag.push_back(tag);
		}
		vector<string> GetRenderTargetTag()const { return m_RenderTargetTag; }

		void SetPipelineState(const wstring& key);
	};

	class ModelRenderer : public RendererComponent{
		shared_ptr<Model> m_Model;
	public:
		ModelRenderer(const shared_ptr<GameObject>& ptr):RendererComponent(ptr){}
		~ModelRenderer() {}

		void SetModel(const string& filename);

		shared_ptr<Model>& GetModel();
		virtual void Update()override;
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;

		REGISTER_NAME(ModelRenderer)
	};

	class ImageRenderer : public RendererComponent {
		shared_ptr<Image> m_Image;
	public:
		ImageRenderer(const shared_ptr<GameObject>& ptr);
		~ImageRenderer(){}
		void Init(const shared_ptr<Camera>& camera);
		void SetTexture(const string& filename);
		virtual void Draw(ID3D12GraphicsCommandList* cmdList, DescriptorHeap* heap)override;
	};
}