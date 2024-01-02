#include "ObjectManager.h"
#include"Input.h"


//InputManager imanagerO = InputManager();


ObjectMng::ObjectMng()
	: m_pObjects(nullptr)
	, m_pYuka(nullptr)
	, m_pPlayer(nullptr)
	, m_pLift_obj(nullptr)
	, m_pLever(nullptr)
	, m_num(0)
	, m_num1(0)
	, m_num2(0)	
	, m_num3(0)
	,m_num4(0)
	
{
	m_pObjectCamera = new CameraDebug();
	aabb = new AABB();
	haabb = new HAABB();
	caabb = new CAABB();
	
	m_pPlayer = new Player();

	//ブロック配置.スケール指定
	struct Setting
	{
		float x, y, z, scaleX, scaleY, scaleZ;
	};
	Setting data[] = {
		//{ 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
		{-2.0f, 0.0f, 3.0f, 0.5f, 0.5f, 0.5f},
		{0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f},
		//{0.0f, 0.0f,  3.0f, 1.0f, 1.0f, 1.0f},

	};
	
	//配列の要素の数から必要なブロック数を計算
	m_num = sizeof(data) / sizeof(data[0]);

	//必要な数だけブロックを確保
	m_pObjects = new Object[m_num];
	//確保したブロックに初期データを設定
	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Create(
			data[i].x, data[i].y, data[i].z,
			data[i].scaleX, data[i].scaleY, data[i].scaleZ
		);
	}
	

//リフト
	struct Setting1
	{
		float x, y, z, scaleX, scaleY, scaleZ,lifth,liftl,lifts;
	};
	//ブロック配置.スケール指定
	Setting1 data1[] = {
		{ 3.0f, 3.5f, 0.0f, 1.0f, 0.25f, 0.5f,4.0f,1.5f,0.5f}
	};

	//配列の要素の数から必要なブロック数を計算
	m_num1 = sizeof(data1) / sizeof(data1[0]);

	//必要な数だけブロックを確保
	m_pLift_obj = new Lift_Obj[m_num1];
	//確保したブロックに初期データを設定
	for (int i = 0; i < m_num1; i++)
	{
		m_pLift_obj[i].Create(
			data1[i].x, data1[i].y, data1[i].z,
			data1[i].scaleX, data1[i].scaleY, data1[i].scaleZ
			,data1[i].lifth, data1[i].liftl,  data1[i].lifts
		);
	}

	//レバー
	struct Setting2
	{
		float x, y, z, scaleX, scaleY, scaleZ;
	};
	//ブロック配置.スケール指定
	Setting2 data2[] = {
		{ 4.0f, 3.0f, 0.0f, 0.5f, 0.5f, 0.5f}
	};

	//配列の要素の数から必要なブロック数を計算
	m_num2 = sizeof(data2) / sizeof(data2[0]);

	//必要な数だけブロックを確保
	m_pLever = new Lever[m_num2];
	//確保したブロックに初期データを設定
	for (int i = 0; i < m_num2; i++)
	{
		m_pLever[i].Create(
			data2[i].x, data2[i].y, data2[i].z,
			data2[i].scaleX, data2[i].scaleY, data2[i].scaleZ		
		);
	}

	//床

	struct Setting3
	{
		float x, y, z, scaleX, scaleY, scaleZ;
	};
	//ブロック配置.スケール指定
	Setting3 data3[] = {
		{ 0.0f, -0.15f, 1.0f, 30.0f, 0.3f, 10.0f},

	};

	//配列の要素の数から必要なブロック数を計算
	m_num3 = sizeof(data3) / sizeof(data3[0]);

	//必要な数だけブロックを確保
	m_pYuka = new Yuka[m_num3];
	//確保したブロックに初期データを設定
	for (int i = 0; i < m_num3; i++)
	{
		m_pYuka[i].Create(
			data3[i].x, data3[i].y, data3[i].z,
			data3[i].scaleX, data3[i].scaleY, data3[i].scaleZ
		);
	}



	// effect
	m_Effect = LibEffekseer::Create("Assets/effect/BoxSet.efkefc");
}


ObjectMng::~ObjectMng()
{
	delete[] m_pObjects;
	
	delete[] m_pLift_obj;

	delete[] m_pLever;

	delete[] m_pYuka;



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

void ObjectMng::Update(float tick)
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

	m_pPlayer->Update(tick);

	for (int y = 0; y < m_num3; y++)
	{
		m_pYuka[y].Update();
	}

	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Update();

		for (int a = 0; a < m_num1; a++)
		{
			//m_pLift_obj[a].Update();
			//if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
			//{
			//	if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
			//	{

			//		//ブロックの衝突
			//		if (m_pObjects[i].col(*lift))
			//		{
			//			m_pObjects[i].OBJPos();
			//		}
			//	}
			//}
			//for (int l = 0; l < m_num2; l++)
			//{

			//	m_pLever[l].Update();//レバーとプレイヤー
			//	if (Lever* lever = dynamic_cast<Lever*>(&m_pLever[l]))
			//	{
			//		if (m_pPlayer->IsCollidingWith(*lever))
			//		{
			//			if (IsKeyPress('3'))
			//			{
			//				m_pLift_obj[a].SetFLever();
			//			}
			//			if (IsKeyPress('4'))
			//			{
			//				m_pLift_obj[a].SetLever();
			//			}
			//		}
			//	}
			//}

			if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
			{
				if (Lift_Obj* lift = dynamic_cast<Lift_Obj*>(&m_pLift_obj[a]))
				{
					float liftposY = lift[a].GetMaxBounds().y;
					m_pLift_obj[a].Update();
					//ブロックの衝突
					if (m_pObjects[i].col(*lift))
					{
						//m_pObjects[i].OBJPosy();
						if (gameObject->GetCMinBounds().y + 0.1 >= lift->GetMaxBounds().y)
						{
							m_pObjects[i].OBJPosy();
						}
						else if (m_pObjects[i].IsXZ())
						{
							m_pObjects[i].OBJPos();
						}
						if (m_pLift_obj[a].IsMove())
						{
							m_pObjects[i].MoveObject(liftposY);
						}
					}
				}
			}
			for (int l = 0; l < m_num2; l++)
			{

				m_pLever[l].Update();//レバーとプレイヤー
				if (Lever* lever = dynamic_cast<Lever*>(&m_pLever[l]))
				{
					if (m_pPlayer->IsCollidingWith(*lever))
					{
						if (IsKeyPress('Q'))
						{
							m_pLift_obj[a].SetMoveTrue();
							m_pPlayer->SetOk();
							m_pPlayer->HPlayerPos();
						}
					}
					if (IsKeyPress('E'))
					{
						m_pLift_obj[a].SetMoveFalse();
						m_pPlayer->SetNOk();
						m_pPlayer->PlayerPos();
					}
				}
			}

			if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
			{
				//////ブロックとプレイヤー衝突
				//if (m_pPlayer->IsCollidingWith(*gameObject)) {
				//	// 衝突時の処理
				//	m_pPlayer->PlayerPos();
				//}
			}
			if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
			{
				//憑依のため・ブロックとプレイヤーが当たった場合
				if (m_pPlayer->HIsCollidingWith(*gameObject))
				{

					if (IsKeyPress('Q'))//(imanagerO.getKey(0) & 0b011)
					{
						//// effectこうしん
						m_EffectHandle = LibEffekseer::GetManager()->Play(m_Effect, m_pPlayer->GetPosX(), m_pPlayer->GetPosY(), m_pPlayer->GetPosZ());

						////移動させる時
						//Effekseer::Matrix43 EffecMat = LibEffekseer::GetManager()->GetBaseMatrix(m_EffectHandle);
						//EffecMat.Translation(0.0f, 1.0f, 0.0f);
						//LibEffekseer::GetManager()->SetBaseMatrix(m_EffectHandle, EffecMat);

						m_pPlayer->SetOk();
						m_pPlayer->HPlayerPos();
						m_pObjects[i].Set();
						m_pObjects[i].Set1();
						m_pObjects[i].SetColgravity();
						//m_pObjects[i].Modelchg();
					}
				}
				//憑依解除
				/*if (!m_pPlayer->HIsCollidingWith(*gameObject))
				{*/
				//if (IsKeyPress('E'))//(imanagerO.getKey(1) & 0b011)
				//{
				//	m_pPlayer->SetNOk();
				//	m_pPlayer->PlayerPos();
				//	if (m_pObjects[i].SetR() == true)
				//	{
				//		m_pObjects[i].SetF();
				//	}
				//	//m_pObjects[i].Modelchg2();
				//}

				if (IsKeyPress('E'))//(imanagerO.getKey(1) & 0b011)
				{
					if (m_pObjects[i].SetR() == true)
					{
						m_pPlayer->SetNOk();
						m_pPlayer->PlayerPos();
						for (int j = 0; j < m_num; j++)
						{
							m_pObjects[j].SetF();
							/*if (m_pObjects[i].SetR() == true)
							{
								m_pObjects[i].SetF();
							}*/
						}
						m_pObjects[i].SetF1();
						m_pObjects[i].SetColgravity();
						//m_pObjects[i].Update(); //重力を消してからUpdateを経ずにfalseに入っている
						//m_pObjects[i].Modelchg2();
					}
				}
				
			}

			if (m_pObjects[i].SetR() == true)//|| m_pObjects[i].SetR() == false)
			{
				for (int j = 0; j < m_num; j++)
				{
					if (j == i)
					{
						j++;
					}
					if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
					{
						if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
						{
							// ブロックiとブロックjの当たり判定
						//if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
						//{	
						//	if (m_pObjects[i].IsGravity())
						//	{
						//		if (gameObject->GetCMinBounds().y + 0.1 >= gameObject2->GetMaxBounds().y)
						//		{
						//			m_pObjects[i].OBJPosy();
						//		}
						//		else if (m_pObjects[i].IsXZ())
						//		{
						//			m_pObjects[i].OBJPos();
						//		}
						//	}
						//	else if (m_pObjects[i].IsXZ())
						//	{
						//		m_pObjects[i].OBJPos();
						//	}
						//}
								// ブロックiとブロックjの当たり判定
							if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
							{
								//m_pObjects[j].OBJPos();
								//MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
								if (m_pObjects[i].IsGravity())
								{
									if (gameObject->GetCMinBounds().y + 0.1 >= gameObject2->GetMaxBounds().y)
									{
										m_pObjects[i].framepls();
										m_pObjects[i].OBJPosy();
									}
									else if (m_pObjects[i].IsXZ())
									{
										m_pObjects[i].OBJPos();
									}
								}
								else if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.5)
								{
									for (int h = 0; h < m_num; h++)
									{
										m_pObjects[h].Set1();
									}
									m_pObjects[j].SetObjectTop();
									//m_pObjects[i].Set1();
									m_pObjects[j].Set();
									m_pObjects[j].OBJPosy();
									//m_pObjects[i].OBJPosy();
									//m_pObjects[i].Update();
									//m_pObjects[j].Update();
									//m_pObjects[j].OBJPos();
								}
								/*else if (m_pObjects[j].IsObjectTop() == true)
								{
									if (gameObject->GetMaxBounds().y <= gameObject2->GetCMinBounds().y + 0.5)
									{
										m_pObjects[j].SetObjectTop();
										m_pObjects[i].SetF1();
									}
								}*/
								else if (m_pObjects[i].IsXZ())
								{
									m_pObjects[i].OBJPos();
									m_pObjects[j].OBJPos();
									//m_pObjects[i].SetF();
								}
							}
							else
							{
								if (m_pObjects[i].IsObjectTop() == true)
								{
									//m_pObjects[i].OBJPos();
									m_pObjects[i].SetObjectTop();
									m_pObjects[i].SetF();

									//m_pObjects[j].OBJPos();
								}
							}
						}
					}
				}
			}

			if (m_pObjects[i].SetR() == false)
			{
				for (int j = 0; j < m_num; j++)
				{
					if (j == i)
					{
						j++;
					}
					if (GameObject* gameObject = dynamic_cast<GameObject*>(&m_pObjects[i]))
					{
						if (GameObject* gameObject2 = dynamic_cast<GameObject*>(&m_pObjects[j]))
						{

							// ブロックiとブロックjの当たり判定
							if (m_pObjects[i].col(*gameObject2) /*&& m_pObjects[j].col(*gameObject)*/)
							{
								if (gameObject->GetCMinBounds().y + 0.1 >= gameObject2->GetMaxBounds().y)
								{
									m_pObjects[i].OBJPosy();


								}
								else if (m_pObjects[j].IsXZ())
								{
									//if(m_pObjects[j].IsObjectTop() == true)
									//{ 
									//	m_pObjects[j].OBJPos();
									//	m_pObjects[j].SetObjectTop();
									//	m_pObjects[j].SetF1();
									//	//m_pObjects[j].OBJPos();
									//}
									//m_pObjects[i].OBJPos();
									for (int h = 0; h < m_num; h++)
									{
										if (h == i || h == j)
										{
											h++;
										}
										if (GameObject* gameObject3 = dynamic_cast<GameObject*>(&m_pObjects[h]))
										{
											if (gameObject2->GetMaxBounds().y <= gameObject3->GetCMinBounds().y + 0.5)
											{
												m_pObjects[j].OBJPos();
												m_pObjects[h].OBJPos();
												m_pObjects[h].SetObjectTop();
												m_pObjects[h].SetF1();
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}



void ObjectMng::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix,bool draw)
{
	//行列の計算
	DirectX::XMFLOAT4X4 effectmat[2];

	effectmat[0] = m_pObjectCamera->GetViewMatrix();
	effectmat[1] = m_pObjectCamera->GetProjectionMatrix();

	//effeksserに送る前に転置する前の状態に戻す
	DirectX::XMMATRIX effekMat = XMLoadFloat4x4(&effectmat[0]);
	effekMat = XMMatrixTranspose(effekMat);
	XMStoreFloat4x4(&effectmat[0], effekMat);

	DirectX::XMMATRIX effekMat2 = XMLoadFloat4x4(&effectmat[1]);
	effekMat2 = XMMatrixTranspose(effekMat2);
	XMStoreFloat4x4(&effectmat[1], effekMat2);

	LibEffekseer::GetManager()->SetScale(m_EffectHandle, 0.7f, 0.7f, 0.7);
	LibEffekseer::SetViewPosition(m_pObjectCamera->GetPos());
	LibEffekseer::SetCameraMatrix(effectmat[0], effectmat[1]);

	for (int i = 0; i < m_num; i++)
	{
		m_pObjects[i].Draw(viewMatrix, projectionMatrix);
		
	}
	for (int i = 0; i < m_num1; i++)
	{
		m_pLift_obj[i].Draw(viewMatrix, projectionMatrix);
	}
	for (int i = 0; i < m_num2; i++)
	{
		m_pLever[i].Draw(viewMatrix, projectionMatrix);
	}

	for (int i = 0; i < m_num3; i++)
	{
		m_pYuka[i].Draw(viewMatrix, projectionMatrix);
	}
	
	


	DirectX::XMFLOAT4X4 mat[3];

	mat[1] = m_pObjectCamera->GetViewMatrix();
	mat[2] = m_pObjectCamera->GetProjectionMatrix();
	
	if (draw==true)
	{
		m_pPlayer->Draw(viewMatrix, projectionMatrix);
	}


}

