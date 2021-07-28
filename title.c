/*=============================================================================

		�^�C�g��[ Title.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/08/02
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "title.h"
#include "fade.h"
#include "usability.h"
#include "sound.h"
#include "cloud.h"
#include "base.h"
#include "bg.h"
#include "enemy.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define TITLE_TEXTURENAME "data/TEXTURE/title/title.png"			//	�^�C�g���e�N�X�`��
#define START_TEXTURENAME "data/TEXTURE/title/start.png"					//	�X�^�[�g���S�e�N�X�`��
#define TUTORIAL_TEXTURENAME "data/TEXTURE/title/tutorial.png"			//	�`���[�g���A�����S�e�N�X�`��
#define SERECT_TEXTURENAME "data/TEXTURE/title/player.png"					//	���e�N�X�`��
#define RANKING_TEXTURENAME "data/TEXTURE/title/ranking.png"			//	�����L���O���S�e�N�X�`��

#define MAX_TITLE_TEXTURE ( 6 )	//	�ő�e�N�X�`��

#define TITLE_BG_POS_X ( 0.0f )	//	�^�C�g���w�i�̕\���ʒu�w
#define TITLE_BG_POS_Y ( 0.0f )	//	�^�C�g���w�i�̕\���ʒu�x

#define TITLE_POS_Y ( 100.0f )	//	�^�C�g���̕\���ʒu�x

#define START_POS_X ( 375.0f )	//	�X�^�[�g�̕\���ʒu�w
#define START_POS_Y ( 250.0f )	//	�X�^�[�g�̕\���ʒu�x
#define START_WIDTH ( 200.0f )	//	�X�^�[�g�̕�
#define START_HEIGHT ( 100.0f )	//	�X�^�[�g�̍���

#define TUTORIAL_POS_X ( START_POS_X )	//	�`���[�g���A���̕\���ʒu�w
#define TUTORIAL_POS_Y ( START_POS_Y + 100.0f )		//	�`���[�g���A���̕\���ʒu�x
#define TUTORIAL_WIDTH ( 200.0f )		//	�`���[�g���A���̕�
#define TUTORIAL_HEIGHT ( 100.0f )		//	�`���[�g���A���̍���

#define RANKING_POS_X ( START_POS_X )	//	�����L���O�̕\���ʒu�w
#define RANKING_POS_Y ( START_POS_Y + 200.0f )		//	�����L���O�̕\���ʒu�x
#define RANKING_WIDTH ( 200.0f )		//	�����L���O�̕�
#define RANKING_HEIGHT ( 100.0f )		//	�����L���O�̍���

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertextitle( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void MakeVerTexSerectPos( VERTEX_2D* pVtx );			//	�����W�X�V
void ScreenTransition( void );	//	��ʑJ��
void CursorMove( void );	//	�J�[�\���ړ�
void GamePadCursorMove( void );	//	�Q�[���p�b�h�̃J�[�\���ړ�
void GamePadTitleScreenTransition( void );	//	�Q�[���p�b�h�̉�ʑJ��
void TitleRogoPos( VERTEX_2D* pVtx );	//	�^�C�g���̍��W�ύX
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitle = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
//	�^�C�g�����S
LPDIRECT3DTEXTURE9 g_pTexturetitle = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
//	�X�^�[�g���S
LPDIRECT3DTEXTURE9 g_pTexturestart = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
//	�`���[�g���A�����S
LPDIRECT3DTEXTURE9 g_pTextureTutorialRogo = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
//	���
LPDIRECT3DTEXTURE9 g_pTextureSerect = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
//	�����L���O���S
LPDIRECT3DTEXTURE9 g_pTextureRankingRogo = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

TITLE g_serect;

TITLE g_TitleRogo;

float g_Offset = 0.0f;

int CntFrame = 0;

bool KeyFlag = false;

bool g_RankingFlag = false;
bool g_TutorialFlag = false;

/*-----------------------------------------------------------------------------
 �֐���:	void InitTitle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C�g���̏�����
-----------------------------------------------------------------------------*/
void InitTitle( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N

	//	�^�C�g�����S
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , TITLE_TEXTURENAME , &g_pTexturetitle  ) ) )
	{
		MessageBox( NULL , "�^�C�g�����S�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�X�^�[�g���S
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , START_TEXTURENAME , &g_pTexturestart  ) ) )
	{
		MessageBox( NULL , "�X�^�[�g���S�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�`���[�g���A�����S
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_TEXTURENAME , &g_pTextureTutorialRogo  ) ) )
	{
		MessageBox( NULL , "�`���[�g���A�����S�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	���
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , SERECT_TEXTURENAME , &g_pTextureSerect  ) ) )
	{
		MessageBox( NULL , "���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�����L���O���S
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , RANKING_TEXTURENAME , &g_pTextureRankingRogo  ) ) )
	{
		MessageBox( NULL , "�����L���O���S�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	������
	//	�J�[�\��
	g_serect.Pos.x = START_POS_X - START_WIDTH * 0.4f;
	g_serect.Pos.y = START_POS_Y + START_HEIGHT * 0.1f;
	g_serect.Size.x = START_WIDTH * 0.5f;
	g_serect.Size.y = START_HEIGHT*0.6f;

	//	�^�C�g�����S
	g_TitleRogo.Pos.x = 200.0f;
	g_TitleRogo.Pos.y = -20.0f;
	g_TitleRogo.Size.x = 600.0f;
	g_TitleRogo.Size.y = 300.0f;
	g_TitleRogo.move.y = 1.0f;

	g_Offset = 0.0f;

	CntFrame = 0;

	KeyFlag = false;

	g_RankingFlag = false;
	g_TutorialFlag = false;

	InitBg();	//	�w�i�̏�����

	InitCloud();	//	�_�̏�����

	InitBase();	//	����̏�����

	InitEnemy();	//	�G�̏�����

	MakeVertextitle( pDevice );	//	���_�̍쐬

	//	���y�Đ�
	PlaySound( SOUND_LABEL_BGM_TITLE );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitTitle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C�g���̏I��
-----------------------------------------------------------------------------*/
void UninitTitle( void )
{

	if( g_pVtxBufferTitle != NULL )
	{
		g_pVtxBufferTitle -> Release();
		g_pVtxBufferTitle = NULL;
	}

	//	�^�C�g�����S
	if( g_pTexturetitle != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturetitle -> Release();
		g_pTexturetitle = NULL;
	}	//	end of if

	//	�X�^�[�g���S
	if( g_pTexturestart != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturestart -> Release();
		g_pTexturestart = NULL;
	}	//	end of if

	//	�`���[�g���A�����S
	if( g_pTextureTutorialRogo != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTutorialRogo -> Release();
		g_pTextureTutorialRogo = NULL;
	}	//	end of if

	//	���
	if( g_pTextureSerect != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureSerect -> Release();
		g_pTextureSerect = NULL;
	}	//	end of if

	//	�����L���O���S
	if( g_pTextureRankingRogo != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureRankingRogo -> Release();
		g_pTextureRankingRogo = NULL;
	}	//	end of if




	UninitBg();	//	�w�i�̏I��

	UninitCloud();	//	�_�̏I��

	UninitBase();	//	����̏I��

	UninitEnemy();	//	�G�̏I��


}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdeteTitle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C�g���̍X�V
-----------------------------------------------------------------------------*/
void UpdateTitle( void )
{
	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�t�F�[�h�����ǂ���
	bool *FadeUse = GetFadeUse();

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	UpdateBg();	//	�w�i�̍X�V

	UpdateCloud();	//	�_�̍X�V

	UpdateBase();	//	����̍X�V

	UpdateEnemy();	//	�G�̍X�V

	CntFrame++;	

	if( *FadeUse == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
		{
			if( GamePad[ CntPad ] == NULL )
			{
				//	�J�[�\���ړ�
				CursorMove();
			}
			if( GamePad[ CntPad ] != NULL )
			{
				
				//	�Q�[���p�b�h�̃J�[�\���ړ�
				GamePadCursorMove();
			}

		}
	}


	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTitle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	�����W�X�V
	MakeVerTexSerectPos( pVtx );

	//	�^�C�g���̍��W�ύX
	TitleRogoPos( pVtx );

	g_pVtxBufferTitle -> Unlock();

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawTitle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C�g���̕`��
-----------------------------------------------------------------------------*/
void DrawTitle( void )
{
	DrawBg();	//	�w�i�̕`��

	DrawCloud();	//	�_�̕`��

	DrawBase();		//	����̕`��

	DrawEnemy();	//	�G�̕`��

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�^�C�g���w�i���S
	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTitle , 0 , sizeof( VERTEX_2D ) );

	//	�X�^�[�g���S
	pDevice -> SetTexture( 0 , g_pTexturestart );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

	//	�`���[�g���A�����S
	pDevice -> SetTexture( 0 , g_pTextureTutorialRogo );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);

	//	�����L���O���S
	pDevice -> SetTexture( 0 , g_pTextureRankingRogo );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12, NUM_POLYGON);

	//	���
	pDevice -> SetTexture( 0 , g_pTextureSerect );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16, NUM_POLYGON);

	//	�^�C�g�����S
	pDevice -> SetTexture( 0 , g_pTexturetitle );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertextitle( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertextitle( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TITLE_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTitle , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;
	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTitle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	�^�C�g�����S
	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );

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

	//	�X�^�[�g���S
	//	���_���W�̐ݒ�
	pVtx[ 4 ].pos = D3DXVECTOR3( START_POS_X                , START_POS_Y , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( START_POS_X + START_WIDTH  , START_POS_Y , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( START_POS_X               , START_POS_Y + START_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( START_POS_X + START_WIDTH , START_POS_Y + START_HEIGHT , 0.0f );

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

	//	�`���[�g���A�����S
	//	���_���W�̐ݒ�
	pVtx[ 8  ].pos = D3DXVECTOR3( TUTORIAL_POS_X                   , TUTORIAL_POS_Y , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH  , TUTORIAL_POS_Y , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( TUTORIAL_POS_X                   , TUTORIAL_POS_Y + TUTORIAL_HEIGHT , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH  , TUTORIAL_POS_Y + TUTORIAL_HEIGHT , 0.0f );

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

	//	�����L���O
	//	���_���W�̐ݒ�
	pVtx[ 12 ].pos = D3DXVECTOR3( RANKING_POS_X                  , RANKING_POS_Y , 0.0f );
	pVtx[ 13 ].pos = D3DXVECTOR3( RANKING_POS_X + RANKING_WIDTH  , RANKING_POS_Y , 0.0f );
	pVtx[ 14 ].pos = D3DXVECTOR3( RANKING_POS_X                  , RANKING_POS_Y + RANKING_HEIGHT , 0.0f );
	pVtx[ 15 ].pos = D3DXVECTOR3( RANKING_POS_X + RANKING_WIDTH  , RANKING_POS_Y + RANKING_HEIGHT , 0.0f );

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

	//	���
	//	���_���W�̐ݒ�
	pVtx[ 16 ].pos = D3DXVECTOR3( g_serect.Pos.x                   , g_serect.Pos.y , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( g_serect.Pos.x + g_serect.Size.x , g_serect.Pos.y , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( g_serect.Pos.x                   , g_serect.Pos.y + g_serect.Size.y , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( g_serect.Pos.x + g_serect.Size.x , g_serect.Pos.y + g_serect.Size.y , 0.0f );

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
	pVtx[ 17 ].tex = D3DXVECTOR2( 0.5f , 0 );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 19 ].tex = D3DXVECTOR2( 0.5f , 1 );

	g_pVtxBufferTitle -> Unlock();

	return S_OK;
}
/*-----------------------------------------------------------------------------
 �֐���:	void ScreenTransition( void )
 ����:		
 �߂�l:	
 ����:		��ʑJ��
-----------------------------------------------------------------------------*/
void ScreenTransition( void )
{
	//	��ʑJ��

	//	�Q�[��
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , START_POS_X , START_POS_Y , START_WIDTH , START_HEIGHT ) )
	{
		if(GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_GAME);

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );


			//	���y��~
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	�`���[�g���A��
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
	{
		if(GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_TUTORIAL);

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	�����L���O
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , RANKING_POS_X , RANKING_POS_Y , RANKING_WIDTH , RANKING_HEIGHT ) )
	{
		if(GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_Offset = 0.5f;

			SetFade( FADE_OUT , MODE_RANKING );

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_TITLE );
		
		}	//	end of if
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void CursorMove( void )
 ����:		
 �߂�l:	
 ����:		�J�[�\���ړ�
-----------------------------------------------------------------------------*/
void CursorMove( void )
{
	//	�J�[�\���ړ�

	//	��ړ�	
	if( GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UPARROW) )
	{
		//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

		//	�J�[�\�����`���[�g���A���ɂ��鎞
		if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , RANKING_POS_X , RANKING_POS_Y , RANKING_WIDTH , RANKING_HEIGHT ) )
		{
			//	�`���[�g���A�����S�Ɉړ�����
			g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.2f;
		}	//	end of if

		//	�J�[�\���������L���O�ɂ��鎞
		else if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
		{
			//	�X�^�[�g���S�Ɉړ�����
			g_serect.Pos.y = START_POS_Y + START_WIDTH * 0.2f;
		}	//	end of else if

		//	SE�Đ�
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if


	//	���ړ�
	if(GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWNARROW) )
	{
		//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

		//	�J�[�\�����X�^�[�g�ɂ��鎞
		if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , START_POS_X , START_POS_Y , START_WIDTH , START_HEIGHT ) )
		{
			//	�`���[�g���A�����S�Ɉړ�����
			g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.2f;
		}	//	end of if

		//	�J�[�\�����`���[�g���A���ɂ��鎞
		else if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
		{
			//	�����L���O���S�Ɉړ�����
			g_serect.Pos.y = RANKING_POS_Y + RANKING_WIDTH * 0.2f;
		}	//	end of else if

		//	SE�Đ�
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if

	//	��ʑJ��
	ScreenTransition();

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void GamePadCursorMove( void )
 ����:		
 �߂�l:	
 ����:		�Q�[���p�b�h�̃J�[�\���ړ�
-----------------------------------------------------------------------------*/
void GamePadCursorMove( void )
{
	//	�X�e�B�b�N�̎擾
	LONG Stick_Y = GetGamePadStick_Y();

	if( CntFrame % DELAY_PAD == 0 )
	{
		CntFrame = 0;

		//	�J�[�\���ړ�
		
		//	��ړ�	
		if( Stick_Y < 0 )
		{
			KeyFlag = true;

			if( KeyFlag == true )
			{
				//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

				//	�����L���O
				if( g_RankingFlag == true )
				{
					g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.1f;

					g_RankingFlag = false;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	�`���[�g���A��
				else if( g_TutorialFlag == true )
				{
					g_serect.Pos.y = START_POS_Y + START_WIDTH * 0.1f;

					g_TutorialFlag = false;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}
			}	//	end of if
		}


		//	���ړ�
		if( 999 < Stick_Y )
		{
			KeyFlag = false;

			if( KeyFlag == false )
			{
			
				//	���݂̃J�[�\���̈ʒu�ɂ���Ĉʒu���ς��

				//	�`���[�g���A��
				if( g_TutorialFlag == false )
				{
					g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.2f;

					g_TutorialFlag = true;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	�����L���O
				else if( g_RankingFlag == false )
				{
					g_serect.Pos.y = RANKING_POS_Y + RANKING_WIDTH * 0.2f;

					g_RankingFlag = true;

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_SERECT );
				}
			}
		}
	}

	//	��ʑJ��
	GamePadTitleScreenTransition();
}
/*-----------------------------------------------------------------------------
 �֐���:	void GamePadTitleScreenTransition( void )
 ����:		
 �߂�l:	
 ����:		�Q�[���p�b�h�̉�ʑJ��
-----------------------------------------------------------------------------*/
void GamePadTitleScreenTransition( void )
{
	//	��ʑJ��

	//	�Q�[��
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , START_POS_X , START_POS_Y , START_WIDTH , START_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_GAME);

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	�`���[�g���A��
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_TUTORIAL);

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	�����L���O
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , RANKING_POS_X , RANKING_POS_Y , RANKING_WIDTH , RANKING_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_Offset = 0.5f;

			SetFade( FADE_OUT , MODE_RANKING );

			//	SE�Đ�
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	���y��~
			StopSound( SOUND_LABEL_BGM_TITLE );
		
		}	//	end of if
	}	//	end of if
}
/*-----------------------------------------------------------------------------
 �֐���:	void MakeVerTexSerectPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�����W�X�V
-----------------------------------------------------------------------------*/
void MakeVerTexSerectPos( VERTEX_2D* pVtx )
{
	//	���_���W�̐ݒ�
	pVtx[ 16 ].pos = D3DXVECTOR3( g_serect.Pos.x , g_serect.Pos.y , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( g_serect.Pos.x+g_serect.Size.x , g_serect.Pos.y , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( g_serect.Pos.x, g_serect.Pos.y+g_serect.Size.y , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( g_serect.Pos.x+g_serect.Size.x , g_serect.Pos.y+g_serect.Size.y , 0.0f );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 + g_Offset , 0 );
	pVtx[ 17 ].tex = D3DXVECTOR2( 0.5f + g_Offset , 0 );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 + g_Offset , 1 );
	pVtx[ 19 ].tex = D3DXVECTOR2( 0.5f + g_Offset , 1 );
}

/*-----------------------------------------------------------------------------
 �֐���:	void TitleRogoPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�^�C�g���̍��W�ύX
-----------------------------------------------------------------------------*/
void TitleRogoPos( VERTEX_2D* pVtx )
{
	if( g_TitleRogo.Pos.y != TITLE_POS_Y )
	{
		g_TitleRogo.Pos.y += g_TitleRogo.move.y;
	}

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );
}