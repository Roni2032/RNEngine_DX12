#pragma once
namespace RNEngine
{
	class Shader
	{
		ComPtr<ID3DBlob> m_Blob;
		ComPtr<ID3DBlob> m_ErrorBlob;
		void Load(const wstring& filename, const wstring& entryPoint, const wstring& target);
	public:
		Shader(){}
		~Shader(){}

		void LoadVS(const wstring& filename,const wstring& entryPoint);
		void LoadPS(const wstring& filename, const wstring& entryPoint);

		ComPtr<ID3DBlob> GetBlob()const { return m_Blob; }
	};

	struct InputLayout {
		InputLayout() {}
		InputLayout(const vector<D3D12_INPUT_ELEMENT_DESC>& layout) :m_Layout(layout) {}
		vector<D3D12_INPUT_ELEMENT_DESC> m_Layout;

		//���_���C�A�E�g�e���v���[�g
		const static vector<D3D12_INPUT_ELEMENT_DESC> P;//�ʒu
		const static vector<D3D12_INPUT_ELEMENT_DESC> PC;//�ʒu�A�F
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCUV;//�ʒu�A�F�AUV
		const static vector<D3D12_INPUT_ELEMENT_DESC> PN;//�ʒu�A�@��
		const static vector<D3D12_INPUT_ELEMENT_DESC> PNUV;//�ʒu�A�@���AUV
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCNT;//�ʒu�A�F�A�@���A�ڐ�
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCNUV;//�ʒu�A�F�A�@���AUV
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCNTBUV;//�ʒu�A�F�A�@���A�ڐ��A�]�@���AUV

	private:
		const static D3D12_INPUT_ELEMENT_DESC POSITION;
		const static D3D12_INPUT_ELEMENT_DESC COLOR;
		const static D3D12_INPUT_ELEMENT_DESC UV;
		const static D3D12_INPUT_ELEMENT_DESC NORMAL;
		const static D3D12_INPUT_ELEMENT_DESC TANGENT;
		const static D3D12_INPUT_ELEMENT_DESC BINORMAL;

	};
}