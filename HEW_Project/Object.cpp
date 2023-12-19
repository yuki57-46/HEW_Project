#include "Object.h"
#include "Geometory.h"
#include "Input.h"
#include <chrono>

//minbound maxboundをメンバ変数に
//create部分にスケールと当たり判定をかけ合わせる処理を追加　yは追加で計算必要


//InputManager imanagerOB = InputManager();

//DirectX::XMFLOAT3 objectMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//プレイヤーとの当たり判定用
//DirectX::XMFLOAT3 objectMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//DirectX::XMFLOAT3 hobjectMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//憑依用
//DirectX::XMFLOAT3 hobjectMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//DirectX::XMFLOAT3 cobjectMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//ブロック同士用
//DirectX::XMFLOAT3 cobjectMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

std::chrono::steady_clock::time_point lastSoundPlayTime;
const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(3000);//再生時間三秒の時

int frame = 50;


Object::Object()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_mmovespeed(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_jmp(0.0f,0.0f,0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)
	, m_pSVSEBlk(nullptr)//スピーカ
	,m_pSDSEBlk(nullptr)//サウンドデータ
	, objectMinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
	, objectMaxBound(0.5f, 0.5f, 0.5f)
	, hobjectMinBound(-0.5f, -0.5f, -0.5f)
	, hobjectMaxBound(0.5f, 0.5f, 0.5f)
	, cobjectMinBound(-0.5f, -0.5f, -0.5f)
	, cobjectMaxBound(0.5f, 0.5f, 0.5f)
	, ok(true)
	, gravity(false)
	, xz(false)
	, colgravity(true)
	, objectTop(false)
{
	m_pObjectModel = new Model;

	/*if (!m_pObjectModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}*/
	/*if (!m_pObjectModel->Load("Assets/Model/Block/Slope.fbx",0.1,  Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}*/
	if (!m_pObjectModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}
	/*if (!m_pObjectModel->Load("Assets/Stage/Butai.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}*/
	m_pObjectVS = new VertexShader();
	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pObjectModel->SetVertexShader(m_pObjectVS);


	SetBounds(minBound, maxBound);
	HSetBounds(hminBound, hmaxBound);
	CSetBounds(cminBound, cmaxBound);
	
	m_pSDSEBlk = LoadSound("Assets/Sound/SE/Blockgaugokuoto_Oobayashi.wav");

}

Object::~Object()
{
	if (m_pObjectModel)
	{
		delete m_pObjectModel;
		m_pObjectModel = nullptr;
	}
	if (m_pObjectVS)
	{
		delete m_pObjectVS;
		m_pObjectVS = nullptr;
	}
	//m_pSVSEBlk->Stop();
}

void Object::Update()
{

	m_oldPos = m_pos;

	float moveSpeed = 0.03f; // 移動速度の調整
	float rotationSpeed = 10.0f;

	if (colgravity == true)
	{
		m_pos.y -= 0.05f;
	}

	if (m_pos.y <= 0.0f)
	{
		//ok = false;
		gravity = false;
	}

	/*imanagerOB.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerOB.inspect();*/



	//// 左スティックのX軸とY軸方向の入力を取得
	//float leftStickX1 = static_cast<float>(imanagerOB.getKey(0));
	//float leftStickX2 = static_cast<float>(imanagerOB.getKey(1));
	//float leftStickZ1 = static_cast<float>(imanagerOB.getKey(2));
	//float leftStickZ2 = static_cast<float>(imanagerOB.getKey(3));



	// 移動方向ベクトルを計算
	//DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	//// 移動方向ベクトルを正規化（長さが1になるように）
	//DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	//directionVector = DirectX::XMVector3Normalize(directionVector);
	//DirectX::XMFLOAT3 normalizedDirection;
	//DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	//// 移動方向ベクトルから回転角度を計算
	//float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//m_rotationY = rotationAngle;

	//if (moveok == true)//憑依時
	//{
	//	m_pos.x -= moveSpeed * moveDirection.x;
	//	m_pos.z -= moveSpeed * moveDirection.z;
	//}

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTime);

	//m_jmp = m_pos;

	if (moveok == true)
	{
		if (IsKeyPress(VK_UP) )
		{
			m_pos.z += moveSpeed;

			if (m_pos.y <= 0.0f)
			{
				if (elapsedTime >= soundInterval)
				{
					m_pSVSEBlk = PlaySound(m_pSDSEBlk);

					// 最後のサウンド再生時間を更新
					lastSoundPlayTime = currentTime;
				}
			}
			xz = true;
		}
		else if (IsKeyPress(VK_DOWN))
		{
			m_pos.z -= moveSpeed;
			if (m_pos.y <= 0.0f)
			{
				if (elapsedTime >= soundInterval)
				{
					m_pSVSEBlk = PlaySound(m_pSDSEBlk);

					// 最後のサウンド再生時間を更新
					lastSoundPlayTime = currentTime;
				}
			}
			xz = true;
		}
		else if (IsKeyPress(VK_RIGHT))
		{
			m_pos.x += moveSpeed;
			if (m_pos.y <= 0.0f)
			{
				if (elapsedTime >= soundInterval)
				{
					m_pSVSEBlk = PlaySound(m_pSDSEBlk);

					// 最後のサウンド再生時間を更新
					lastSoundPlayTime = currentTime;
				}
			}
			xz = true;
		}
		else if (IsKeyPress(VK_LEFT))
		{
			m_pos.x -= moveSpeed;
			if (m_pos.y <= 0.0f)
			{
				if (elapsedTime >= soundInterval)
				{
					m_pSVSEBlk = PlaySound(m_pSDSEBlk);

					// 最後のサウンド再生時間を更新
					lastSoundPlayTime = currentTime;
				}
			}
			xz = true;
		}
		else
		{
			xz = false;
		}

		if (IsKeyPress('U'))
		{
			m_pos.y -= moveSpeed;
		}
		if (IsKeyPress('I'))
		{
			m_pos.y += moveSpeed;
		}


		if (ok == false)
		{

			if (IsKeyPress(VK_SPACE))
			{
				frame -= moveSpeed * 0.01;
				// スペースキーが押されたら上昇を実行.ゲージを減少
			   //m_pos.y += 0.07f;
				m_pos.y += frame * 0.003f;

				if (m_pos.y > 2.5f)
				{
					m_pos.y = m_oldPos.y;
				}
			}
			if (frame <= 0 || !(IsKeyPress(VK_SPACE)))
			{
				m_pos.y -= 0.1f;
				gravity = true;
			}
			if (m_pos.y <= 0.0f&&frame <= 0)
			{
				frame = 50;
			}

		}
		else if (ok == true)
		{
			m_pos.y -= 0.05f;
			gravity = true;
			//m_jmp = m_pos;
		}
		
		//if (ok == true)
		//{
		//	m_pos.y -= 0.5f;
		//	gravity = true;
		//	//m_jmp = m_pos;
		//}
	}
	
		SetBounds(objectMinBound, objectMaxBound);  //最小値と最大値をセット
		HSetBounds(hobjectMinBound, hobjectMaxBound);//憑依用の当たり判定
		CSetBounds(cobjectMinBound, cobjectMaxBound);//ブロック同士の当たり判定


		if (m_pos.x >= 7.0f || m_pos.x <= -7.0f
			|| m_pos.z >= 7.0f || m_pos.z <= -5.0f
			|| m_pos.y >= 7.0f)
		{

			OBJPos();
		}
		if (m_pos.y <= 0.0f)
		{
			SetF1();
			OBJPosy();
			gravity = false;
		}	
}
void Object::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	DirectX::XMMATRIX world = MoS * MoT;
	//world = [ワールド行列の設定];
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 与えられた viewMatrix を使う
	mat[2] = projectionMatrix; // 与えられた projectionMatrix を使う
	
	m_pObjectVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
	m_pObjectModel->Draw();

}


void Object::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Object::GetminBounds() 
{
	return minBound;
}

DirectX::XMFLOAT3 Object::GetmaxBounds() 
{
	return maxBound;
}

DirectX::XMFLOAT3 Object::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//憑依当たり判定
void Object::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Object::HGetminBounds()
{
	return hminBound;
}

DirectX::XMFLOAT3 Object::HGetmaxBounds()
{
	return hmaxBound;
}

DirectX::XMFLOAT3 Object::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//ブロック同士
void Object::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Object::CGetminBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 Object::CGetmaxBounds()
{
	return cmaxBound;
}

DirectX::XMFLOAT3 Object::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

DirectX::XMFLOAT3 Object::GetObjectMaxBounds()
{
	return cobjectMaxBound;
}

DirectX::XMFLOAT3 Object::GetPos()
{
	return m_pos;
}

void Object::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;


	//
	objectMinBound.x *= m_scale.x;
	objectMinBound.y *= m_scale.y;
	objectMinBound.z *= m_scale.z;
	objectMaxBound.x *= m_scale.x;
	objectMaxBound.y *= m_scale.y;
	objectMaxBound.z *= m_scale.z;

	SetBounds(objectMinBound, objectMaxBound);

	cobjectMinBound.x *= m_scale.x;
	cobjectMinBound.y *= m_scale.y;
	cobjectMinBound.z *= m_scale.z;
	cobjectMaxBound.x *= m_scale.x;
	cobjectMaxBound.y *= m_scale.y;
	cobjectMaxBound.z *= m_scale.z;

	//これがないとy軸の当たり判定おかしくなる

	if (cobjectMinBound.y < 0)
	{
		a = cobjectMinBound.y *= -1;
		cobjectMaxBound.y += a;

		cobjectMinBound.y = 0;
	}

	CSetBounds(cobjectMinBound, cobjectMaxBound);


	hobjectMinBound.x *= m_scale.x;
	hobjectMinBound.y *= m_scale.y;
	hobjectMinBound.z *= m_scale.z;
	hobjectMaxBound.x *= m_scale.x;
	hobjectMaxBound.y *= m_scale.y;
	hobjectMaxBound.z *= m_scale.z;


	if (cobjectMinBound.y < 0)
	{
		a = cobjectMinBound.y *= -1;
		cobjectMaxBound.y += a;

		cobjectMinBound.y = 0;
	}

	HSetBounds(hobjectMinBound, hobjectMaxBound);
	
	m_mmovespeed = m_pos;

}



//憑依判定用
void Object::Set()
{
	moveok = true;
}

void Object::SetF()
{
	moveok = false;
}

bool Object::SetR()
{
	return moveok;
}



//ブロック同士がぶつかった時に返す

void Object::OBJPos()
{
	m_pos = m_oldPos;
}

void Object::OBJPosy()
{
	m_pos.y = m_oldPos.y;
}

void Object::Modelchg()
{
	if (m_pObjectModel->Load("Assets/Model/test_model/test_block.fbx",  Model::Flip::XFlip));
}

void Object::Modelchg2()
{
	if (m_pObjectModel->Load("Assets/Model/Block/BoxS.fbx",  Model::Flip::XFlip));
}

void Object::Set1()
{
	ok = true;
}

void Object::SetF1()
{
	ok = false;
}

bool Object::SetR1()
{
	return ok;
}

bool Object::IsGravity()
{
	return gravity;
}

bool Object::IsXZ()
{
	return xz;
}

void Object::MoveObject(float y)
{
	m_pos.y = y;
}
bool Object::IsObjectTop()
{
	return objectTop;
}

void Object::SetObjectTop()
{
	if (objectTop == true)
	{
		objectTop = false;
	}
	else/* if (colgravity == false)*/
	{
		objectTop = true;
	}
}

void Object::SetColgravity()
{
	if (colgravity == true)
	{
		colgravity = false;
	}
	else/* if (colgravity == false)*/
	{
		colgravity = true;
	}
}

void Object::framepls()
{
	frame=50;
}