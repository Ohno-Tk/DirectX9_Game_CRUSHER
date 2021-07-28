/*=============================================================================

		�G[ enemy.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/08/08
-------------------------------------------------------------------------------
	���@Update
	2016/07/28
		�E�w�b�_�t�@�C���ǉ�
			#include <stdio.h>
			#include "main.h"
			#include "enemy.h"
		�E�萔��`�ǉ�
			#define ENEMY_TEXTURENAME
		�E�O���[�o���ϐ��ǉ�
			LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
			LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

			ENEMY g_Enemy;
		�E�v���g�^�C�v�錾�ǉ�
			HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
	2016/09/07
		�E�v���g�^�C�v�錾�ǉ�
			void EnemyStage1( void );	//	�X�e�[�W�P�̓G�̔z�u
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "sound.h"
#include "enemy.h"
#include "player.h"
#include "base.h"
#include "fade.h"
#include "particle.h"
#include "score.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define ENEMY_TEXTURENAME "data/TEXTURE/game/car.png"
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

ENEMY g_Enemy[MAX_ENEMY];
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEnemy( LPDIRECT3DDEVICE9 pDevice );		//	���_�̍쐬
void ModeTitleEnemy( int IndexEnemy );						//	�^�C�g�����[�h���̓G�̍s��
void MakeEnemyPos( VERTEX_2D* pVtx , int IndexEnemy );		//	�G�̍��W�ύX
void UpdateEnemyAct( int IndexEnemy );						//	�X�V���̓G�̍s��
void CreateEnemy( float PosX , float PosY , float Speed );	//	�G�̐���
void EnemyTitle( void );									//	�^�C�g���̓G�̐���
void EnemyTutorial( void );									//	�`���[�g���A���̓G�̔z�u
void EnemyStage1( void );									//	�X�e�[�W�P�̓G�̔z�u
/*-----------------------------------------------------------------------------
 �֐���:	void InitEnemy(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�̏�����
-----------------------------------------------------------------------------*/
void InitEnemy(void)
{
	int CntEnemy;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	������̎擾
	BASE *Base = GetBase( 0 );

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , ENEMY_TEXTURENAME , &g_pTextureEnemy  ) ) )
	{
		MessageBox( NULL , "�G�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�G�̏�����
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		g_Enemy[CntEnemy].Size.x = 100.0f;
		g_Enemy[CntEnemy].Size.y = 100.0f;
		g_Enemy[CntEnemy].Move.x = 1.0f;
		g_Enemy[CntEnemy].Move.y = 1.0f;
		g_Enemy[CntEnemy].Scroll_Move_X = Base -> move_X;
		g_Enemy[CntEnemy].Score = 100 + rand()%100;
		g_Enemy[CntEnemy].Use = false;
	}	//	end of for

	//	�G�̐���
	if( *Mode == MODE_TITLE )
	{
		EnemyTitle();	//	�^�C�g���̓G�̔z�u
	}	//	end of if
	if( *Mode == MODE_TUTORIAL )
	{
		EnemyTutorial();	//	�`���[�g���A���̓G�̔z�u
	}	//	end of if

	else if( *Mode == MODE_GAME )
	{
		EnemyStage1();	//	�X�e�[�W�P�̓G�̔z�u
	}	//	end of else if
	

	MakeVertexEnemy( pDevice );	//	���_�̍쐬
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitEnemy(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�̏I��
-----------------------------------------------------------------------------*/
void UninitEnemy(void)
{

	if( g_pTextureEnemy != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureEnemy -> Release();
		g_pTextureEnemy = NULL;
	}	//	end of if

	if(g_pVtxBufferEnemy != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferEnemy -> Release();
		g_pVtxBufferEnemy = NULL;
	}	//	end of if

	//	�G�̏�����
	for( int CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		if( g_Enemy[CntEnemy].Use == true )
		{
			g_Enemy[CntEnemy].Use = false;
		}
	}	//	end of for

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateEnemy(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�̍X�V
-----------------------------------------------------------------------------*/
void UpdateEnemy(void)
{
	int CntEnemy;

	//	���̃��[�h
	MODE *Mode = GetNextMode();

	//	�t�F�[�h�����ǂ���
	bool *FadeUse = GetFadeUse();

	VERTEX_2D* pVtx;

	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferEnemy -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{

		//	�G�������Ă����� ���@�t�F�[�h�����Ă��Ȃ�������
		if( g_Enemy[CntEnemy].Use == true && *FadeUse == false )
		{
			if( *Mode == MODE_TITLE )
			{
				//	�^�C�g�����[�h���̓G�̍s��
				ModeTitleEnemy( CntEnemy );

			}

			else if( *Mode == MODE_GAME || *Mode == MODE_TUTORIAL )
			{
				//	�X�V���̓G�̍s��
				UpdateEnemyAct( CntEnemy );
			}

			MakeEnemyPos( pVtx , CntEnemy );	//	�G�̍��W�ύX
		}	//	end of if
	}	//	end of for

	g_pVtxBufferEnemy -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawEnemy(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�̕`��
-----------------------------------------------------------------------------*/
void DrawEnemy(void)
{
	int CntEnemy;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferEnemy , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureEnemy );

	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		if( g_Enemy[CntEnemy].Use == true )
		{
			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntEnemy * NUM_VERTEX , NUM_POLYGON);
		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexEnemy( LPDIRECT3DDEVICE9 pDevice )
 ����:		�Ȃ�
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEnemy( LPDIRECT3DDEVICE9 pDevice )
{
	int CntEnemy;

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_ENEMY  , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferEnemy , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferEnemy -> Lock ( 0 , 0 , ( void** )&pVtx ,  0 );

	for(CntEnemy = 0;CntEnemy < MAX_ENEMY;CntEnemy++)
	{
		//	���_���W�̐ݒ�
		pVtx[ CntEnemy*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x , g_Enemy[CntEnemy].Pos.y , 0.0f );
		pVtx[ CntEnemy*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x+g_Enemy[CntEnemy].Size.x , g_Enemy[CntEnemy].Pos.y , 0.0f );
		pVtx[ CntEnemy*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x , g_Enemy[CntEnemy].Pos.y+g_Enemy[CntEnemy].Size.y , 0.0f );
		pVtx[ CntEnemy*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x+g_Enemy[CntEnemy].Size.x , g_Enemy[CntEnemy].Pos.y+g_Enemy[CntEnemy].Size.y , 0.0f );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntEnemy*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntEnemy*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ CntEnemy*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntEnemy*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntEnemy*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntEnemy*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ CntEnemy*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntEnemy*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntEnemy*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntEnemy*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	g_pVtxBufferEnemy -> Unlock();

	return S_OK;
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void ModeTitleEnemy( int IndexEnemy )
 ����:		int IndexEnemy
 �߂�l:	�Ȃ�
 ����:		�^�C�g�����[�h���̓G�̍s��
-----------------------------------------------------------------------------*/
void ModeTitleEnemy( int IndexEnemy )
{
	g_Enemy[IndexEnemy].Pos.x += g_Enemy[IndexEnemy].Move.x;

	//	��ʊO����
	if( g_Enemy[IndexEnemy].Pos.x > SCREEN_WIDTH )
	{
		g_Enemy[IndexEnemy].Pos.x = -( g_Enemy[IndexEnemy].Size.x );
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateEnemyAct( int IndexEnemy )
 ����:		int IndexEnemy
 �߂�l:	�Ȃ�
 ����:		�X�V���̓G�̍s��
-----------------------------------------------------------------------------*/
void UpdateEnemyAct( int IndexEnemy )
{
	g_Enemy[IndexEnemy].OldPos.x = g_Enemy[IndexEnemy].Pos.x;
	g_Enemy[IndexEnemy].OldPos.y = g_Enemy[IndexEnemy].Pos.y;

	//	�G����ʂɌ��ꂽ��
	if( g_Enemy[IndexEnemy].Pos.x + g_Enemy[IndexEnemy].Size.x < SCREEN_WIDTH )
	{
		//	�ړ�
		g_Enemy[IndexEnemy].Pos.x -= g_Enemy[IndexEnemy].Move.x;
	}	//	end of if

	g_Enemy[IndexEnemy].Move.y += GRAVITY;	//	�d�͂�������
	g_Enemy[IndexEnemy].Pos.y += g_Enemy[IndexEnemy]. Move.y;

	//	��ʂ�������
	if( g_Enemy[IndexEnemy].Pos.x + g_Enemy[IndexEnemy].Size.x < 0 )
	{
		g_Enemy[IndexEnemy].Use = false;
	}	//	end of if
	if( g_Enemy[IndexEnemy].Pos.y > SCREEN_HEIGHT )
	{
		g_Enemy[IndexEnemy].Use = false;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void CreateEnemy( float PosX , float PosY )
 ����:		float PosX
			float PosY
 �߂�l:	�Ȃ�
 ����:		�G�̐���
-----------------------------------------------------------------------------*/
void CreateEnemy( float PosX , float PosY , float Speed )
{
	int CntEnemy;

	for ( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		if( g_Enemy[ CntEnemy ].Use == false )
		{
			g_Enemy[ CntEnemy ].Pos.x = PosX;
			g_Enemy[ CntEnemy ].Pos.y = PosY;
			g_Enemy[CntEnemy].Move.x = Speed;
			g_Enemy[ CntEnemy ].Use = true;
			break;
		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void MakeEnemyPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�G�̍��W�ύX
-----------------------------------------------------------------------------*/
void MakeEnemyPos( VERTEX_2D* pVtx , int IndexEnemy )
{
	pVtx += IndexEnemy*NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x , g_Enemy[IndexEnemy].Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x+g_Enemy[IndexEnemy].Size.x , g_Enemy[IndexEnemy].Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x , g_Enemy[IndexEnemy].Pos.y+g_Enemy[IndexEnemy].Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x+g_Enemy[IndexEnemy].Size.x , g_Enemy[IndexEnemy].Pos.y+g_Enemy[IndexEnemy].Size.y , 0.0f );
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void EnemyTutorial( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�^�C�g���̓G�̔z�u
-----------------------------------------------------------------------------*/
void EnemyTitle( void )
{
	CreateEnemy( -100.0f , SCREEN_HEIGHT , 1.0f );						//	g_Enemy[ 0 ]
	CreateEnemy( -100.0f , SCREEN_HEIGHT , 2.0f );						//	g_Enemy[ 0 ]
	CreateEnemy( -100.0f , SCREEN_HEIGHT , 3.0f );						//	g_Enemy[ 0 ]
}

/*-----------------------------------------------------------------------------
 �֐���:	void EnemyTutorial( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A���̓G�̔z�u
-----------------------------------------------------------------------------*/
void EnemyTutorial( void )
{
	CreateEnemy( SCREEN_WIDTH + 600.0f , SCREEN_HEIGHT  - 100.0f , 1.0f );						//	g_Enemy[ 0 ]

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void EnemyStage1( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�X�e�[�W�P�̓G�̔z�u
-----------------------------------------------------------------------------*/
void EnemyStage1( void )
{
	CreateEnemy( SCREEN_WIDTH + 97.0f , SCREEN_HEIGHT  - 50.0f , 1.0f );						//	g_Enemy[ 0 ]
	CreateEnemy( SCREEN_WIDTH + 1270.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 1 ]
	CreateEnemy( SCREEN_WIDTH + 1700.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 2 ]
	CreateEnemy( SCREEN_WIDTH + 1895.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 3 ]
	CreateEnemy( SCREEN_WIDTH + 3200.0f , SCREEN_HEIGHT - 480.0f , 1.0f );						//	g_Enemy[ 4 ]
	CreateEnemy( SCREEN_WIDTH + 3300.0f , SCREEN_HEIGHT - 480.0f , 1.0f );						//	g_Enemy[ 5 ]
	CreateEnemy( SCREEN_WIDTH + 3900.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 6 ]
	//CreateEnemy( SCREEN_WIDTH + 4000.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 7 ]
	CreateEnemy( SCREEN_WIDTH + 4300.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 8 ]
	CreateEnemy( SCREEN_WIDTH + 4800.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 9 ]
	//CreateEnemy( SCREEN_WIDTH + 4750.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 10 ]
	CreateEnemy( SCREEN_WIDTH + 4930.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 11 ]
	CreateEnemy( SCREEN_WIDTH + 5440.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 12 ]
	CreateEnemy( SCREEN_WIDTH + 5800.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 13 ]
	CreateEnemy( SCREEN_WIDTH + 5890.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 14 ]
	CreateEnemy( SCREEN_WIDTH + 7700.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 15 ]
	CreateEnemy( SCREEN_WIDTH + 7800.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 16 ]

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	ENEMY *GetEnemy( int IndexEnemy )
 ����:		int IndexEnemy
 �߂�l:	return &g_Enemy;
 ����:		�G���̎擾
-----------------------------------------------------------------------------*/
ENEMY *GetEnemy( int IndexEnemy )
{
	return &g_Enemy[ IndexEnemy ];
}	//	end of func