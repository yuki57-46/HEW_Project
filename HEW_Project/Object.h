#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include"Soundtest.h"

class Object : public GameObject
{
public:
	Object();
	~Object();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBounds();
	DirectX::XMFLOAT3 GetmaxBounds() ;
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 HGetminBounds();
	DirectX::XMFLOAT3 HGetmaxBounds();
	DirectX::XMFLOAT3 HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);


	void CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max);
	DirectX::XMFLOAT3 CGetminBounds();
	DirectX::XMFLOAT3 CGetmaxBounds();
	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	DirectX::XMFLOAT3 GetObjectMaxBounds();
	DirectX::XMFLOAT3 GetPos();

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
	void Set();
	void SetF();
	bool SetR();
	

	void OBJPos();
	void OBJPosy();
	void Modelchg();
	void Modelchg2();

	void Set1();
	void SetF1();

	void SetColgravity();
	void SetColgravityfalse();
	void framepls();
	void SetObjectTop();
	void MoveObject(float y);
	void SetSlope();
	void SetReverseSlope();

	bool SetR1();
	bool IsGravity();
	bool IsXZ();
	bool IsObjectTop();
	bool IsMove();
private:
	Model* m_pObjectModel;
	VertexShader* m_pObjectVS;
	DirectX::XMFLOAT3 m_oldPos;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_jmp;
	DirectX::XMFLOAT3 m_mmovespeed;

	float m_rotationY;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;
	bool moveok;
	

	bool ok;
	bool gravity;
	bool xz;
	bool objectTop;
	bool colgravity;
	float a = 0.0f;

	
	

	IXAudio2SourceVoice* m_pSVSEBlk; // サウンドソース
	XAUDIO2_BUFFER* m_pSDSEBlk; // サウンドバッファ

	DirectX::XMFLOAT3 objectMinBound;
	DirectX::XMFLOAT3 objectMaxBound;

	DirectX::XMFLOAT3 hobjectMinBound;
	DirectX::XMFLOAT3 hobjectMaxBound;

	DirectX::XMFLOAT3 cobjectMinBound;
	DirectX::XMFLOAT3 cobjectMaxBound;

	int frame = 30;


};

