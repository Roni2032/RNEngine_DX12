#include "stdafx.h"
#include "project.h"

namespace RNEngine {

	void Model::Load(ComPtr<ID3D12Device>& _dev, const string& filename) {
		Assimp::Importer importer;
		unsigned int readFlags = 0;
		readFlags |= aiProcess_FlipUVs; // UV�𔽓]������
		readFlags |= aiProcess_MakeLeftHanded; // ������W�n�ɕϊ�����
		readFlags |= aiProcess_Triangulate; // �O�p���i�Ǎ����x���Ȃ�j
		auto scene = importer.ReadFile(filename, readFlags);
		assert(scene);

		const UINT meshSize = scene->mNumMeshes;
		m_Meshes.reserve(meshSize);
		for (UINT i = 0; i < meshSize; i++) {
			auto& mesh = scene->mMeshes[i];
			UINT vertexSize = mesh->mNumVertices;

			Mesh newMesh;
			auto& vertices = newMesh.m_Verteces;
			vertices.reserve(vertexSize);

			for (UINT j = 0; j < vertexSize; j++) {
				// ���_���W
				XMFLOAT3 position(0, 0, 0);
				if (mesh->HasPositions())
					::CopyMemory(&position, &mesh->mVertices[j], sizeof(position));
				//// �@���x�N�g��
				//XMFLOAT3 normal(0, 0, 0);
				//if (mesh->HasNormals())
				//	::CopyMemory(&normal, &mesh->mNormals[j], sizeof(normal));
				// UV���W
				XMFLOAT2 uv(0, 0);
				if (mesh->HasTextureCoords(0))
					::CopyMemory(&uv, &mesh->mTextureCoords[0][j], sizeof(uv));
				vertices.push_back({ position, uv });
			}

			newMesh.m_VertexBuffer = make_shared<VertexBuffer>();
			newMesh.m_VertexBuffer->Create(_dev, vertices);

			auto& indices = newMesh.m_Indeces;
			auto faceSize = mesh->mNumFaces;
			indices.reserve(faceSize * 3);
			for (UINT j = 0; j < faceSize; j++) {
				for (UINT k = 0; k < 3; k++) {
					indices.push_back(mesh->mFaces[j].mIndices[k]);
				}
			}

			newMesh.m_IndexBuffer = make_shared<IndexBuffer>();
			newMesh.m_IndexBuffer->Create(_dev, indices);

			newMesh.m_MaterialIndex = mesh->mMaterialIndex;

			m_Meshes.push_back(newMesh);
		}
		
		auto numMaterials = scene->mNumMaterials;
		m_MaterialTextureName.reserve(numMaterials);
		for (int i = 0; i < numMaterials; i++) {
			auto& material = scene->mMaterials[i];
			
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			string imageName = path.C_Str();

			string filePath = filename;
			size_t pos = filePath.find_last_of("/");
			if (pos != string::npos) {
				filePath = filePath.substr(0, pos + 1);
			}
			filePath += imageName;

			ResourceManager::RegisterTexture(filePath);

			m_MaterialTextureName.push_back(filePath);
		}

		if (m_IsDebug) OutputDebug(scene);
	}

	void Model::Draw(ComPtr<ID3D12GraphicsCommandList> cmdList, const DescriptorHeap* heap,const ConstBuffer* constantBuffer) {
		for (auto& mesh : m_Meshes) {
			cmdList->SetDescriptorHeaps(1, heap->GetHeap().GetAddressOf());
			auto startHandle = heap->GetGPUHandle();
			auto handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
				startHandle,
				constantBuffer->GetCBVHandle(),
				heap->GetHeapSize()
			);
			cmdList->SetGraphicsRootDescriptorTable(0, handle);

			auto& textureName = m_MaterialTextureName[mesh.m_MaterialIndex];

			auto texture = ResourceManager::GetTextureBuffer(textureName);
			handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
				startHandle,
				texture->GetSRVHandle(),
				heap->GetHeapSize()
			);
			cmdList->SetGraphicsRootDescriptorTable(1, handle);

			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			cmdList->IASetVertexBuffers(0, 1, &mesh.m_VertexBuffer->m_VertexBufferView);
			cmdList->IASetIndexBuffer(&mesh.m_IndexBuffer->m_IndexBufferView);

			cmdList->DrawIndexedInstanced((UINT)mesh.m_IndexBuffer->GetIndexCount(), 1, 0, 0, 0);
		}
	}

	void Model::OutputDebug(const aiScene* scene) {
		ofstream ofs("../Assets/" + m_DebugName + ".txt");
		if (ofs)
		{
			ofs << "----- Mesh Data -----" << std::endl;
			auto numMeshes = scene->mNumMeshes;
			ofs << "Num Meshes : " << numMeshes << std::endl;

			// �e���b�V���̒��_�����o�͂���
			for (unsigned int i = 0; i < numMeshes; i++)
			{
				auto& mesh = scene->mMeshes[i];

				unsigned int numVertices = mesh->mNumVertices;

				// ���_�����o�͂���
				ofs << "mesh " << i << " : " << numVertices << std::endl;

			}
			ofs << std::endl << "----- Material Data -----" << std::endl;

			auto numMaterials = scene->mNumMaterials;
			ofs << "Num Materials : " << numMaterials << std::endl;

			for (unsigned int i = 0; i < numMaterials; i++) {
				auto& material = scene->mMaterials[i];
				ofs << "Material " << i << " : " << material->GetName().C_Str() << "  ";

				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				ofs << "=> " << path.C_Str() << std::endl;
			}
		}
	}

	void ModelRenderer::Init(XMFLOAT3 eye, XMFLOAT3 target) {
		m_Matrix.m_World = XMMatrixRotationY(0);

		XMFLOAT3 up(0, 1, 0);
		auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2,//��p��90��
			static_cast<float>(1280) / static_cast<float>(720),//�A�X��
			0.1f,//�߂���
			1000.0f//������
		);
		m_Matrix.m_ViewProjection = viewMat * projMat;

		m_ConstantBuffer = make_unique<ConstBuffer>();
		auto dev = RnEngine::g_pInstance->GetDevice()->GetPtr();
		m_ConstantBuffer->Create(dev, m_Matrix);
	}

	void ModelRenderer::SetMatrixWorld(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale) {
		m_Matrix.m_World = XMMatrixRotationY(rotation.y);
		m_Matrix.m_World *= XMMatrixTranslation(position.x, position.y, position.z);

		m_ConstantBuffer->Upadte(m_Matrix);
	}
}