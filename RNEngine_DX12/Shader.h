#pragma once
namespace RNEngine
{
	class Shader
	{
		ComPtr<ID3DBlob> m_Blob;
		ComPtr<ID3DBlob> m_ErrorBlob;
     /// <summary>
		/// シェーダファイルを読み込み、コンパイルして内部バイナリに格納する。
		/// </summary>
		/// <param name="filename">シェーダファイルのパス（ワイド文字列）</param>
		/// <param name="entryPoint">エントリポイント名（例: "main"）</param>
		/// <param name="target">コンパイルターゲット（例: "vs_5_0" / "ps_5_0"）</param>
		void Load(const wstring& filename, const string& entryPoint, const string& target);
	public:
      /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		Shader(){}
		/// <summary>
		/// デストラクタ
		/// </summary>
		~Shader(){}

		/// <summary>
		/// 頂点シェーダを読み込む。
		/// </summary>
		/// <param name="filename">シェーダファイルのパス（ワイド文字列）</param>
		/// <param name="entryPoint">エントリポイント名</param>
		void LoadVS(const wstring& filename,const string& entryPoint);
		/// <summary>
		/// ピクセルシェーダを読み込む。
		/// </summary>
		/// <param name="filename">シェーダファイルのパス（ワイド文字列）</param>
		/// <param name="entryPoint">エントリポイント名</param>
		void LoadPS(const wstring& filename, const string& entryPoint);

		/// <summary>
		/// コンパイル済みシェーダバイナリのポインタを取得する。
		/// </summary>
		/// <returns>ID3DBlob の生ポインタを返します（所有権は渡しません）。</returns>
		const ID3DBlob* GetBlob()const { return m_Blob.Get(); }
		/// <summary>
		/// シェーダバイトコードを D3D12 用の構造体として取得する。
		/// </summary>
		/// <returns>D3D12_SHADER_BYTECODE 構造体を返します。</returns>
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
		const static D3D12_INPUT_ELEMENT_DESC BONEWEIGHT;
		const static D3D12_INPUT_ELEMENT_DESC BONEINDICES;

	};
}