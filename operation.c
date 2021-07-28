/*=============================================================================

		������@[ operation.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/09/21
-------------------------------------------------------------------------------
	���@�쐬��
		2016/09/21
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "operation.h"
#include "result.h"
#include "pause.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define OPERATION_KEYBOARD_TEXTURENAME "data/TEXTURE/operation/������@01.jpg"
#define OPERATION_GAMEPAD_TEXTURENAME "data/TEXTURE/operation/������@02.jpg"
#define OPERATION_BUTTOM_TEXTURENAME "data/TEXTURE/func/ButtomRogo .png"
#define OPERATION_PAD_TEXTURENAME "data/TEXTURE/func/ButtomPad.png"

#define MAX_OPERATION_TEXTURE ( 2 )
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexOperation( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void ButtomFade( VERTEX_2D* pVtx );	//	�{�^���̃t�F�[�h
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferOperation = NULL;

//	�L�[�{�[�h����
LPDIRECT3DTEXTURE9 g_pTextureOperation_Keyboard = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�Q�[���p�b�h
LPDIRECT3DTEXTURE9 g_pTextureOperation_GamePad = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�{�^��
LPDIRECT3DTEXTURE9 g_pTextureOperation_Buttom = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

FADE g_OperationFade = FADE_IN;	//	�t�F�[�h���
D3DXCOLOR g_OperationFadeColor;	//	�t�F�[�h�F

/*-----------------------------------------------------------------------------
 �֐���:	void InitOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		������@�̏�����
-----------------------------------------------------------------------------*/
void InitOperation( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�G���[�`�F�b�N

	//	�Q�[���p�b�h������������
	if( GamePad[ 0 ] == NULL )
	{
		//	�L�[�{�[�h
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_KEYBOARD_TEXTURENAME , &g_pTextureOperation_Keyboard  ) ) )
		{
			MessageBox( NULL , "������@�O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if

		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_BUTTOM_TEXTURENAME , &g_pTextureOperation_Buttom  ) ) )
		{
			MessageBox( NULL , "����{�^���O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	�Q�[���p�b�h����������
	else if( GamePad[ 0 ] != NULL )
	{
		//	�Q�[���p�b�h
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_GAMEPAD_TEXTURENAME , &g_pTextureOperation_Keyboard  ) ) )
		{
			MessageBox( NULL , "������@�O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if

		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_PAD_TEXTURENAME , &g_pTextureOperation_Buttom  ) ) )
		{
			MessageBox( NULL , "����{�^���O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	

	//	������
	g_OperationFadeColor = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	���_�̍쐬
	MakeVertexOperation( pDevice );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		������@�̏I��
-----------------------------------------------------------------------------*/
void UninitOperation( void )
{
	if( g_pVtxBufferOperation != NULL )
	{
		g_pVtxBufferOperation -> Release();
		g_pVtxBufferOperation = NULL;
	}	//	end of if

	//	������@�O�P
	if( g_pTextureOperation_Keyboard != NULL )
	{
		g_pTextureOperation_Keyboard -> Release();
		g_pTextureOperation_Keyboard = NULL;
	}	//	end of if

	//	�{�^��
	if( g_pTextureOperation_Buttom != NULL )
	{
		g_pTextureOperation_Buttom -> Release();
		g_pTextureOperation_Buttom = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		������@�̍X�V
-----------------------------------------------------------------------------*/
void UpdateOperation( void )
{
	//	�|�[�Y�g�p�t���O�擾
	bool *PauseUse = GetOpeFlag();
	bool *Pause = GetPause();

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferOperation -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	�{�^���̃t�F�[�h
	ButtomFade( pVtx );

	g_pVtxBufferOperation -> Unlock();

	if( GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) )
	{
		*PauseUse = false;
		//*Pause = true;
	}

	if( GetGamePadTrigger( BUTTOM_04 ) )
	{
		*PauseUse = false;
		//*Pause = true;
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		������@�̕`��
-----------------------------------------------------------------------------*/
void DrawOperation( void )
{
	//	�|�[�Y�g�p�t���O�擾
	bool *PauseUse = GetOpeFlag();

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferOperation , 0 , sizeof( VERTEX_2D ) );

	if( *PauseUse ==true )
	{
		//	������@�O�P
		pDevice -> SetTexture( 0 , g_pTextureOperation_Keyboard );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

		//	������@�O�Q
		pDevice -> SetTexture( 0 , g_pTextureOperation_GamePad );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);

		//	�{�^��
		pDevice -> SetTexture( 0 , g_pTextureOperation_Buttom );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
	}


}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexOperation( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexOperation( LPDIRECT3DDEVICE9 pDevice )
{

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_OPERATION_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferOperation , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferOperation -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	�{�^��

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

	//	������@�O�P

	//	���_���W�̐ݒ�
	pVtx[ 4 ].pos = D3DXVECTOR3( 0.0f                , 0.0f                 , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f                 , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( 0.0f                , 0.0f + SCREEN_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f + SCREEN_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 4 ].rhw = 1.0f;
	pVtx[ 5 ].rhw = 1.0f;
	pVtx[ 6 ].rhw = 1.0f;
	pVtx[ 7 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 4 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );
	pVtx[ 5 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );
	pVtx[ 6 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );
	pVtx[ 7 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 4 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 5 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 6 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 7 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferOperation -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void ButtomFade( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�{�^���̃t�F�[�h
-----------------------------------------------------------------------------*/
void ButtomFade( VERTEX_2D* pVtx )
{
	if( g_OperationFade == FADE_IN )	//	�t�F�[�h�C�����̏���
	{
		g_OperationFadeColor.a -= FADE_RATE;	//	a�l�����Z���Č��̉�ʂ𕂂��яオ�点��
		//	�t�F�[�h�C���̏I��
		if( g_OperationFadeColor.a < 0.0f )
		{
			g_OperationFadeColor.a = 0.0f;
			g_OperationFade = FADE_OUT;
		}	//	end od if
	}	//	end of if

	else if( g_OperationFade == FADE_OUT )	//	�t�F�[�h�A�E�g���̏���
	{
		g_OperationFadeColor.a += FADE_RATE;	//	a�l�����Z���Č��̉�ʂ������Ă���
		//	�t�F�[�h�A�E�g�̏I��
		if( g_OperationFadeColor.a > 1.0f )
		{
			g_OperationFadeColor.a = 1.0f;
			g_OperationFade = FADE_IN;	//	�t�F�[�h�C���ɏ����̐؂�ւ�
		}	//	end of if
	}	//	end of else if

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );

}