#include "Geometory.h"

void Geometory::MakeBox()
{
	// ���f�[�^�̍쐬
	Vertex vtx[] = {
		// -Z��
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f} },

		// -Y��
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f} },

		// -X��
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f} },
		{ {-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f} },

		// +Z��
		{ { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f} },
		{ {-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f} },
		{ {-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f} },

		// +Y��
		{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f} },
		{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
		{ {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f} },
		{ { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },

		// +X��
		{ { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f} },
		{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
		{ { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
		{ { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f} },
	};

	int idx[] = {
		 0,  1,  2,   1,  3,  2,	// -Z��
		 4,  5,  6,   5,  7,  6,	// -Y��
		 8,  9, 10,   9, 11, 10,	// -X��
		12, 13, 14,  13, 15, 14,	// +Z��
		16, 17, 18,  17, 19, 18,	// +Y��
		20, 21, 22,  21, 23, 22,	// +X��
		
	};

	// �o�b�t�@�̍쐬
	MeshBuffer::Description desc = {};
	desc.pVtx		= vtx;
	desc.vtxCount	= _countof(vtx);	// ���_�̐�  sizeof(vtx) / sizeof(vtx[0]) 24
	desc.vtxSize	= sizeof(Vertex);	// ���_�������̃f�[�^�T�C�Y�𒲂ׂ� 20
	desc.pIdx		= idx;	
	desc.idxCount	= _countof(idx);	// �C���f�b�N�X�̐�
	desc.idxSize	= sizeof(int);		// �C���f�b�N�X�������̃f�[�^�T�C�Y 4
	desc.topology	= D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	// �����̂�\�����邽�߂̃v���~�e�B�u
	m_pBox			= new MeshBuffer(desc);
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