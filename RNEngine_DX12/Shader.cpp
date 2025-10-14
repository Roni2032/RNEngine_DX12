#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	void Shader::Load(const wstring& filename, const wstring& entryPoint, const wstring& target) {
		string entry = string(entryPoint.begin(), entryPoint.end());
		string targ = string(target.begin(), target.end());
		auto result = D3DCompileFromFile(
			filename.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entry.c_str(),
			targ.c_str(),
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			m_Blob.GetAddressOf(),
			m_ErrorBlob.GetAddressOf()
		);

		if(FAILED(result)){
			if (m_ErrorBlob) {
				//エラー内容を文字列としてコピー
				string err = (char*)m_ErrorBlob->GetBufferPointer();
				//エラー内容を出力
				cout << err << endl;
			}
			assert(false);
		}
	}
	void Shader::LoadVS(const wstring& filename, const wstring& entryPoint) {
		Load(filename, entryPoint, L"vs_5_0");
	}
	void Shader::LoadPS(const wstring& filename, const wstring& entryPoint) {
		Load(filename, entryPoint, L"ps_5_0");
	}

//------------------------------InputLayoutテンプレート----------------------------------

	const D3D12_INPUT_ELEMENT_DESC InputLayout::POSITION = {
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	const D3D12_INPUT_ELEMENT_DESC InputLayout::COLOR = {
		"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	const D3D12_INPUT_ELEMENT_DESC InputLayout::UV = {
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	const D3D12_INPUT_ELEMENT_DESC InputLayout::NORMAL = {
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	const D3D12_INPUT_ELEMENT_DESC InputLayout::TANGENT = {
		"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};
	const D3D12_INPUT_ELEMENT_DESC InputLayout::BINORMAL = {
		"BINORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	};

	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::P = { POSITION };//位置
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PC = { POSITION,COLOR };//位置、色
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCUV = { POSITION,COLOR,UV };//位置、色、UV
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PN = { POSITION,NORMAL };//位置、法線
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PNUV = { POSITION,NORMAL,UV };//位置、法線、UV
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCNT = { POSITION,COLOR,NORMAL,TANGENT };//位置、色、法線、接線
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCNUV = { POSITION,COLOR,NORMAL,UV };//位置、色、法線、UV
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCNTBUV = { POSITION,COLOR,NORMAL,TANGENT,BINORMAL,UV };//位置、色、法線、接線、従法線、UV

}