/*=============================================================================

		����[ tutorial.h ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/09/01
-------------------------------------------------------------------------------
	���@Update
		2016/09/01
			�E�v���g�^�C�v�錾�ǉ�
				void InitTutorial(void);	//	�`���[�g���A���̏�����
				void UninitTutorial(void);	//	�`���[�g���A���̏I��
				void UpdateTutorial(void);	//	�`���[�g���A���̍X�V
				void DrawTutorial(void);	//	�`���[�g���A���̕`��
-----------------------------------------------------------------------------*/
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define MAX_TUTORIAL ( 5 )
/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR2 pos;	//	���W
	D3DXVECTOR2 size;	//	�傫��
	float Move_X;	//	�ړ���
}TUTORIAL;
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
void InitTutorial(void);	//	�`���[�g���A���̏�����
void UninitTutorial(void);	//	�`���[�g���A���̏I��
void UpdateTutorial(void);	//	�`���[�g���A���̍X�V
void DrawTutorial(void);	//	�`���[�g���A���̕`��
TUTORIAL *GetTutorial( void );	//	�`���[�g���A�����̎擾
#endif