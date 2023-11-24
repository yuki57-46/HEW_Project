#include "Player.h"
#include "Input.h"
#include "PlayerCamera.h"
#include "Sprite.h"
#include "Texture.h"

Player::Player()
    : m_pos(0.0f, 0.0f, 0.0f)
    , m_pCamera(nullptr)
    , m_pShadowTex(nullptr)
    , m_pMarker(nullptr)
    , m_pMoveEmitter(nullptr)
{
    m_pModel = new Model();
    if(!m_pModel->Load("Assets/Model/Squid_LOD0.fbx", 0.025f, Model::XFlip))
    {
        MessageBox(NULL, "Model", "Error", MB_OK);
    }
    
    m_pShadowTex = new Texture();
    if(FAILED(m_pShadowTex->Create("Assets/Texture/Shadow.png")))
    {
        MessageBox(NULL, "Shadow", "Error", MB_OK);
    }
    m_pMarker = new Texture();
    if (FAILED(m_pMarker->Create("Assets/Texture/Marker.png")))
    {
        MessageBox(NULL, "Marker", "Error", MB_OK);
    }

    m_pVS = new VertexShader();
    if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
    {
        MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
    }

    m_pPS = new PixelShader();
 //   if (FAILED(m_pPS->Load("Assets/Shader/PS_Gray_test.cso")))
 //   {
	//	MessageBox(nullptr, "PS_Model.cso", "Error", MB_OK);
	//}


    m_pModel->SetVertexShader(m_pVS);
    //m_pModel->SetPixelShader(m_pPS);
    m_pEffect = new Effect();
    m_pMoveEmitter = new MoveEmitter();
    m_pEffect->AddEmitter(m_pMoveEmitter);
}

Player::~Player()
{
    SAFE_DELETE(m_pMoveEmitter);
    SAFE_DELETE(m_pEffect);

    if (m_pMarker)
    {
        delete m_pMarker;
        m_pMarker = nullptr;
    }
    if (m_pShadowTex)
    {
        delete m_pShadowTex;
        m_pShadowTex = nullptr;
    }
    if (m_pPS)
    {
		delete m_pPS;
		m_pPS = nullptr;
	}
    if(m_pVS)
    {
        delete m_pVS;
        m_pVS = nullptr;
    }
    if(m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }
}

void Player::Update()
{
    DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();
    DirectX::XMFLOAT3 camLook = m_pCamera->GetLook();
    DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);
    DirectX::XMVECTOR vCamLook = DirectX::XMLoadFloat3(&camLook);

    DirectX::XMVECTOR vFront; // カメラの正面ベクトル
    vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos); // カメラの位置と注視点から正面ベクトルを計算
    vFront = DirectX::XMVector3Normalize(vFront);       // 正面ベクトルを正規化

    // ベクトルを90°回転させる行列を計算
    DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90.0f));
    // 正面方向のベクトルに回転行列を適用させたベクトル
    DirectX::XMVECTOR vSide = DirectX::XMVector3Transform(vFront, matRotSide);
    // 0で初期化
    DirectX::XMVECTOR vMove = DirectX::XMVectorZero();
    const float Speed = 0.1f;
    bool isMove = false;
    // [W,A,S,Dでプレイヤーを移動させる処理]
    if (IsKeyPress('W')) { isMove = true; vMove = DirectX::XMVectorAdd(vMove, vFront); }
    if(IsKeyPress('S')) { isMove = true; vMove = DirectX::XMVectorSubtract(vMove, vFront); }
    if(IsKeyPress('D')) { isMove = true; vMove = DirectX::XMVectorAdd(vMove, vSide); }
    if(IsKeyPress('A')) { isMove = true; vMove = DirectX::XMVectorSubtract(vMove, vSide); }

    vMove = DirectX::XMVectorMultiply(vMove, DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f));
    // 移動ベクトルの正規化
    vMove = DirectX::XMVector3Normalize(vMove);
    // 移動ベクトルの移動速度を変更(定数倍)
    vMove = DirectX::XMVectorScale(vMove, Speed);

    DirectX::XMFLOAT3 move; // 計算した移動ベクトルの格納先
    DirectX::XMStoreFloat3(&move, vMove); // XMVECTOR型からXMFLOAT3型に変換し、moveに格納
    m_pos.x += move.x; m_pos.y += move.y; m_pos.z += move.z; // プレイヤーの座標を更新

    m_pEffect->Update();
    m_pEffect->SetPos(m_pos);
    // エフェクトを再生していない, 動いている
    if (!m_pEffect->IsPlay() && isMove)
    {
        m_pEffect->Play();
    }
}

void Player::Draw()
{
    RenderTarget* pRTV = GetDefaultRTV();
    DepthStencil* pDSV = GetDefaultDSV();
    // カメラがなければ処理を行わない
    if(!m_pCamera) { return; }


    // 足元の影の表示の設定など
    // 足元に影として表示されるようにワールド座標を計算する
    SetRenderTargets(1, &pRTV, nullptr);
    DirectX::XMMATRIX _mat = DirectX::XMMatrixTranslation(m_pos.x, 0.0f, m_pos.z);
    _mat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f)) * _mat;
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(_mat));
    Sprite::SetWorld(world);
    Sprite::SetView(m_pCamera->GetViewMatrix());
    Sprite::SetProjection(m_pCamera->GetProjectionMatrix());
    Sprite::SetSize(DirectX::XMFLOAT2(0.5f, 0.5f));
    Sprite::SetTexture(m_pShadowTex);
    Sprite::Draw();


    SetRenderTargets(1, &pRTV, pDSV);
    // 行列の計算
    DirectX::XMFLOAT4X4 mat[3];
    DirectX::XMStoreFloat4x4(&mat[0],
        DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)));
    mat[1] = m_pCamera->GetViewMatrix();
    mat[2] = m_pCamera->GetProjectionMatrix();


    // Markerの表示設定など
    DirectX::XMFLOAT4X4 inv; // 逆行列(inverse)の格納先
    inv = m_pCamera->GetViewMatrix();

    // カメラの行列はGPUに渡す際に転置されているため、逆行列の計算のために一度元に戻す
    DirectX::XMMATRIX matInv = DirectX::XMLoadFloat4x4(&inv);
    matInv = DirectX::XMMatrixTranspose(matInv);

    // 移動成分は逆行列で打ち消す必要がないので、0を設定して移動を無視する。
    DirectX::XMStoreFloat4x4(&inv, matInv);
    inv._41 = inv._42 = inv._43 = 0.0f;

    matInv = DirectX::XMLoadFloat4x4(&inv);
    matInv = DirectX::XMMatrixInverse(nullptr, matInv);
    DirectX::XMStoreFloat4x4(&inv, matInv);
    
    // モデルの表示
    m_pVS->WriteBuffer(0, mat);
    m_pPS->Bind();
    m_pPS->WriteBuffer(0, &mat);
    m_pModel->Draw();


    // ワールド行列とビルボードの行列を掛け合わせたもの(複数行)
    DirectX::XMMATRIX _world = DirectX::XMMatrixMultiply(matInv, DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
    DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(_world));
    Sprite::SetWorld(mat[0]);
    Sprite::SetTexture(m_pMarker);
    Sprite::Draw();

    m_pEffect->SetBillboardMatrix(inv);
    m_pEffect->Draw(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
}
