/*=============================================================================

		�|�[�Y���[ pause.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/09/19
-------------------------------------------------------------------------------
	���@Update
		2016/08/22
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "usability.h"
#include "fade.h"
#include "pause.h"
#include "title.h"
#include "gamepad.h"
#include "player.h"
#include "base.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define PAUSE_BG_TEXTURENAME "data/TEXTURE/func/titleBg.jpg"
#define PAUSE_FRAME_TEXTURENAME "data/TEXTURE/game/pause/pauseBg01.jpg"
#define PAUSE_SELECT_TEXTURENAME "data/TEXTURE/game/pause/���.png"
#define PAUSE_CONTINUE_TEXTURENAME "data/TEXTURE/game/pause/continue.png"
#define PAUSE_RETRY_TEXTURENAME "data/TEXTURE/game/pause/retry.png"
#define PAUSE_QUIT_TEXTURENAME "data/TEXTURE/game/pause/quit.png"
#define PAUSE_OPERATION_TEXTURENAME "data/TEXTURE/game/pause/operation.png"
#define PAUSE_SCROLL_TEXTURENAME "data/TEXTURE/game/pause/pausescroll.png"

#define MAX_PAUSE_TEXTURENAME ( 8 )	//	�|�[�Y�̍ő�e�N�X�`����

#define PAUSE_BG_POS_X ( SCREEN_WIDTH * 0.3f )	//	�|�[�Y�w�iX
#define PAUSE_BG_POS_Y ( SCREEN_HEIGHT * 0.05f )	//	�|�[�Y�w�iY
#define PAUSE_BG_WIDTH ( 350.0f )				//	�|�[�Y�w�iWIDTH
#define PAUSE_BG_HEIGHT ( 500.0f )				//	�|�[�Y�w�iHEIGHT

#define PAUSE_CONTINUE_POS_X ( 400.0f )		//	�R���e�B�j���[X
#define PAUSE_CONTINUE_POS_Y ( 80.0f )		//	�R���e�B�j���[Y
#define PAUSE_CONTINUE_WIDTH ( 200.0f )		//	�R���e�B�j���[WIDTH
#define PAUSE_CONTINUE_HEIGHT ( 100.0f )	//	�R���e�B�j���[HEIGHT

#define PAUSE_RETRY_POS_X ( PAUSE_CONTINUE_POS_X )			//	���g���CX
#define PAUSE_RETRY_POS_Y ( PAUSE_CONTINUE_POS_Y + 125.0f )	//	���g���CY
#define PAUSE_RETRY_WIDTH ( PAUSE_CONTINUE_WIDTH )			//	���g���CWIDTH
#define PAUSE_RETRY_HEIGHT ( PAUSE_CONTINUE_HEIGHT )		//	���g���CHEIGHT

#define PAUSE_QUIT_POS_X ( PAUSE_CONTINUE_POS_X )			//	�I��X
#define PAUSE_QUIT_POS_Y ( PAUSE_CONTINUE_POS_Y + 325.0f )	//	�I��Y
#define PAUSE_QUIT_WIDTH ( PAUSE_CONTINUE_WIDTH )			//	�I��WIDTH
#define PAUSE_QUIT_HEIGHT ( PAUSE_CONTINUE_HEIGHT )			//	�I��HEIGHT

#define PAUSE_OPERATION_POS_X ( PAUSE_CONTINUE_POS_X )			//	������@X
#define PAUSE_OPERATION_POS_Y ( PAUSE_CONTINUE_POS_Y + 225.0f )	//	������@Y
#define PAUSE_OPERATION_WIDTH ( PAUSE_CONTINUE_WIDTH )			//	������@WIDTH
#define PAUSE_OPERATION_HEIGHT ( PAUSE_CONTINUE_HEIGHT )		//	������@HEIGHT

#define PAUSE_SCROLL_POS_X ( SCREEN_WIDTH * 0.31f )	//	�|�[�Y�X�N���[��X
#define PAUSE_SCROLL_POS_Y ( SCREEN_HEIGHT * 0.08f )	//	�|�[�Y�X�N���[��Y
#define PAUSE_SCROLL_WIDTH ( 330.0f )				//	�|�[�Y�X�N���[��WIDTH
#define PAUSE_SCROLL_HEIGHT ( 475.0f )				//	�|�[�Y�X�N���[��HEIGHT
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice );						//	���_�̍쐬
void PauseCursorKeyBoard( void );	//	�{�[�Y�J�[�\���ړ�
void PauseCursorGamePad( void );	//	�Q�[���p�b�h�J�[�\���ړ�
void PauseCursorPos( VERTEX_2D* pVtx );	//	�{�[�Y�J�[�\�����W�ύX
void PauseScreenTransition( void );	//	��ʑJ��
void GamePadScreenTransition( void );	//	�Q�[���p�b�h�̉�ʑJ��
void PauseScoll( VERTEX_2D* pVtx );	//	�|�[�Y�X�N���[��
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPause = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
//	�|�[�Y�w�i���S
LPDIRECT3DTEXTURE9 g_pTexturePauseBg = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�|�[�Y�w�i���S
LPDIRECT3DTEXTURE9 g_pTexturePauseFrame = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�J�[�\��
LPDIRECT3DTEXTURE9 g_pTexturePauseSelect = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�R���e�B�j���[
LPDIRECT3DTEXTURE9 g_pTexturePauseContinue = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	���g���C
LPDIRECT3DTEXTURE9 g_pTexturePauseRetry = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�I��
LPDIRECT3DTEXTURE9 g_pTexturePauseQuit = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	������@
LPDIRECT3DTEXTURE9 g_pTexturePauseOperation = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�X�N���[���|�[�Y
LPDIRECT3DTEXTURE9 g_pTexturePauseScroll = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

TITLE g_PauseSelect;

//	�f�B���C
int CntPuseFrame = 0;

float Pause_U = 0.0f , Pause_V = 0.0f;

//	�g�p�t���O
bool g_PadKeyFlag = false;

bool g_use = false;
bool g_OperationFlag = false;

bool g_Retry = false;
bool g_Quit = false;
bool g_Operation = false;
/*-----------------------------------------------------------------------------
 �֐���:	void InitPause( void )
 ����:		
 �߂�l:	
 ����:		�|�[�Y�̏�����
-----------------------------------------------------------------------------*/
void InitPause( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N
	//	�|�[�Y�w�i
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_BG_TEXTURENAME , &g_pTexturePauseBg  ) ) )
	{
		MessageBox( NULL , "�|�[�Y�w�i���S�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�|�[�Y�t���[��
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_FRAME_TEXTURENAME , &g_pTexturePauseFrame  ) ) )
	{
		MessageBox( NULL , "�|�[�Y�t���[���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�J�[�\��
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_SELECT_TEXTURENAME , &g_pTexturePauseSelect  ) ) )
	{
		MessageBox( NULL , "�|�[�Y�J�[�\���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�R���e�B�j���[
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_CONTINUE_TEXTURENAME , &g_pTexturePauseContinue  ) ) )
	{
		MessageBox( NULL , "�R���e�B�j���[�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	���g���C
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_RETRY_TEXTURENAME , &g_pTexturePauseRetry  ) ) )
	{
		MessageBox( NULL , "���g���C�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�I��
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_QUIT_TEXTURENAME , &g_pTexturePauseQuit  ) ) )
	{
		MessageBox( NULL , "�I���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	������@
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_OPERATION_TEXTURENAME , &g_pTexturePauseOperation  ) ) )
	{
		MessageBox( NULL , "�|�[�Y�̑�����@�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�|�[�Y�X�N���[��
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_SCROLL_TEXTURENAME , &g_pTexturePauseScroll  ) ) )
	{
		MessageBox( NULL , "�|�[�Y�X�N���[���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	������

	//	�J�[�\��������
	g_PauseSelect.Pos.x = PAUSE_CONTINUE_POS_X - PAUSE_CONTINUE_WIDTH * 0.5f;
	g_PauseSelect.Pos.y = PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT * 0.2f;
	g_PauseSelect.Size.x = PAUSE_CONTINUE_WIDTH * 0.6f;
	g_PauseSelect.Size.y = PAUSE_CONTINUE_HEIGHT * 0.7f;

	g_PadKeyFlag = false;

	g_use = false;
	g_OperationFlag = false;

	g_Retry = false;
	g_Quit = false;
	g_Operation = false;

	CntPuseFrame = 0;

	Pause_U = 0.0f;
	Pause_V = 0.0f;

	//	���_�̍쐬
	MakeVertexPause( pDevice );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitPause( void )
 ����:		
 �߂�l:	
 ����:		�|�[�Y�̏I��
-----------------------------------------------------------------------------*/
void UninitPause( void )
{
	if( g_pVtxBufferPause != NULL )	//	���_�o�b�t�@�J��
	{
		g_pVtxBufferPause -> Release();
		g_pVtxBufferPause = NULL;
	}	//	end of if

	//	�|�[�Y�w�i���S
	if( g_pTexturePauseBg != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseBg -> Release();
		g_pTexturePauseBg = NULL;
	}	//	end of if

	//	�|�[�Y�t���[��
	if( g_pTexturePauseFrame != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseFrame -> Release();
		g_pTexturePauseFrame = NULL;
	}	//	end of if

	//	�|�[�Y�J�[�\��
	if( g_pTexturePauseSelect != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseSelect -> Release();
		g_pTexturePauseSelect = NULL;
	}	//	end of if

	//	�R���e�B�j���[
	if( g_pTexturePauseContinue != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseContinue -> Release();
		g_pTexturePauseContinue = NULL;
	}	//	end of if

	//	���g���C
	if( g_pTexturePauseRetry != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseRetry -> Release();
		g_pTexturePauseRetry = NULL;
	}	//	end of if

	//	�I��
	if( g_pTexturePauseQuit != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseQuit -> Release();
		g_pTexturePauseQuit = NULL;
	}	//	end of if

	//	������@
	if( g_pTexturePauseOperation != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseOperation -> Release();
		g_pTexturePauseOperation = NULL;
	}	//	end of if

	//	�|�[�Y�X�N���[��
	if( g_pTexturePauseScroll != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePauseScroll -> Release();
		g_pTexturePauseScroll = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdatePause( void )
 ����:		
 �߂�l:	
 ����:		�|�[�Y�̍X�V
-----------------------------------------------------------------------------*/
void UpdatePause( void )
{
	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�v���C���[�̏��̎擾
	PLAYER *Player = GetPlayer();

	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	if( Player -> Hit == false && Base -> GoolFalg == false )
	{

		Pause_U += 0.004f;
		Pause_V -= 0.004f;

		if( g_use == false )
		{
			if( GetKeyboardRelease( DIK_P ) )
			{
				g_use = true;
			}
			if( GetGamePadTrigger( BUTTOM_12 ) )
			{
				g_use = true;
			}
		}
		else if( g_use == true )
		{
			if( GetKeyboardRelease( DIK_P ) )
			{
				g_use = false;
			}
			if( GetGamePadTrigger( BUTTOM_12 ) )
			{
				g_use = false;
			}
		}

		// �o�b�t�@�����b�N���A���z�A�h���X���擾
		g_pVtxBufferPause -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

		//	�{�[�Y�J�[�\�����W�ύX
		PauseCursorPos( pVtx );

		//	�|�[�Y�X�N���[��
		PauseScoll( pVtx );

		g_pVtxBufferPause -> Unlock();

		if( g_use == true )
		{
			for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
			{
				if( GamePad[ CntPad ] == NULL )
				{
					//	�{�[�Y�J�[�\���ړ�
					PauseCursorKeyBoard();
				}
				else if( GamePad[ CntPad ] != NULL )
				{
					//	�Q�[���p�b�h�J�[�\���ړ�
					PauseCursorGamePad();
				}
			}
		}
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawPause( void )
 ����:		
 �߂�l:	
 ����:		�|�[�Y�̕`��
-----------------------------------------------------------------------------*/
void DrawPause( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferPause , 0 , sizeof( VERTEX_2D ) );

	if( g_use == true )
	{
		//	�|�[�Y�w�i
		pDevice -> SetTexture( 0 , g_pTexturePauseBg );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);


		//	�|�[�Y�t���[��
		pDevice -> SetTexture( 0 , g_pTexturePauseFrame );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);


		//	�|�[�Y�X�N���[��
		pDevice -> SetTexture( 0 , g_pTexturePauseScroll );
		
		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 28 , NUM_POLYGON);

		//	�R���e�B�j���[
		pDevice -> SetTexture( 0 , g_pTexturePauseContinue );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12 , NUM_POLYGON);


		//	���g���C
		pDevice -> SetTexture( 0 , g_pTexturePauseRetry );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16 , NUM_POLYGON);


		//	�I��
		pDevice -> SetTexture( 0 , g_pTexturePauseQuit );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 20 , NUM_POLYGON);


		//	������@
		pDevice -> SetTexture( 0 , g_pTexturePauseOperation );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 24 , NUM_POLYGON);


		//	�|�[�Y�J�[�\��
		pDevice -> SetTexture( 0 , g_pTexturePauseSelect );

		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8 , NUM_POLYGON);
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		�|�[�Y�̕`��
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice )
{
	//	�G���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_PAUSE_TEXTURENAME , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferPause , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferPause -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	�|�[�Y�w�i

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f                  , 0.0f , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH   , 0.0f , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f                  , 0.0f + SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH   , 0.0f + SCREEN_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

	//	�|�[�Y�t���[��
	
	//	���_���W�̐ݒ�
	pVtx[ 4 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X                  , PAUSE_BG_POS_Y , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X + PAUSE_BG_WIDTH , PAUSE_BG_POS_Y , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X                  , PAUSE_BG_POS_Y + PAUSE_BG_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X + PAUSE_BG_WIDTH , PAUSE_BG_POS_Y + PAUSE_BG_HEIGHT , 0.0f );

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

	//	�|�[�Y�J�[�\��
	
	//	���_���W�̐ݒ�
	pVtx[ 8  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );

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

	//	�R���e�B�j���[
	
	//	���_���W�̐ݒ�
	pVtx[ 12 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X                        , PAUSE_CONTINUE_POS_Y , 0.0f );
	pVtx[ 13 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X + PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_POS_Y , 0.0f );
	pVtx[ 14 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X                        , PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT , 0.0f );
	pVtx[ 15 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X + PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT , 0.0f );

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

	//	���g���C

	//	���_���W�̐ݒ�
	pVtx[ 16 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X                     , PAUSE_RETRY_POS_Y , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X + PAUSE_RETRY_WIDTH , PAUSE_RETRY_POS_Y , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X                     , PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X + PAUSE_RETRY_WIDTH , PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT , 0.0f );

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

	//	�I��

	//	���_���W�̐ݒ�
	pVtx[ 20 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X                     , PAUSE_QUIT_POS_Y , 0.0f );
	pVtx[ 21 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X + PAUSE_QUIT_WIDTH  , PAUSE_QUIT_POS_Y , 0.0f );
	pVtx[ 22 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X                     , PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT , 0.0f );
	pVtx[ 23 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X + PAUSE_QUIT_WIDTH  , PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 20 ].rhw = 1.0f;
	pVtx[ 21 ].rhw = 1.0f;
	pVtx[ 22 ].rhw = 1.0f;
	pVtx[ 23 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 20 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 21 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 22 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 23 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 20 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 21 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 22 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 23 ].tex = D3DXVECTOR2( 1 , 1 );

	//	������@

	//	���_���W�̐ݒ�
	pVtx[ 24 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X                          , PAUSE_OPERATION_POS_Y , 0.0f );
	pVtx[ 25 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X + PAUSE_OPERATION_WIDTH  , PAUSE_OPERATION_POS_Y , 0.0f );
	pVtx[ 26 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X                          , PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT , 0.0f );
	pVtx[ 27 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X + PAUSE_OPERATION_WIDTH  , PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 24 ].rhw = 1.0f;
	pVtx[ 25 ].rhw = 1.0f;
	pVtx[ 26 ].rhw = 1.0f;
	pVtx[ 27 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 24 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 25 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 26 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 27 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 24 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 25 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 26 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 27 ].tex = D3DXVECTOR2( 1 , 1 );

	//	�|�[�Y�X�N���[��

	//	���_���W�̐ݒ�
	pVtx[ 28 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X                      , PAUSE_SCROLL_POS_Y , 0.0f );
	pVtx[ 29 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X + PAUSE_SCROLL_WIDTH , PAUSE_SCROLL_POS_Y , 0.0f );
	pVtx[ 30 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X                      , PAUSE_SCROLL_POS_Y + PAUSE_SCROLL_HEIGHT , 0.0f );
	pVtx[ 31 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X + PAUSE_SCROLL_WIDTH , PAUSE_SCROLL_POS_Y + PAUSE_SCROLL_HEIGHT , 0.0f );

	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 28 ].rhw = 1.0f;
	pVtx[ 29 ].rhw = 1.0f;
	pVtx[ 30 ].rhw = 1.0f;
	pVtx[ 31 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 28 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 29 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 30 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 31 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 28 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 29 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 30 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 31 ].tex = D3DXVECTOR2( 1 , 1 );

	g_pVtxBufferPause -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void PauseCursorMove( void )
 ����:		
 �߂�l:	
 ����:		�{�[�Y�J�[�\���ړ�
-----------------------------------------------------------------------------*/
void PauseCursorKeyBoard( void )
{
	//	��ړ�	
	if( GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UPARROW) )
	{
		//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

		//	�J�[�\�������g���C�ɂ��鎞
		if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
		{
			//	�R���e�B�j���[�Ɉړ�
			g_PauseSelect.Pos.y = PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT * 0.2f;
		}	//	end of if

		//	�J�[�\����������@�ɂ��鎞
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
		{
			//	���g���C�Ɉړ�
			g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;
		}	//	end of if

		//	�J�[�\�����I���ɂ��鎞
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_QUIT_POS_X , PAUSE_QUIT_POS_Y , PAUSE_QUIT_WIDTH , PAUSE_QUIT_HEIGHT ) )
		{
			//	������@�Ɉړ�
			g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;
		}	//	end of else if

		//	SE�Đ�
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if

	//	���ړ�
	if(GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWNARROW) )
	{
		//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

		//	�J�[�\�����R���e�B�j���[�ɂ��鎞
		if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_CONTINUE_POS_X , PAUSE_CONTINUE_POS_Y , PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_HEIGHT ) )
		{
			//	���g���C�Ɉړ�
			g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;
		}	//	end of if

		//	�J�[�\�������g���C�ɂ��鎞
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
		{
			//	������@�Ɉړ�
			g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;
		}	//	emd of else if

		//	�J�[�\����������@�ɂ��鎞
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
		{
			//	�I���Ɉړ�
			g_PauseSelect.Pos.y = PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT * 0.2f;
		}	//	end of if

		//	SE�Đ�
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if

	//	��ʑJ��
	PauseScreenTransition();

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void PauseCursorMove( void )
 ����:		
 �߂�l:	
 ����:		�Q�[���p�b�h�J�[�\���ړ�
-----------------------------------------------------------------------------*/
void PauseCursorGamePad( void )
{
	//	�X�e�B�b�N�̎擾
	LONG Stick_Y = GetGamePadStick_Y();

	CntPuseFrame++;

	if( CntPuseFrame % DELAY_PAD == 0 )
	{
		//	��ړ�	
		if( Stick_Y < -999 )
		{
			g_PadKeyFlag = true;

			if( g_PadKeyFlag == true )
			{
				//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

				//	�J�[�\�����I���ɂ��鎞
				if( g_Quit == true )
				{
					//	������@�Ɉړ�
					g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;

					g_Quit = false;
				}

				//	�J�[�\����������@�ɂ��鎞
				else if( g_Operation == true )
				{
					//	���g���C�Ɉړ�
					g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;

					g_Operation = false;
				}

				//	�J�[�\�������g���C�ɂ��鎞
				else if( g_Retry == true )
				{
					//	�R���e�B�j���[�Ɉړ�
					g_PauseSelect.Pos.y = PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT * 0.2f;

					g_Retry = false;
				}
			}

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_SERECT );
		}	//	end of if

		//	���ړ�
		if( Stick_Y > 999 )
		{
			g_PadKeyFlag = false;

			if( g_PadKeyFlag == false )
			{
				//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

				//	�J�[�\�������g���C�ɂ��鎞
				if( g_Retry == false )
				{
					//	���g���C�Ɉړ�
					g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;

					g_Retry = true;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	�J�[�\����������@�ɂ��鎞
				else if( g_Operation == false )
				{
					//	������@�Ɉړ�
					g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;

					g_Operation = true;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	�J�[�\�����I���ɂ��鎞
				else if( g_Quit == false )
				{
					//	�R���e�B�j���[�Ɉړ�
					g_PauseSelect.Pos.y = PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT * 0.2f;

					g_Quit = true;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}	//	end of else if
			}	//	end of if
		}	//	end of if
	}	//	end of if

	//	�Q�[���p�b�h�̉�ʑJ��
	GamePadScreenTransition();

}
/*-----------------------------------------------------------------------------
 �֐���:	void PauseScreenTransition( void )
 ����:		
 �߂�l:	
 ����:		��ʑJ��
-----------------------------------------------------------------------------*/
void PauseScreenTransition( void )
{
	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	�R���e�B�j���[
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_CONTINUE_POS_X , PAUSE_CONTINUE_POS_Y , PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_use = false;
		}	//	end of if
	}	//	end of if

	//	���g���C
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			if( *Mode == MODE_GAME )
			{
				SetFade( FADE_OUT , MODE_GAME );
			}
			else if( *Mode == MODE_TUTORIAL )
			{
				SetFade( FADE_OUT , MODE_TUTORIAL );
			}

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_GAME );

		}	//	end of if
	}	//	end of if

	//	������@
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_OperationFlag = true;

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if

	//	�I��
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_QUIT_POS_X , PAUSE_QUIT_POS_Y , PAUSE_QUIT_WIDTH , PAUSE_QUIT_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			SetFade( FADE_OUT , MODE_TITLE );

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_GAME );

		}	//	end of if
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void GamePadScreenTransition( void )
 ����:		
 �߂�l:	
 ����:		�Q�[���p�b�h�̉�ʑJ��
-----------------------------------------------------------------------------*/
void GamePadScreenTransition( void )
{
	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	�R���e�B�j���[
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_CONTINUE_POS_X , PAUSE_CONTINUE_POS_Y , PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_use = false;
		}	//	end of if
	}	//	end of if

	//	���g���C
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			if( *Mode == MODE_GAME )
			{
				SetFade( FADE_OUT , MODE_GAME );
			}
			else if( *Mode == MODE_TUTORIAL )
			{
				SetFade( FADE_OUT , MODE_TUTORIAL );
			}

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if

	//	������@
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_OperationFlag = true;

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if

	//	�I��
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_QUIT_POS_X , PAUSE_QUIT_POS_Y , PAUSE_QUIT_WIDTH , PAUSE_QUIT_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			SetFade( FADE_OUT , MODE_TITLE );

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if
}
/*-----------------------------------------------------------------------------
 �֐���:	void PauseCursorPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	
 ����:		�{�[�Y�J�[�\�����W�ύX
-----------------------------------------------------------------------------*/
void PauseCursorPos( VERTEX_2D* pVtx )
{
	//	���_���W�̐ݒ�
	pVtx[ 8  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void PauseScoll( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	
 ����:		�{�[�Y�X�N���[��
-----------------------------------------------------------------------------*/
void PauseScoll( VERTEX_2D* pVtx )
{
	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 28 ].tex = D3DXVECTOR2( 0 + Pause_U , 0 + Pause_V );
	pVtx[ 29 ].tex = D3DXVECTOR2( 1 + Pause_U , 0 + Pause_V );
	pVtx[ 30 ].tex = D3DXVECTOR2( 0 + Pause_U , 1 + Pause_V );
	pVtx[ 31 ].tex = D3DXVECTOR2( 1 + Pause_U , 1 + Pause_V );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	bool *GetPause( void )
 ����:		
 �߂�l:	
 ����:		�{�[�Y���Ă��邩
-----------------------------------------------------------------------------*/
bool *GetPause( void )
{
	return &g_use;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	bool *GetPause( void )
 ����:		
 �߂�l:	
 ����:		�{�[�Y���Ă��邩
-----------------------------------------------------------------------------*/
bool *GetOpeFlag( void )
{
	return &g_OperationFlag;
}