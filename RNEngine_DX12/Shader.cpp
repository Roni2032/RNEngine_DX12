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
				//�G���[���e�𕶎���Ƃ��ăR�s�[
				string err = (char*)m_ErrorBlob->GetBufferPointer();
				//�G���[���e���o��
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

//------------------------------InputLayout�e���v���[�g----------------------------------

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

	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::P = { POSITION };//�ʒu
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PC = { POSITION,COLOR };//�ʒu�A�F
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCUV = { POSITION,COLOR,UV };//�ʒu�A�F�AUV
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PN = { POSITION,NORMAL };//�ʒu�A�@��
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PNUV = { POSITION,NORMAL,UV };//�ʒu�A�@���AUV
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCNT = { POSITION,COLOR,NORMAL,TANGENT };//�ʒu�A�F�A�@���A�ڐ�
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCNUV = { POSITION,COLOR,NORMAL,UV };//�ʒu�A�F�A�@���AUV
	const vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::PCNTBUV = { POSITION,COLOR,NORMAL,TANGENT,BINORMAL,UV };//�ʒu�A�F�A�@���A�ڐ��A�]�@���AUV

}