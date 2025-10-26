#pragma once
namespace RNEngine
{
	class Shader
	{
		ComPtr<ID3DBlob> m_Blob;
		ComPtr<ID3DBlob> m_ErrorBlob;
		void Load(const wstring& filename, const string& entryPoint, const string& target);
	public:
		Shader(){}
		~Shader(){}

		void LoadVS(const wstring& filename,const string& entryPoint);
		void LoadPS(const wstring& filename, const string& entryPoint);

		const ComPtr<ID3DBlob> GetBlob()const { return m_Blob; }
		D3D12_SHADER_BYTECODE GetBytecode()const { return { m_Blob->GetBufferPointer(), m_Blob->GetBufferSize() }; }
	};

	struct InputLayout {
		InputLayout() {}
		InputLayout(const vector<D3D12_INPUT_ELEMENT_DESC>& layout) :m_Layout(layout) {}
		vector<D3D12_INPUT_ELEMENT_DESC> m_Layout;

		//頂点レイアウトテンプレート
		const static vector<D3D12_INPUT_ELEMENT_DESC> P;//位置
		const static vector<D3D12_INPUT_ELEMENT_DESC> PC;//位置、色
		const static vector<D3D12_INPUT_ELEMENT_DESC> PUV;//位置、色
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCUV;//位置、色、UV
		const static vector<D3D12_INPUT_ELEMENT_DESC> PN;//位置、法線
		const static vector<D3D12_INPUT_ELEMENT_DESC> PNUV;//位置、法線、UV
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCNT;//位置、色、法線、接線
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCNUV;//位置、色、法線、UV
		const static vector<D3D12_INPUT_ELEMENT_DESC> PCNTBUV;//位置、色、法線、接線、従法線、UV

	private:
		const static D3D12_INPUT_ELEMENT_DESC POSITION;
		const static D3D12_INPUT_ELEMENT_DESC COLOR;
		const static D3D12_INPUT_ELEMENT_DESC UV;
		const static D3D12_INPUT_ELEMENT_DESC NORMAL;
		const static D3D12_INPUT_ELEMENT_DESC TANGENT;
		const static D3D12_INPUT_ELEMENT_DESC BINORMAL;

	};


	struct Vertex {
		XMFLOAT3 m_Position;
		XMFLOAT2 m_Uv;
	};
}