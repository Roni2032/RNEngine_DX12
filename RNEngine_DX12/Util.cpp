#include "stdafx.h"
#include "Util.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace RNEngine {
	wstring Util::ConvertStrToWstr(const string& str) {
		int strSize = MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			(int)str.size(),
			nullptr,
			0
		);

		wstring result(strSize, 0);

		MultiByteToWideChar(
			CP_UTF8,0,
			str.c_str(),(int)str.size(),
			&result[0],strSize
		);
		return result;
	}
	string Util::ConvertWstrToStr(const wstring& str) {
		int strSize = WideCharToMultiByte(
			CP_UTF8,0,
			str.c_str(),(int)str.size(),
			nullptr,0,nullptr,nullptr
		);
		string result(strSize, 0);

		WideCharToMultiByte(CP_UTF8,0,
			str.c_str(),(int)str.size(),
			&result[0],strSize,
			nullptr,nullptr
		);

		return result;
	}


	void DxUtil::CreateMeshBuffer(ID3D12Device* _dev, Mesh& mesh) {
		mesh.m_VertexBuffer = make_shared<VertexBuffer>();
		mesh.m_IndexBuffer = make_shared<IndexBuffer>();

		mesh.m_VertexBuffer->Create(_dev, mesh.m_Vertices);
		mesh.m_IndexBuffer->Create(_dev, mesh.m_Indices);
	}
}