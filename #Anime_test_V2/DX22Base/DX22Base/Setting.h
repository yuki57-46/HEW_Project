#pragma once

#include <stdio.h>
#include "Model.h"

// �\�����郂�f���̃p�X
#define MODEL_PATH ("Assets/Model/mixamo/Running.fbx")
//#define MODEL_PATH ("Assets/Model/mixamo/sotai_pinch.fbx")

// �\��������A�j���[�V�����̃p�X
// �A�j���[�V�����ƃ��f�������򂵂Ă���ꍇ��
#define ANIME_PATH ("Assets/Model/Animation/Running.fbx")
//#define ANIME_PATH ("Assets/Model/mixamo/sotai_pinch.fbx")

// �\������傫��
// 1.0f��FBX�̃T�C�Y�ʂ�
//#define MODEL_SCALE (0.6f)		// ���q�p 
#define MODEL_SCALE (0.01f)		// �z�z�p�i�����j���O�j

// ���f���̕\���̌���
// Model::None  ��DirectX�̕W���̕\���̎d��
// Model::XFlip ��Maya�̕\���̎d��
// Model::ZFlip ��DirectX�̕\���̎d��(Maya����180�x��]�������)
#define MODEL_FLIP (Model::XFlip)

