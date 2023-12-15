#include "Player.h"
#include "Input.h"


//InputManager imanagerP = InputManager();



DirectX::XMFLOAT3 MinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);  //境界の最小値
DirectX::XMFLOAT3 MaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);     //最大値

DirectX::XMFLOAT3 HMinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);  //境界の最小値
DirectX::XMFLOAT3 HMaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);     //最大値


Player::Player()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationY(0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	,ok (false)
	, m_anime_Levitation(NULL)
	, m_anime_possession(NULL)
{
	m_pModel = new Model;
	 //モデルの読み込み処理
	if (!m_pModel->Load("Assets/Model/Player/kuroko.fbx", 0.5f /*, Model::Flip::XFlip*/)) {
		MessageBox(NULL, "モデルの読み込みエラー", "Error", MB_OK);
	}
	// モデルにShaderListからVS,PSを読み込む
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	//m_pVS = new VertexShader();
	//if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	//{
	//	MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	//}
	//m_pModel->SetVertexShader(m_pVS);

	m_anime_Levitation = m_pModel->AddAnimation("./Assets/Animation/kuroko_huyu.fbx");	//	ファイルパスを入れる
	m_anime_possession = m_pModel->AddAnimation("./Assets/Animation/kuroko_hyoui.fbx");

	minBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	maxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);

	hminBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	hmaxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);

}

Player::~Player()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	//if (m_pVS)
	//{
	//	delete m_pVS;
	//	m_pVS = nullptr;
	//}
}

void Player::Update(float tick)
{
	//憑依解除時に憑依した時の位置にプレイヤーを戻すため
	if (ok==false)
	{
		m_oldPos = m_pos;
	}

	m_pModel->Step(tick);

	if (ok == false)
	{
		m_pModel->Play(m_anime_Levitation, true);	// 浮遊アニメーション(常時)
	}
	else
	{
		m_pModel->Play(m_anime_possession, false);	// ループ無しアニメーション
	}


	//m_pCamera->Update();
	//ゲームパッドの対応
	/*imanagerP.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
	imanagerP.inspect();
*/
	float moveSpeed = 0.03f; // 移動速度の調整

	float rotationSpeed = 10.0f;

	// 左スティックのX軸とY軸方向の入力を取得
	/*float leftStickX1 = static_cast<float>(imanagerP.getKey(0));
	float leftStickX2 = static_cast<float>(imanagerP.getKey(1));
	float leftStickZ1 = static_cast<float>(imanagerP.getKey(2));
	float leftStickZ2 = static_cast<float>(imanagerP.getKey(3));*/




	// 移動方向ベクトルを計算
	//DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	// 移動方向ベクトルを正規化
	//DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	//directionVector = DirectX::XMVector3Normalize(directionVector);
	//DirectX::XMFLOAT3 normalizedDirection;
	//DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	//// 移動方向ベクトルから回転角度を計算
	//float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//m_rotationY = rotationAngle;

	// 位置を更新


	/*m_pos.x -= moveSpeed * moveDirection.x;
	m_pos.z -= moveSpeed * moveDirection.z;*/

	// 回転行列を更新
	//m_rotationMatrix = DirectX::XMMatrixRotationY(m_rotationY);

	//if (moveDirection.x == 0.0f && moveDirection.z == 0.0f)
	//{
	//	// 最後に向いていた方向を使用
	//	m_rotationY = m_lastFacingDirection;
	//}
	//else
	//{
	//	// 移動方向ベクトルから回転角度を計算
	//	float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//	m_rotationY = rotationAngle;

	//	// 最後に向いた方向を更新
	//	m_lastFacingDirection = m_rotationY;
	//}



	if (IsKeyPress(VK_UP) && IsKeyPress(VK_RIGHT) || IsKeyPress('W') && IsKeyPress('D'))
	{
		m_pos.z += moveSpeed;
		m_pos.x += moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = -135.0f;
	}
	else if (IsKeyPress(VK_UP) && IsKeyPress(VK_LEFT) || IsKeyPress('W') && IsKeyPress('A'))
	{
		m_pos.z += moveSpeed;
		m_pos.x -= moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = 135.0f;
	}
	else if (IsKeyPress(VK_DOWN) && IsKeyPress(VK_RIGHT) || IsKeyPress('S') && IsKeyPress('D'))
	{
		m_pos.z -= moveSpeed;
		m_pos.x += moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = -45.0f;
	}
	else if (IsKeyPress(VK_DOWN) && IsKeyPress(VK_LEFT) || IsKeyPress('S') && IsKeyPress('A'))
	{
		m_pos.z -= moveSpeed;
		m_pos.x -= moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = 45.0f;
	}
	else if (IsKeyPress(VK_UP) || IsKeyPress('W'))
	{
		m_pos.z += moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = -180.0f;
	}
	else if (IsKeyPress(VK_DOWN) || IsKeyPress('S'))
	{
		m_pos.z -= moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = 0.0f;
	}
	else if (IsKeyPress(VK_RIGHT) || IsKeyPress('D'))
	{
		m_pos.x += moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = -90.0f;
	}
	else if (IsKeyPress(VK_LEFT) || IsKeyPress('A'))
	{
		m_pos.x -= moveSpeed;
		SetBounds(MinBound, MaxBound);  //最小値と最大値をセット
		m_rotationY = 90.0f;
	}




	HSetBounds(HMinBound, HMaxBound);
}

void Player::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列の計算に回転を追加
	DirectX::XMMATRIX MT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MR = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_rotationY)); // Y軸回転
	DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX world = MS * MR * MT;//MR* MT; // 回転を適用

	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = viewMatrix;
	mat[2] = projectionMatrix;

	 //行列をシェーダーへ設定
	//m_pVS->WriteBuffer(0, mat);
	// m_pModel->Draw();
	ShaderList::SetWVP(mat);	// 転置済みの変換行列
	m_pModel->Draw(nullptr, [this](int index)	// ラムダ式
		{
			const Model::Mesh* pMesh = m_pModel->GetMesh(index);
			const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
			ShaderList::SetMaterial(*pMaterial);

			DirectX::XMFLOAT4X4 bones[200];
			for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
			{
				// この計算はゲームつくろー「スキンメッシュの仕組み」が参考になる
				DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
					pMesh->bones[i].invOffset *
					m_pModel->GetBone(pMesh->bones[i].index)
				));
			}
			ShaderList::SetBones(bones);
		});

}

void Player::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Player::GetminBox()
{
	return minBound;
}

DirectX::XMFLOAT3 Player::GetmaxBox()
{
	return maxBound;
}


DirectX::XMFLOAT3 Player::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//posに最小値、最大値を足して当たり判定をずらす
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}



//憑依当たり判定
void Player::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Player::HGetminBox()
{
	return hminBound;
}

DirectX::XMFLOAT3 Player::HGetmaxBox()
{
	return hmaxBound;
}


DirectX::XMFLOAT3 Player::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//posに最小値、最大値を足して当たり判定をずらす
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//__


//ブロックとプレイヤー衝突時、プレイヤーの位置を返す
void Player::PlayerPos()
{
	m_pos = m_oldPos;
}

//憑依時に飛ばす
void Player::HPlayerPos()
{
	m_pos.y = 100.0f;
}

//リセット用（未実装）
void Player::RPlayerPos()
{
	m_pos.z = 0.0f;
	m_pos.x = 0.0f;

}


bool Player::Set()
{
	return true;
}
void Player::SetOk()
{
	ok = true;
}
void Player::SetNOk()
{
	ok = false;
}

/**
 * @brief アニメーションの設定
 * @param[in] なし
 * @return なし
 */
void Player::SetAnime()
{
	// 入れるアニメーションのセット

}

/**
 * @brief アニメーションの設定
 * @param[in] なし
 * @return なし
 */
void Player::SetAnime2()
{
	m_pModel->Play(m_anime_possession, false);	// 憑依の描画
}
