#include "ObjectManager.h"
#include"Input.h"

#define HBLOCK	1	// �߈ˉ\�u���b�N
#define DBLOCK  2	// �߈˕s�u���b�N
#define MBLOCK	3	// ���E�ړ��u���b�N

//InputManager imanagerO = InputManager();

ObjectMng::ObjectMng()
	: m_pObjects(nullptr)
	, m_pObjectsNot(nullptr)
	, m_pObjectsAuto(nullptr)
	, m_pPlayer(nullptr)
	, m_num(0)
	
{
	m_pObjectCamera = new CameraDebug();
	aabb = new AABB();
	haabb = new HAABB();
	caabb = new CAABB();
	
	m_pPlayer = new Player();
	struct Setting
	{
		float x, y, z, scaleX, scaleY, scaleZ, kind;
	};
	//�u���b�N�z�u.�X�P�[���w��
	Setting data[] = {
		{ 2.0f, 0.0f, 0.0f, 10.0f, 30.0f, 10.0f, HBLOCK},
		{-2.0f, 0.0f, 0.0f, 10.0f, 30.0f, 10.0f, HBLOCK},
		{0.0f, 0.0f, -3.0f, 10.0f, 30.0f, 10.0f, HBLOCK},
		{0.0f, 0.0f,  3.0f, 10.0f, 30.0f, 10.0f, DBLOCK},
		{-2.0f, 0.0f, 1.5f, 20.0f, 30.0f, 10.0f, MBLOCK},	// 12/01 �ǉ��v���O����
	};
	
	//�z��̗v�f�̐�����K�v�ȃu���b�N�����v�Z
	m_num = sizeof(data) / sizeof(data[0]);

	//�K�v�Ȑ������u���b�N���m��
	m_pObjects		= new Object[m_num];
	m_pObjectsNot	= new ObjectNot[m_num];
	m_pObjectsAuto	= new ObjectAutoMove[m_num];

	//�m�ۂ����u���b�N�ɏ����f�[�^��ݒ�
	for (int i = 0; i < m_num; i++)
	{		
		int kindInt = static_cast<int>(data[i].kind);  // float����int�ւ̕ϊ�

		switch (kindInt)
		{
		// �߈ˉ\�u���b�N
		case 1:
			m_pObjects[i].Create(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ);
				break;
		// �߈˕s�u���b�N
		case 2:
			m_pObjectsNot[i].CreateNot(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ);
			break;
		// ���E�ړ��u���b�N
		case 3:
			m_pObjectsAuto[i].CreateAuto(
				data[i].x, data[i].y, data[i].z,
				data[i].scaleX, data[i].scaleY, data[i].scaleZ);
			break;
		}

		// 12/08 �ǉ��v���O����
		//if (data[i].kind)
		//	m_pObjects[i].Create(
		//		data[i].x, data[i].y, data[i].z,
		//		data[i].scaleX, data[i].scaleY, data[i].scaleZ);
		//if (data[i].kind)
		//	m_pObjectsAuto[i].CreateAuto(
		//		data[i].x, data[i].y, data[i].z,
		//		data[i].scaleX, data[i].scaleY, data[i].scaleZ);
	}
}


ObjectMng::~ObjectMng()
{

	delete[] m_pObjects;
	delete[] m_pObjectsNot;
	delete[] m_pObjectsAuto;
	

	if (m_pObjectCamera)
	{
		delete m_pObjectCamera;
		m_pObjectCamera = nullptr;
	}
	if (aabb)
	{
		delete aabb;
		aabb = nullptr;
	}
	if (haabb)
	{
		delete haabb;
		haabb = nullptr;
	}
	if (caabb)
	{
		delete caabb;
		caabb = nullptr;
	}
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

void ObjectMng::Update()
{
	/*imanagerO.addKeycode(0, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_A);
	imanagerO.addKeycode(1, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerO.addKeycode(2, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_Y);
	imanagerO.addKeycode(3, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_X);

	imanagerO.inspect();

	float AB = static_cast<float>(imanagerO.getKey(0));
	float BB = static_cast<float>(imanagerO.getKey(1));
	float YB = static_cast<float>(imanagerO.getKey(2));
	float XB = static_cast<float>(imanagerO.getKey(3));
*/
	m_pPlayer->Update();

	for (int i = 0; i < m_num; i++)
	{
		int BlockType = 0;

		m_pObjects[i].Update();
		m_pObjectsNot[i].Update();
		m_pObjectsAuto[i].Update();

		//if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
		//{
		//	//�u���b�N�ƃv���C���[�Փ�
		//	if (m_pPlayer->IsCollidingWith(*gameObject)) {
		//		// �Փˎ��̏���
		//		m_pPlayer->PlayerPos();
		//	}
			switch (BlockType)
			{
				// �߈ˉ\�u���b�N
			case HBLOCK:
				//if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				//{
				//	//�u���b�N�ƃv���C���[�Փ�
				//	if (m_pPlayer->IsCollidingWith(*gameObject)) {
				//		// �Փˎ��̏���
				//		m_pPlayer->PlayerPos();
				//	}
				//	//�߈˂̂��߁E�u���b�N�ƃv���C���[�����������ꍇ
				//	if (m_pPlayer->HIsCollidingWith(*gameObject))
				//	{
				//		if (IsKeyPress('Q'))
				//		{
				//			m_pPlayer->SetOk();
				//			m_pPlayer->HPlayerPos();
				//			m_pObjects[i].Set();
				//			m_pObjects[i].Modelchg();
				//		}
				//	}
				//	//�߈ˉ���
				//	if (!m_pPlayer->HIsCollidingWith(*gameObject))
				//	{
				//		if (IsKeyPress('E'))	//(imanagerO.getKey(1) & 0b011)
				//		{
				//			m_pPlayer->SetNOk();
				//			m_pPlayer->PlayerPos();
				//			if (m_pObjects[i].SetR() == true)
				//			{
				//				m_pObjects[i].SetF();
				//			}
				//			m_pObjects[i].Modelchg2();
				//		}
				//	}
				//}
				break;

				// �߈˕s�u���b�N
			case DBLOCK:
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjectsNot[i]))
				{
					//�߈˂̂��߁E�u���b�N�ƃv���C���[�����������ꍇ(�߈˂��Ȃ�ver)
					if (m_pPlayer->HIsCollidingWith(*gameObject))
					{
						m_pPlayer->SetNOk();
						m_pPlayer->HPlayerPos();
						m_pObjectsNot[i].SetF();
						m_pObjectsNot[i].Modelchg();	
					}

				}
				break;

				// ���E�ړ��u���b�N
			case MBLOCK:
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjectsAuto[i]))
				{
					//float moveSpeed = 1.0f;
					//float time = static_cast<float>(timeGetTime()) * 0.001f;
					//float displacement = sin(time * moveSpeed);

					//�߈˂̂��߁E�u���b�N�ƃv���C���[�����������ꍇ(�߈˂��Ȃ�ver)
					if (m_pPlayer->HIsCollidingWith(*gameObject))
					{
						m_pPlayer->SetNOk();
						m_pPlayer->HPlayerPos();
						m_pObjectsAuto[i].SetF();
						m_pObjectsAuto[i].Modelchg();
					}
				}
				break;
		}

		if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
		{
			//�߈˂̂��߁E�u���b�N�ƃv���C���[�����������ꍇ
			if (m_pPlayer->HIsCollidingWith(*gameObject))
			{
				if (IsKeyPress('Q'))
//				if (IsKeyPress('Q') && m_pPlayer->HIsCollidingWith(*gameObject))	//(imanagerO.getKey(0) & 0b011)
				{
					m_pPlayer->SetOk();
					m_pPlayer->HPlayerPos();
					m_pObjects[i].Set();

					m_pObjects[i].Modelchg();
				}
			}
			//�߈ˉ���
			if (!m_pPlayer->HIsCollidingWith(*gameObject))
			{
				if (IsKeyPress('E'))	//(imanagerO.getKey(1) & 0b011)
				{
					m_pPlayer->SetNOk();
					m_pPlayer->PlayerPos();
					if (m_pObjects[i].SetR() == true)
					{
						m_pObjects[i].SetF();
					}
					m_pObjects[i].Modelchg2();
				}
			}
		}
		if (m_pObjects[i].SetR() == true)
		{
			for (int j = 0; j < m_num; j++)
			{
				if (j == i)
				{
					j = i + 1;
				}
				if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
				{
					if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
					{
						// �u���b�Ni�ƃu���b�Nj�̓����蔻��
						if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
						{
							// �Փ˂����ꍇ�̏���
							//MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
							//m_pObjects[i].GetF();

							m_pObjects[i].OBJPos();

						}
					}
				}
			}
		}
	}
}

void ObjectMng::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	
	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Draw(viewMatrix, projectionMatrix);
		m_pObjectsNot[i].Draw(viewMatrix, projectionMatrix);
		m_pObjectsAuto[i].Draw(viewMatrix, projectionMatrix);
	}
	
	DirectX::XMFLOAT4X4 mat[3];

	mat[1] = m_pObjectCamera->GetViewMatrix();
	mat[2] = m_pObjectCamera->GetProjectionMatrix();
	

	m_pPlayer->Draw(viewMatrix, projectionMatrix);
}

