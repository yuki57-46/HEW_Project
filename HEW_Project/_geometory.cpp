#include "Geometory.h"

void Geometory::MakeBox()
{
	// ���f�[�^�̍쐬
	Vertex vtx[] = {
		// -Z��
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
		//+Z��
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
		//-X��
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
		//+X��
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
		//-Y��
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
		//+Y��
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
	};
	int idx[] = {
	 0,  1,  2,   1,  3,  2,	//-Z��
	 5,  4,  7,   4,  6,  7,	//+Z��
 	 8,  9, 10,   9, 11, 10,	//-X��
	12, 13, 14,  13, 15, 14,    //+X��
	18, 19, 16,  19, 17, 16,	//-Y��
	20, 21, 22,  21, 23, 22		//+Y��
	};
	// �o�b�t�@�̍쐬
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = _countof(vtx); //�z��̃f�[�^�����J�E���g����}�N��       //sizeof(vtx) / sizeof(vtx[0]); 
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = _countof(idx);
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pBox = new MeshBuffer(desc);
}

void Geometory::MakeCylinder()
{
	//--- ���_�̍쐬
	// �V�ʁA���

	// ����

	//--- �C���f�b�N�X�̍쐬
	// �V�ʁA���

	// ����


	//--- �o�b�t�@�̍쐬
}

void Geometory::MakeSphere()
{
	//--- ���_�̍쐬

	//--- �C���f�b�N�X�̍쐬

	// �o�b�t�@�̍쐬
}



void Geometory::MakeTriangle()
{
	// ���_�̍쐬
	Vertex vtx[] = {
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},  // ����
		{{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},   // �E��
		{{0.0f, -0.5f, 0.0f}, {0.5f, 1.0f}}    // ��ӂ̒���
	};

	// �C���f�b�N�X�̍쐬
	int idx[] = { 0, 1, 2 };

	// �o�b�t�@�̍쐬
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = _countof(vtx);
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = _countof(idx);
	desc.idxSize = sizeof(int);
	desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pTriangle = new MeshBuffer(desc);
}