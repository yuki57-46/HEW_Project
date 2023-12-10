#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>

// ���\�[�X�p�X
#define ASSET(path)	"Assets/"path

// �^�C�g��
#ifdef _DEBUG
//#define APP_TITLE "Define.h�Ń^�C�g�����ύX�ł���̂ŁA[�N���X�L������]�ɕς��Ă�������"
#define APP_TITLE "AT12D192 �R�c��C"
#else
#define APP_TITLE "Define.h�Ń^�C�g�����ύX�ł���̂ŁA�Q�[���̃^�C�g���ɕς��Ă�������"
#endif

// ��ʃT�C�Y
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)

// �O���b�h�T�C�Y
#define DEBUG_GRID_NUM (10) // �O���b�h���S����[�܂ł̐��̐�
#define DEBUG_GRID_MARGIN (1.0f) // �O���b�h�z�u��(���[�g�����Z)


#endif // __DEFINES_H__