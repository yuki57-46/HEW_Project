﻿#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Model::MakeMesh(const void* ptr, float scale, Flip flip)
{
	// 事前準備
	aiVector3D zero3(0.0f, 0.0f, 0.0f);
	aiColor4D one4(1.0f, 1.0f, 1.0f, 1.0f);
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);
	//float zflip = flip == Flip::ZFlip ? -1.0f : 1.0f;
	//int idx1 = flip != Flip::None ? 2 : 1;
	//int idx2 = flip != Flip::None ? 1 : 2;
	float zflip = (flip == Flip::ZFlip || flip == Flip::ZFlipUseAnime) ? -1.0f : 1.0f;
	int idx1 = (flip == Flip::XFlip || flip == Flip::ZFlip) ? 2 : 1;
	int idx2 = flip != Flip::None ? 1 : 2;

	// メッシュの作成
	m_meshes.resize(pScene->mNumMeshes);
	for (unsigned int i = 0; i < m_meshes.size(); ++i)
	{
		// 頂点の作成
		m_meshes[i].vertices.resize(pScene->mMeshes[i]->mNumVertices);
		
		for (unsigned int j = 0; j < m_meshes[i].vertices.size(); ++j)
		{
			// モデルデータから値取得
			// 座標
			aiVector3D pos = pScene->mMeshes[i]->mVertices[j];

			// 法線
			aiVector3D normal = pScene->mMeshes[i]->HasNormals() ? pScene->mMeshes[i]->mNormals[j] : zero3;

			// UV
			aiVector3D uv = pScene->mMeshes[i]->HasTextureCoords(0) ? pScene->mMeshes[i]->mTextureCoords[0][j] : zero3;

			// 頂点に設定されている色
			aiColor4D color = pScene->mMeshes[i]->HasVertexColors(0) ? pScene->mMeshes[i]->mColors[0][j] : one4;

			// 値を設定
			m_meshes[i].vertices[j] =
			{
				DirectX::XMFLOAT3(pos.x * scale, pos.y * scale,pos.z * scale),
				DirectX::XMFLOAT3(normal.x, normal.y, normal.z),
				DirectX::XMFLOAT2(uv.x, uv.y),
				DirectX::XMFLOAT4(color.r, color.g, color.b, color.a),
			};
		}
		// 頂点の作成後、MakeWeightを呼び出す
		MakeWeight(pScene, i);

		// インデックスの作成
		m_meshes[i].indices.resize(pScene->mMeshes[i]->mNumFaces * 3);
		for (unsigned int j = 0; j < pScene->mMeshes[i]->mNumFaces; ++j)
		{
			aiFace face = pScene->mMeshes[i]->mFaces[j];
			int idx = j * 3;

			m_meshes[i].indices[idx + 0] = face.mIndices[0];
			m_meshes[i].indices[idx + 1] = face.mIndices[idx1];
			m_meshes[i].indices[idx + 2] = face.mIndices[idx2];
		}

		// マテリアルの割り当て
		m_meshes[i].materialID = pScene->mMeshes[i]->mMaterialIndex;

		// メッシュを元に頂点バッファ作成
		MeshBuffer::Description desc = {};
		desc.pVtx = m_meshes[i].vertices.data();
		desc.vtxSize = sizeof(Vertex);
		desc.vtxCount = (UINT)m_meshes[i].vertices.size();
		desc.pIdx = m_meshes[i].indices.data();
		desc.idxSize = sizeof(unsigned long);
		desc.idxCount = (UINT)m_meshes[i].indices.size();
		desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_meshes[i].pMesh = new MeshBuffer(desc);
	}
}
void Model::MakeMaterial(const void* ptr, std::string directory)
{
	// 事前準備
	const aiScene* pScene = reinterpret_cast<const aiScene*>(ptr);

	// マテリアルの作成
	m_materials.resize(pScene->mNumMaterials);
	for (unsigned int i = 0; i < m_materials.size(); ++i)
	{
		// 各種パラメーター
		aiColor3D color(0.0f, 0.0f, 0.0f);
		float shininess;

		// 拡散光の取得
		m_materials[i].diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) ==
			AI_SUCCESS ? DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// 環境光の取得
		m_materials[i].ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) ==
			AI_SUCCESS ? DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

		// 反射の強さを取得
		shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;

		// 反射光の取得
		m_materials[i].specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) ==
			AI_SUCCESS ? DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// テクスチャ
		HRESULT hr;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// テクスチャ領域確保
			m_materials[i].pTexture = new Texture;
			// そのまま探索
			hr = m_materials[i].pTexture->Create(path.C_Str());
			// モデルと同じ階層を探索
			if (FAILED(hr))
			{
				// パスからファイル名のみ取得
				std::string fullPath = path.C_Str();
				std::string::iterator strIt = fullPath.begin();
				while (strIt != fullPath.end())
				{
					if (*strIt == '/')
						*strIt = '\\';
					++strIt;
				}
				size_t find = fullPath.find_last_of("\\");
				std::string fileName = fullPath;
				if (find != std::string::npos)
					fileName = fileName.substr(find + 1);
				// テクスチャの読込
				hr = m_materials[i].pTexture->Create((directory + fileName).c_str());
			}
			// テクスチャが見つからなかった
			if (FAILED(hr)) {
				delete m_materials[i].pTexture;
				m_materials[i].pTexture = nullptr;
#ifdef _DEBUG
				m_errorStr += path.C_Str();
#endif // !_DEBUG
			}
		}
		else {
			m_materials[i].pTexture = nullptr;
		}
	}
}
