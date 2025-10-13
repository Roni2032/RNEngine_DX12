#pragma once
#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"
namespace RNEngine {
	
	///----------------------------------------------------------------
	/// Renderer �w�b�_
	/// 
	/// Renderer �N���X
	/// PipelineState �N���X
	/// Shader �N���X
	/// Buffer �N���X
	/// Texture �N���X
	/// CommandContext �N���X
	///		:
	/// 
	/// ----------------------------------------------------------------


	class PipelineState {
		ComPtr<ID3D12PipelineState> m_PipelineState;
		InputLayout m_InputLayout;
	public:
		PipelineState() {}
		~PipelineState() {}

		void SetInputLayout(const InputLayout& layout) { m_InputLayout = layout; }
		void SetInputLayout(const vector<D3D12_INPUT_ELEMENT_DESC>& layout) { m_InputLayout = InputLayout(layout); }

		void Create(ComPtr<ID3D12Device>& _dev, ComPtr<ID3DBlob>& _vsBlob, ComPtr<ID3DBlob>& _psBlob);
	};
	class RenderTarget {
		ComPtr<ID3D12Resource> m_RenderTarget;
	};



	/// <summary>
	/// �`�揈�����s���N���X
	/// </summary>
	class Renderer {
		RTVBuffer m_RTVBuffer;	//�����_�[�^�[�Q�b�g�r���[�p�̃q�[�v
		DSVBuffer m_DSVBuffer;	//�[�x�o�b�t�@�p�̃q�[�v
		D3D12_VIEWPORT m_ViewPort;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		ComPtr<IDXGISwapChain4> m_SwapChain;
		Fence m_Fence;
		Barrier m_Barrier;
		PipelineState m_PipelineState;

		vector<float> m_ClearColor;
	public:
		Renderer() {}
		~Renderer() {}

		void Init(Device& _dev, const Window& _window);
		void BeginRenderer();
		void EndRenderer();

		void SetClearColor(float r, float g, float b, float a) {
			m_ClearColor = { r,g,b,a };
		}
	};

}