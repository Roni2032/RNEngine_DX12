#pragma once
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

	/// <summary>
	/// �`�揈�����s���N���X
	/// </summary>
	class Renderer {
		RTVBuffer m_RTVBuffer;	//�����_�[�^�[�Q�b�g�r���[�p�̃q�[�v
		DSVBuffer m_DSVBuffer;	//�[�x�o�b�t�@�p�̃q�[�v
		ComPtr<D3D12_VIEWPORT> m_ViewPort;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		ComPtr<IDXGISwapChain4> m_SwapChain;
	public:
		Renderer() {}
		~Renderer() {}

		void Init(Device& _dev,const Window& _window);
		void BeginRenderer();
	};


	class RenderTarget {
		ComPtr<ID3D12Resource> m_RenderTarget;
	};
}