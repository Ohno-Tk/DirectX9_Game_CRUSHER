/*=============================================================================

		���U���g[ result.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/06/29
-------------------------------------------------------------------------------
	���@�쐬��
		2016/08/23
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "result.h"
#include "fade.h"
#include "player.h"
#include "base.h"
#include "time.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define RESULT_TEXTURENAME "data/TEXTURE/result/result.jpg"

#define RESULT_PUSH_TEXTURENAME "data/TEXTURE/func/ButtomRogo .png"

#define RESULT_CLEAR_TEXTURENAME "data/TEXTURE/result/clear.png"

#define RESULT_GAMEOVER_TEXTURENAME "data/TEXTURE/result/gameover.png"

#define RESULT_PLAYER_TEXTURENAME "data/TEXTURE/game/player2.png"

#define RESULT_PAD_TEXTURENAME "data/TEXTURE/func/ButtomPad.png"


#define MAX_RESULT_TEXTURE ( 5 )	//	�e�N�X�`���̍ő吔

#define RESULT_POS_X ( 0.0f )		//	�|���S���̕\���ʒu�w
#define RESULT_POS_Y ( 0.0f )		//	�|���S���̕\���ʒu�x
#define RESULT_WIDTH ( 800.0f )		//	�|���S���̕�
#define RESULT_HEIGHT ( 600.0f )	//	�|���S���̍���

#define RESULT_CLEAR_POS_X ( 250.0f )	//	�N���APosX
#define RESULT_CLEAR_POS_Y ( 0.0f )		//	�N���APosY
#define RESULT_CLEAR_WIDTH ( 500.0f )	//	�N���AWIDTH
#define RESULT_CLEAR_HEIGHT ( 250.0f )	//	�N���AHEIGHT

#define RESULT_GAMEOVER_POS_X ( 200.0f )	//	�Q�[���I�[�o�[PosX
#define RESULT_GAMEOVER_POS_Y ( 0.0f )		//	�Q�[���I�[�o�[PosY
#define RESULT_GAMEOVER_WIDTH ( 500.0f )	//	�Q�[���I�[�o�[WIDTH
#define RESULT_GAMEOVER_HEIGHT ( 250.0f )	//	�Q�[���I�[�o�[HEIGHT

#define RESULT_PLAYER_POS_X ( 300.0f )	//	�v���C���[PosX
#define RESULT_PLAYER_POS_Y ( 200.0f )	//	�v���C���[PosY
#define RESULT_PLAYER_WIDTH ( 400.0f )	//	�v���C���[WIDTH
#define RESULT_PLAYER_HEIGHT ( 300.0f )	//	�v���C���[HEIGHT

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexresult( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void MakeVertexTex( VERTEX_2D* pVtx );	//	�e�N�X�`���ύX
void PushBottomFade( VERTEX_2D* pVtx );	//	�v�b�V���{�^���̃t�F�[�h
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferresult = NULL;

//	���U���g�w�i
LPDIRECT3DTEXTURE9 g_pTextureresult = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	Push�{�^��
LPDIRECT3DTEXTURE9 g_pTextureResult_Push = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�N���A
LPDIRECT3DTEXTURE9 g_pTextureResult_Clear = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�Q�[���I�[�o�[
LPDIRECT3DTEXTURE9 g_pTextureResult_Gameover = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�v���C���[
LPDIRECT3DTEXTURE9 g_pTextureResult_Player = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

FADE g_ResultFade = FADE_IN;	//	�t�F�[�h���
D3DXCOLOR g_ResultFadeColor;	//	�t�F�[�h�F

float Result_Uset = 0.0f , Result_Vset = 0.0f;
float Result_Bg_Uset = 0.0f , Result_Bg_Vset = 0.0f;
/*-----------------------------------------------------------------------------
 �֐���:	void InitResult( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		���U���g�̏�����
-----------------------------------------------------------------------------*/
void InitResult( void )
{
	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_TEXTURENAME , &g_pTextureresult  ) ) )
	{
		MessageBox( NULL , "���U���g�w�i�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_PLAYER_TEXTURENAME , &g_pTextureResult_Player  ) ) )
	{
		MessageBox( NULL , "���U���g�̃v���C���[�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�Q�[���p�b�h������������
	if( GamePad[ 0 ] == NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_PUSH_TEXTURENAME , &g_pTextureResult_Push  ) ) )
		{
			MessageBox( NULL , "Push�{�^���O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	�Q�[���p�b�h����������
	else if( GamePad[ 0 ] != NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_PAD_TEXTURENAME , &g_pTextureResult_Push  ) ) )
		{
			MessageBox( NULL , "Push�{�^���O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	�S�[��������
	if( Base -> GoolFalg == true )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_CLEAR_TEXTURENAME , &g_pTextureResult_Clear  ) ) )
		{
			MessageBox( NULL , "�N���A�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}	//	end of if

	else if( Base -> GoolFalg == false )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_GAMEOVER_TEXTURENAME , &g_pTextureResult_Gameover  ) ) )
		{
			MessageBox( NULL , "�Q�[���I�[�o�[�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}	//	end of else if

	MakeVertexresult( pDevice );	//	���_�̍쐬

	//	������
	Result_Uset = 0.0f;
	Result_Vset = 0.0f;

	Result_Bg_Uset = 0.0f;
	Result_Bg_Vset = 0.0f;

	g_ResultFadeColor = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	���y�Đ�
	PlaySound( SOUND_LABEL_BGM_RESULT );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitResult( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		���U���g�̏I��
-----------------------------------------------------------------------------*/
void UninitResult( void )
{
	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	Base -> GoolFalg = false;

	//	�w�i

	if( g_pTextureresult != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureresult -> Release();
		g_pTextureresult = NULL;
	}	//	end of if
	if( g_pVtxBufferresult != NULL )
	{
		g_pVtxBufferresult -> Release();
		g_pVtxBufferresult = NULL;
	}	//	end of if

	//	Push�{�^��
	if( g_pTextureResult_Push != NULL )
	{
		g_pTextureResult_Push -> Release();
		g_pTextureResult_Push = NULL;
	}	//	end of if

	//	�v���C���[
	if( g_pTextureResult_Player != NULL )
	{
		g_pTextureResult_Player -> Release();
		g_pTextureResult_Player = NULL;
	}	//	end of if

	//	�N���A
	if( g_pTextureResult_Clear != NULL )
	{
		g_pTextureResult_Clear -> Release();
		g_pTextureResult_Clear = NULL;
	}	//	end of if

	//	�Q�[���I�[�o�[
	if( g_pTextureResult_Gameover != NULL )
	{
		g_pTextureResult_Gameover -> Release();
		g_pTextureResult_Gameover = NULL;
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdeteResult( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		���U���g�̍X�V
-----------------------------------------------------------------------------*/
void UpdateResult( void )
{
	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	//	���̃��[�h
	MODE *Mode = GetMode();

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�t�F�[�h�����ǂ���
	bool *FadeUse = GetFadeUse();


	//	�S�[��������
	if( Base -> GoolFalg == true )
	{
		Result_Uset = 0.5f;
		Result_Vset = 0.5f;
	}	//	end of if
	else if( Base -> GoolFalg == false )
	{
		Result_Uset = 0.0f;
		Result_Vset = 0.5f;
	}	//	end of else if

	Result_Bg_Uset += 0.004f;
	Result_Bg_Vset -= 0.004f;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferresult -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	MakeVertexTex( pVtx );	//	�e�N�X�`���ύX
	
	PushBottomFade(pVtx );	//	�v�b�V���{�^���̃t�F�[�h

	g_pVtxBufferresult -> Unlock();

	if( *FadeUse == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
		{
			//	��ʑJ��
			if( GamePad[ CntPad ] != NULL )
			{
				if( GetGamePadTrigger( BUTTOM_04 ) )
				{
					//	���[�h���Q�[���̎�
					if( *Mode == MODE_GAME )
					{
						SetFade(FADE_OUT , MODE_RANKING);

						//	SE�Đ�
						PlaySound( SOUND_LABEL_SE_ENTER );

						//	���y��~
						StopSound( SOUND_LABEL_BGM_RESULT );
					}	//	end of if

					//	���[�h���`���[�g���A���̎�
					else if( *Mode == MODE_TUTORIAL )
					{
						SetFade(FADE_OUT , MODE_TITLE);
					}	//	end of else if

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	���y��~
					StopSound( SOUND_LABEL_BGM_RESULT );

				}		//	end of if
			}	//	end of if
			else if( GamePad[ CntPad ] == NULL )
			{

				if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
				{

					//	���[�h���Q�[���̎�
					if( *Mode == MODE_GAME )
					{
						SetFade(FADE_OUT , MODE_RANKING);

						//	SE�Đ�
						PlaySound( SOUND_LABEL_SE_ENTER );

						//	���y��~
						StopSound( SOUND_LABEL_BGM_RESULT );

					}	//	end of if

					//	���[�h���`���[�g���A���̎�
					else if( *Mode == MODE_TUTORIAL )
					{
						SetFade(FADE_OUT , MODE_TITLE);
				
					}	//	end of else if

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	���y��~
					StopSound( SOUND_LABEL_BGM_RESULT );

				}	//	end of if
			}	//	end of else if
		}	//	end of for
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawResult( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		���U���g�̕`��
-----------------------------------------------------------------------------*/
void DrawResult( void )
{
	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferresult , 0 , sizeof( VERTEX_2D ) );

	//	���U���g�w�i
	pDevice -> SetTexture( 0 , g_pTextureresult );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

	//	�v���C���[
	pDevice -> SetTexture( 0 , g_pTextureResult_Player );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16, NUM_POLYGON);


	//	�S�[��������
	if( Base -> GoolFalg == true )
	{
		//	�N���A
		pDevice -> SetTexture( 0 , g_pTextureResult_Clear );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);
	}
	else if( Base -> GoolFalg == false )
	{
		//	�Q�[���I�[�o�[
		pDevice -> SetTexture( 0 , g_pTextureResult_Gameover );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12, NUM_POLYGON);
	}

	//	Push�{�^��
	pDevice -> SetTexture( 0 , g_pTextureResult_Push );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexresult( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexresult( LPDIRECT3DDEVICE9 pDevice )
{

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_RESULT_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferresult , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferresult -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	�w�i

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_POS_X                , RESULT_POS_Y                 , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_POS_X + SCREEN_WIDTH , RESULT_POS_Y                 , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_POS_X                , RESULT_POS_Y + SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_POS_X + SCREEN_WIDTH , RESULT_POS_Y + SCREEN_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

	//	Push�{�^��

	//	���_���W�̐ݒ�
	pVtx[ 4 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 4 ].rhw = 1.0f;
	pVtx[ 5 ].rhw = 1.0f;
	pVtx[ 6 ].rhw = 1.0f;
	pVtx[ 7 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 4 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 5 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 6 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 7 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 4 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 5 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 6 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 7 ].tex = D3DXVECTOR2( 1 , 1 );

	//	�N���A

	//	���_���W�̐ݒ�
	pVtx[ 8  ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X                      , RESULT_CLEAR_POS_Y                       , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X + RESULT_CLEAR_WIDTH , RESULT_CLEAR_POS_Y                       , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X                      , RESULT_CLEAR_POS_Y + RESULT_CLEAR_HEIGHT , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X + RESULT_CLEAR_WIDTH , RESULT_CLEAR_POS_Y + RESULT_CLEAR_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 8  ].rhw = 1.0f;
	pVtx[ 9  ].rhw = 1.0f;
	pVtx[ 10 ].rhw = 1.0f;
	pVtx[ 11 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 8  ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 9  ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 10 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 11 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 8  ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 9  ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 10 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 11 ].tex = D3DXVECTOR2( 1 , 1 );

	//	�Q�[���I�[�o�[

	//	���_���W�̐ݒ�
	pVtx[ 12 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X                         , RESULT_GAMEOVER_POS_Y                          , 0.0f );
	pVtx[ 13 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X + RESULT_GAMEOVER_WIDTH , RESULT_GAMEOVER_POS_Y                          , 0.0f );
	pVtx[ 14 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X                         , RESULT_GAMEOVER_POS_Y + RESULT_GAMEOVER_HEIGHT , 0.0f );
	pVtx[ 15 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X + RESULT_GAMEOVER_WIDTH , RESULT_GAMEOVER_POS_Y + RESULT_GAMEOVER_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 12 ].rhw = 1.0f;
	pVtx[ 13 ].rhw = 1.0f;
	pVtx[ 14 ].rhw = 1.0f;
	pVtx[ 15 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 12 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 13 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 14 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 15 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 12 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 13 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 14 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 15 ].tex = D3DXVECTOR2( 1 , 1 );

	//	�v���C���[

	//	���_���W�̐ݒ�
	pVtx[ 16 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X                       , RESULT_PLAYER_POS_Y                          , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X + RESULT_PLAYER_WIDTH , RESULT_PLAYER_POS_Y                          , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X                       , RESULT_PLAYER_POS_Y + RESULT_PLAYER_HEIGHT , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X + RESULT_PLAYER_WIDTH , RESULT_PLAYER_POS_Y + RESULT_PLAYER_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 16 ].rhw = 1.0f;
	pVtx[ 17 ].rhw = 1.0f;
	pVtx[ 18 ].rhw = 1.0f;
	pVtx[ 19 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 16 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 17 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 18 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 19 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 17 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 19 ].tex = D3DXVECTOR2( 1 , 1 );

	g_pVtxBufferresult -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexTex( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�e�N�X�`���ύX
-----------------------------------------------------------------------------*/
void MakeVertexTex( VERTEX_2D* pVtx )
{
	//	���U���g�w�i

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 + Result_Bg_Uset , 0 + Result_Bg_Vset );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 + Result_Bg_Uset , 0 + Result_Bg_Vset );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 + Result_Bg_Uset , 1 + Result_Bg_Vset );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 + Result_Bg_Uset , 1 + Result_Bg_Vset );

	//	���U���g

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 + Result_Uset         , 0 + Result_Vset );
	pVtx[ 17 ].tex = D3DXVECTOR2( TEX_WIDTH + Result_Uset , 0 + Result_Vset );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 + Result_Uset         , TEX_HEIGHT + Result_Vset );
	pVtx[ 19 ].tex = D3DXVECTOR2( TEX_WIDTH + Result_Uset , TEX_HEIGHT + Result_Vset );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void PushBottomFade( VERTEX_2D* pVtx; )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�v�b�V���{�^���̃t�F�[�h
-----------------------------------------------------------------------------*/
void PushBottomFade( VERTEX_2D* pVtx )
{
	if( g_ResultFade == FADE_IN )	//	�t�F�[�h�C�����̏���
	{
		g_ResultFadeColor.a -= FADE_RATE;	//	a�l�����Z���Č��̉�ʂ𕂂��яオ�点��
		//	�t�F�[�h�C���̏I��
		if( g_ResultFadeColor.a < 0.0f )
		{
			g_ResultFadeColor.a = 0.0f;
			g_ResultFade = FADE_OUT;
		}	//	end od if
	}	//	end of if

	else if( g_ResultFade == FADE_OUT )	//	�t�F�[�h�A�E�g���̏���
	{
		g_ResultFadeColor.a += FADE_RATE;	//	a�l�����Z���Č��̉�ʂ������Ă���
		//	�t�F�[�h�A�E�g�̏I��
		if( g_ResultFadeColor.a > 1.0f )
		{
			g_ResultFadeColor.a = 1.0f;
			g_ResultFade = FADE_IN;	//	�t�F�[�h�C���ɏ����̐؂�ւ�
		}	//	end of if
	}	//	end of else if

	//	���_�F�̐ݒ�
	pVtx[ 4 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
	pVtx[ 5 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
	pVtx[ 6 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
	pVtx[ 7 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
}	//	end of func