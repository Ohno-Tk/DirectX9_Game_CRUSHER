/*=============================================================================

		����[ tutorial.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/09/01
-------------------------------------------------------------------------------
	���@Update
		2016/09/01
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "fade.h"
#include "tutorial.h"
#include "base.h"
#include "player.h"
#include "cloud.h"
#include "enemy.h"
#include "bg.h"
#include "particle.h"
#include "pause.h"
#include "operation.h"
#include "guid.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
//	�ړ�
#define TUTORIAL_MOVE01_TEXTURENAME "data/TEXTURE/tutorial/move01.png"
#define TUTORIAL_MOVE02_TEXTURENAME "data/TEXTURE/tutorial/move02.png"

//	�U��
#define TUTORIAL_ATTACK01_TEXTURENAME "data/TEXTURE/tutorial/attack01.png"
#define TUTORIAL_ATTACK02_TEXTURENAME "data/TEXTURE/tutorial/attack02.png"

//	�_�b�V��
#define TUTORIAL_DASH01_TEXTURENAME "data/TEXTURE/tutorial/dash01.png"
#define TUTORIAL_DASH02_TEXTURENAME "data/TEXTURE/tutorial/dash02.png"

//	�W�����v
#define TUTORIAL_JUMP01_TEXTURENAME "data/TEXTURE/tutorial/jump01.png"
#define TUTORIAL_JUMP02_TEXTURENAME "data/TEXTURE/tutorial/jump02.png"

//	�|�[�Y
#define TUTORIAL_PAUSE01_TEXTURENAME "data/TEXTURE/tutorial/pause01.png"
#define TUTORIAL_PAUSE02_TEXTURENAME "data/TEXTURE/tutorial/pause02.png"
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorial = NULL;

//	�ړ�
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Move = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�U��
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Attack = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�_�b�V��
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Dash = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�W�����v
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Jump = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�|�[�Y
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Pause = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

TUTORIAL g_Tutorial[ MAX_TUTORIAL ];
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice );						//	���_�̍쐬
void TutorialPos( VERTEX_2D* pVtx , int IndexTutorial );	//	���W�ύX
/*-----------------------------------------------------------------------------
 �֐���:	void InitTutorial(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A���̏�����
-----------------------------------------------------------------------------*/
void InitTutorial(void)
{
	int CntTutorial;

	//	���̃��[�h
	MODE *Mode = GetMode();

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	������̎擾
	BASE *Base = GetBase( 0 );

	if( GamePad[ 0 ] == NULL )
	{
		//	�G���[�`�F�b�N
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_MOVE01_TEXTURENAME , &g_pTextureTutorial_Move  ) ) )
		{
			MessageBox( NULL , "�ړ��O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_ATTACK01_TEXTURENAME , &g_pTextureTutorial_Attack  ) ) )
		{
			MessageBox( NULL , "�U���O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_DASH01_TEXTURENAME , &g_pTextureTutorial_Dash  ) ) )
		{
			MessageBox( NULL , "�_�b�V���O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_JUMP01_TEXTURENAME , &g_pTextureTutorial_Jump  ) ) )
		{
			MessageBox( NULL , "�W�����v�O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_PAUSE01_TEXTURENAME , &g_pTextureTutorial_Pause  ) ) )
		{
			MessageBox( NULL , "�|�[�Y�O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	if( GamePad[ 0 ] != NULL )
	{
		//	�G���[�`�F�b�N
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_MOVE02_TEXTURENAME , &g_pTextureTutorial_Move  ) ) )
		{
			MessageBox( NULL , "�ړ��O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_ATTACK02_TEXTURENAME , &g_pTextureTutorial_Attack  ) ) )
		{
			MessageBox( NULL , "�U���O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_DASH02_TEXTURENAME , &g_pTextureTutorial_Dash  ) ) )
		{
			MessageBox( NULL , "�_�b�V���O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_JUMP02_TEXTURENAME , &g_pTextureTutorial_Jump  ) ) )
		{
			MessageBox( NULL , "�W�����v�O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_PAUSE02_TEXTURENAME , &g_pTextureTutorial_Pause  ) ) )
		{
			MessageBox( NULL , "�|�[�Y�O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	���_�̍쐬
	MakeVertexTutorial( pDevice );

	*Mode = MODE_TUTORIAL;

	InitBg();	//	�w�i�̏�����

	InitBase();	//	����̏�����

	InitGuid();	//	�ē��̏�����

	InitCloud();	//	�_�̏�����

	InitEnemy();	//	�G�̏�����

	InitPlayer();	//	�v���C���[�̏�����

	Initparticle();	//	�p�[�e�B�N���̏�����

	InitPause();	//	�|�[�Y�̏�����

	InitOperation();	//	������@�̏�����

	//	������
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ )
	{
		g_Tutorial[ CntTutorial ].pos.y = 100.0f;
		g_Tutorial[ CntTutorial ].size.x = 300.0f;
		g_Tutorial[ CntTutorial ].size.y = 300.0f;
		g_Tutorial[ CntTutorial ].Move_X = Base -> move_X;
	}	//	end of for

	g_Tutorial[ 0 ].pos.x = 450.0f;
	g_Tutorial[ 1 ].pos.x = SCREEN_WIDTH + 300.0f;
	g_Tutorial[ 2 ].pos.x = SCREEN_WIDTH + 1000.0f;
	g_Tutorial[ 3 ].pos.x = SCREEN_WIDTH + 1500.0f;
	g_Tutorial[ 4 ].pos.x = SCREEN_WIDTH + 2250.0f;

	

	//	���y�Đ�
	PlaySound( SOUND_LABEL_BGM_GAME );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitTutorial(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A���̏I��
-----------------------------------------------------------------------------*/
void UninitTutorial(void)
{
	if( g_pVtxBufferTutorial != NULL )
	{
		g_pVtxBufferTutorial -> Release();
		g_pVtxBufferTutorial = NULL;
	}	//	end of if

	//	�ړ�
	if( g_pTextureTutorial_Move != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTutorial_Move -> Release();
		g_pTextureTutorial_Move = NULL;
	}	//	end of if

	//	�U��
	if( g_pTextureTutorial_Attack != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTutorial_Attack -> Release();
		g_pTextureTutorial_Attack = NULL;
	}	//	end of if

	//	�_�b�V��
	if( g_pTextureTutorial_Dash != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTutorial_Dash -> Release();
		g_pTextureTutorial_Dash = NULL;
	}	//	end of if

	//	�W�����v
	if( g_pTextureTutorial_Jump != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTutorial_Jump -> Release();
		g_pTextureTutorial_Jump = NULL;
	}	//	end of if

	//	�|�[�Y
	if( g_pTextureTutorial_Pause != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTutorial_Pause -> Release();
		g_pTextureTutorial_Pause = NULL;
	}	//	end of if

	UninitBg();	//	�w�i�̏I��

	UninitBase();	//	����̏I��

	UninitGuid();	//	�ē��̏I��

	UninitCloud();	//	�_�̏I��

	UninitEnemy();	//	�G�̏I��

	UninitPlayer();	//	�v���C���[�̏I��

	Uninitparticle();	//	�p�[�e�B�N���̏I��

	UninitPause();	//	�|�[�Y�̏I��

	UninitOperation();	//	������@�̏I��

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateTutorial(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A���̍X�V
-----------------------------------------------------------------------------*/
void UpdateTutorial(void)
{
	int CntTutorial;

	//	�|�[�Y���̎擾
	bool *Pause = GetPause();
	bool *PauseUse = GetOpeFlag();

	//	�v���C���[���̎擾
	PLAYER *Player = GetPlayer();

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTutorial -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ )
	{
		//	���W�ύX
		TutorialPos( pVtx , CntTutorial );
	}	//	end of for

	g_pVtxBufferTutorial -> Unlock();

	if( *Pause == false )
	{

		UpdateBg();	//	�w�i�̍X�V

		UpdateBase();	//	����̍X�V

		UpdateGuid();	//	�ē��̍X�V

		UpdateCloud();	//	�_�̍X�V

		UpdateEnemy();	//	�G�̍X�V

		UpdatePlayer();	//	�v���C���[�̍X�V

		Updateparticle();	//	�p�[�e�B�N���̍X�V
	}

	if( *PauseUse == false )
	{
		UpdatePause();	//	�|�[�Y�̍X�V
	}
	else if( *PauseUse == true )
	{
		UpdateOperation();	//	������@�̍X�V
	}

	//	��ʑJ��
	if( Player->gool == true || Player -> bUse == false )
	{
		SetFade(FADE_OUT , MODE_RESULT);

		//	���y��~
		StopSound( SOUND_LABEL_BGM_GAME );
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawTutorial(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A���̕`��
-----------------------------------------------------------------------------*/
void DrawTutorial(void)
{
	//	�|�[�Y���̎擾
	bool *PauseUse = GetOpeFlag();

	DrawBg();	//	�w�i�̕`��

	DrawBase();		//	����̕`��

	DrawGuid();	//	�ē��̕`��

	DrawCloud();	//	�_�̕`��



	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTutorial , 0 , sizeof( VERTEX_2D ) );

	//	�ړ�
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Move );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

	//	�U��
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Attack );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

	//	�_�b�V��
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Dash );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);

	//	�W�����v
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Jump );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12, NUM_POLYGON);

	//	�|�[�Y
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Pause );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16, NUM_POLYGON);


	DrawEnemy();	//	�G�̕`��

	DrawPlayer();	//	�v���C���[�̕`��

	Drawparticle();	//	�p�[�e�B�N���̕`��

	if( *PauseUse == false )
	{
		DrawPause();	//	�|�[�Y�̕`��
	}
	else if( *PauseUse == true )
	{
		DrawOperation();	//	������@�̕`��
	}


}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice )
{
	int CntTutorial;

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TUTORIAL , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTutorial , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTutorial -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ )
	{
		//	���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x                                    , g_Tutorial[ CntTutorial ].pos.y                                    , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x + g_Tutorial[ CntTutorial ].size.x , g_Tutorial[ CntTutorial ].pos.y                                    , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x                                    , g_Tutorial[ CntTutorial ].pos.y + g_Tutorial[ CntTutorial ].size.y , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x + g_Tutorial[ CntTutorial ].size.x , g_Tutorial[ CntTutorial ].pos.y + g_Tutorial[ CntTutorial ].size.y , 0.0f );

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

		//	�|�C���^�����炷
		pVtx += 4;
	}	//	end of for

	g_pVtxBufferTutorial -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void TutorialPos( VERTEX_2D* pVtx , int IndexTutorial )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		���W�ύX
-----------------------------------------------------------------------------*/
void TutorialPos( VERTEX_2D* pVtx , int IndexTutorial )
{
	pVtx += IndexTutorial * NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x                                      , g_Tutorial[ IndexTutorial ].pos.y                                      , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x + g_Tutorial[ IndexTutorial ].size.x , g_Tutorial[ IndexTutorial ].pos.y                                      , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x                                      , g_Tutorial[ IndexTutorial ].pos.y + g_Tutorial[ IndexTutorial ].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x + g_Tutorial[ IndexTutorial ].size.x , g_Tutorial[ IndexTutorial ].pos.y + g_Tutorial[ IndexTutorial ].size.y , 0.0f );


}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	TUTORIAL *GetTutorial( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A�����̎擾
-----------------------------------------------------------------------------*/
TUTORIAL *GetTutorial( void )
{
	return &g_Tutorial[ 0 ];
}