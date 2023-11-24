#include "Geometory.h"

void Geometory::MakeBox()
{
	// 元データの作成
	Vertex vtx[] = {
		// -Z面
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f} },
		// Z面
		{ {-0.5f,  0.5f, 0.5}, {0.0f, 0.0f} },
		{ {-0.5f, -0.5f, 0.5}, {1.0f, 0.0f} },
		{ { 0.5f,  0.5f, 0.5}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f, 0.5}, {1.0f, 1.0f} }, 
		// X面
		{ {-0.5,  0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {-0.5,  0.5f, -0.5f}, {1.0f, 0.0f} },
		{ {-0.5, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ {-0.5, -0.5f, -0.5f}, {1.0f, 1.0f} },
		// -X面
		{ {0.5,  0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {0.5, -0.5f,  0.5f}, {1.0f, 0.0f} },
		{ {0.5,  0.5f, -0.5f}, {0.0f, 1.0f} },
		{ {0.5, -0.5f, -0.5f}, {1.0f, 1.0f} },
		// Y面
		{ {-0.5f, 0.5f,  0.5f}, {0.0f, 0.0f} },
		{ { 0.5f, 0.5f,  0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, 0.5f, -0.5f}, {1.0f, 1.0f} },
		// -Y面
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f} },
	};
	int idx[] = {
		0, 1, 2, 1, 3, 2,	// -Z面
		4, 5, 6, 5, 7, 6,	// Z面
		8, 9, 10, 9, 11, 10,	// X面
		12, 13, 14, 13, 15, 14,	// -X面
		16, 17, 18, 17, 19, 18,	// -Y面
		20, 21, 22, 21, 23, 22,	// Y面
	};

	// バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = _countof(vtx);
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = _countof(idx);
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pBox = new MeshBuffer(desc);
}

void Geometory::MakeCylinder()
{
	std::vector<Vertex> topVertex;
	std::vector<Vertex> bottomVertex;
	std::vector<Vertex> sideVertex;
	
	int nNumSegments = 20; // セグメント数
	float fRadius = 1.0f; // 半径
	float fHeight = 2.0f; // 高さ
	//--- 頂点の作成
	// 天面、底面
	for (int i = 0; i <= nNumSegments; ++i)
	{
		float theta = static_cast<float>(i) / static_cast<float>(nNumSegments)*DirectX::XM_2PI;
		float x = fRadius * cosf(theta);
		float z = fRadius * sinf(theta);

		Vertex vtx;
		vtx.pos[0] = x;
		vtx.pos[1] = fHeight;
		vtx.pos[2] = z;

		topVertex.push_back(vtx);
	}


	for (int i = 0; i <= nNumSegments; ++i)
	{
		float theta = static_cast<float>(i) / static_cast<float>(nNumSegments)*DirectX::XM_2PI;
		float x = fRadius * cosf(theta);
		float z = fRadius * sinf(theta);

		Vertex vtx;
		vtx.pos[0] = x;
		vtx.pos[1] = fHeight;
		vtx.pos[2] = z;

		bottomVertex.push_back(vtx);
	}

	// 側面
	for (int i = 0; i <= nNumSegments; ++i)
	{
		float theta = static_cast<float>(i) / static_cast<float>(nNumSegments)*DirectX::XM_2PI;
		float x = fRadius * cosf(theta);
		float z = fRadius * sinf(theta);

		Vertex topVertex, bottomVertex;
		topVertex.pos[0] = x;
		topVertex.pos[1] = fHeight;
		topVertex.pos[2] = z;

		bottomVertex.pos[0] = x;
		bottomVertex.pos[1] = fHeight;
		bottomVertex.pos[2] = z;

		sideVertex.push_back(topVertex);
		sideVertex.push_back(bottomVertex);
	}

	//--- インデックスの作成
	std::vector<int> topIdx;
	std::vector<int> bottomIdx;
	std::vector<int> sideIdx;
	// 天面、底面


	// 側面


	//--- バッファの作成

}

void Geometory::MakeSphere()
{
	//--- 頂点の作成

	//--- インデックスの作成

	// バッファの作成
}