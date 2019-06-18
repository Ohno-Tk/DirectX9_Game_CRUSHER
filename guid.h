/*=============================================================================

		�ē��Ŕ`��[ guid.h ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/09/27
-------------------------------------------------------------------------------
	���@Update
		2016/09/27
-----------------------------------------------------------------------------*/
#ifndef _GUID_H_
#define _GUID_H_
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR2 pos;	//	���W
	D3DXVECTOR2 size;	//	�傫��
	float move_X;	//	�ړ���
}GUIDSIGNBOARD;
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
void InitGuid( void );			//	�ē��̏�����
void UninitGuid( void );		//	�ē��̏I��
void UpdateGuid( void );		//	�ē��̍X�V
void DrawGuid( void );			//	�ē��̕`��
GUIDSIGNBOARD *GetGuid( void );	//	�ē����̎擾
#endif