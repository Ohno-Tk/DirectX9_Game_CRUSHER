/*=============================================================================

		����`��[ base.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/08/22
-------------------------------------------------------------------------------
	���@Update
		2016/08/22
			�E�w�b�_�t�@�C���ǉ�
				#include <stdio.h>
				#include "main.h"
				#include "base.h"
				#include "bg.h"
			�E�萔��`�ǉ�
				#define BASE_TEXTURENAME
			�E�v���g�^�C�v�錾�ǉ�
				HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
				void BasePos( VERTEX_2D* pVtx , int IndexBase );			//	������W�X�V
				void CreateBase( float PosX , float PosY , float Width , float Height );	//	����̐���
			�E�O���[�o���ϐ��ǉ�
				LPDIRECT3DTEXTURE9 g_pTextureBase = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
				LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBase = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
				BASE g_Base[ MAX_BASE ];
		2016/08/27
			�E�w�b�_�t�@�C���ǉ�
				#include "player.h"
			�E�v���g�^�C�v�錾�ǉ�
			void Create( void );	//	��ʃX�N���[�����̑���̐���
			void BaseDecision( VERTEX_2D* pVtx );	//	����̔���
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "base.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define BASE_TEXTURENAME "data/TEXTURE/game/base.jpg"
#define GOOL_TEXTURENAME "data/TEXTURE/game/gool.png"
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice );						//	���_�̍쐬
void GoolPos( VERTEX_2D* pVtx );											//	�S�[�����W�X�V
void BasePos( VERTEX_2D* pVtx , int IndexBase );							//	������W�X�V
void CreateBase( float PosX , float PosY , float Width , float Height );	//	����̐���
void BaseTutorial( void );	//	�`���[�g���A���̑���̐���
void BaseStage1( void );													//	�X�e�[�W�P�̑���̐���
void BaseDecision( VERTEX_2D* pVtx );										//	����̔���
void PlayerBaseDecision( VERTEX_2D* pVtx );									//	�v���C���[�̑��ꔻ��
void EnemyBaseDecision( VERTEX_2D* pVtx );									//	�G�̑��ꔻ��
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureBase = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBase = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
//	�S�[��
LPDIRECT3DTEXTURE9 g_pTextureGool = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

BASE g_Base[ MAX_BASE ];

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitBase( void )
 ����:		�Ȃ�
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		����̏�����
-----------------------------------------------------------------------------*/
HRESULT InitBase( void )
{
	int CntBase;

	//	�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , BASE_TEXTURENAME , &g_pTextureBase  ) ) )
	{
		MessageBox( NULL , "����̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		return E_FAIL;
	}
	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , GOOL_TEXTURENAME , &g_pTextureGool  ) ) )
	{
		MessageBox( NULL , "�S�[���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		return E_FAIL;
	}
	
	//	���ꏉ����
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ )
	{
		g_Base[ CntBase ].size.x = 0.0f;	//	Width	
		g_Base[ CntBase ].size.y = 0.0f;	//	Height
		g_Base[ CntBase ].move_X = MOVE_REPOSIT;	//	�ړ���
		g_Base[ CntBase ].use = false;		//	�g�p�t���O
	}	//	end of for

	if( *Mode == MODE_TITLE )
	{
		CreateBase( -20.0f , SCREEN_HEIGHT - 50.0f , SCREEN_WIDTH + 20.0f , 50.0f );
	}	

	else if( *Mode == MODE_TUTORIAL )
	{
		BaseTutorial();	//	�`���[�g���A���̑���̐���

		//	�S�[��������
		g_Base[ MAX_GOOL ].pos.x = SCREEN_WIDTH + 2800.0f;
		g_Base[ MAX_GOOL ].pos.y = 200.0f;
		g_Base[ MAX_GOOL ].size.x = 100.0f;
		g_Base[ MAX_GOOL ].size.y = 323.0f;
		g_Base[ MAX_GOOL ].move_X = MOVE_REPOSIT;
		g_Base[ MAX_GOOL ].GoolFalg = false;
		g_Base[ MAX_GOOL ].use = true;
	}	//	end of if

	else if( *Mode == MODE_GAME )
	{
		BaseStage1();	//	�X�e�[�W�P�̑���̐���

		//	�S�[��������
		g_Base[ MAX_GOOL ].pos.x = SCREEN_WIDTH + 8700.0f;
		g_Base[ MAX_GOOL ].pos.y = 100.0f;
		g_Base[ MAX_GOOL ].size.x = 100.0f;
		g_Base[ MAX_GOOL ].size.y = 430.0f;
		g_Base[ MAX_GOOL ].move_X = 3.0f;
		g_Base[ MAX_GOOL ].GoolFalg = false;
		g_Base[ MAX_GOOL ].use = true;

	}	//	end of else if

	MakeVertexBase( pDevice );	//	���_�̍쐬

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitBase( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		����̏I��
-----------------------------------------------------------------------------*/
void UninitBase( void )
{
	if( g_pTextureBase != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureBase -> Release();
		g_pTextureBase = NULL;
	}
	if(g_pVtxBufferBase != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferBase -> Release();
		g_pVtxBufferBase  = NULL;
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateBase( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		����̍X�V
-----------------------------------------------------------------------------*/
void UpdateBase( void )
{
	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferBase -> Lock ( 0 ,0 ,( void** )&pVtx ,0 );

	GoolPos( pVtx );

	if( g_Base[ MAX_GOOL ].GoolFalg == false )
	{
		BaseDecision( pVtx );
	}	//	end of if

	g_pVtxBufferBase -> Unlock();
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawBase( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		����̕`��
-----------------------------------------------------------------------------*/
void DrawBase( void )
{
	int CntBase;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferBase , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureBase );

	//	����
	for( CntBase = 0 ; CntBase < MAX_BASE - 1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == true )
		{
			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntBase * NUM_VERTEX, NUM_POLYGON);
		}	//	end of if
	}	//	end of for

	//	�S�[��
	pDevice -> SetTexture( 0 , g_pTextureGool );

	if( g_Base[ MAX_GOOL ].use == true )
	{
		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , MAX_GOOL * NUM_VERTEX, NUM_POLYGON);
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice )
{
	int CntBase;

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_BASE  , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferBase , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferBase -> Lock ( 0 , 0 , ( void** )&pVtx ,  0 );
	
	//	����
	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		//	���_���W�̐ݒ�
		BasePos( pVtx , CntBase );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntBase*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntBase*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntBase*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	//	�S�[��
	for( CntBase = MAX_GOOL ; CntBase < MAX_BASE ; CntBase++ )
	{
		//	�S�[��
		//	���_���W�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
		pVtx[ CntBase*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
		pVtx[ CntBase*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );
		pVtx[ CntBase*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ CntBase*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntBase*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntBase*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntBase*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}

	g_pVtxBufferBase -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void CreateBase( float PosX , float PosY , float Width , float Height )
 ����:		float PosX
			float PosY
			float Width
 �߂�l:	�Ȃ�
 ����:		����̐���
-----------------------------------------------------------------------------*/
void CreateBase( float PosX , float PosY , float Width , float Height )
{
	int CntBase;

	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == false )
		{
			g_Base[ CntBase ].pos.x = PosX;		//	X���W
			g_Base[ CntBase ].pos.y = PosY;		//	Y���W
			g_Base[ CntBase ].size.x = Width;	//	Width
			g_Base[ CntBase ].size.y = Height;	//	Height
			g_Base[ CntBase ].use = true;		//	�g�p�t���O
			break;
		}
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void BaseTutorial( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�`���[�g���A���̑���̐���
-----------------------------------------------------------------------------*/
void BaseTutorial( void )
{
	//	����̐���
	CreateBase( -20.0f , SCREEN_HEIGHT - 50.0f , SCREEN_WIDTH + 1950.0f , 50.0f );

	CreateBase( SCREEN_WIDTH + 2025.0f , SCREEN_HEIGHT - 50.0f , 2800.0f , 50.0f );

	//	�K�C�h����
	CreateBase( 300.0f , SCREEN_HEIGHT - 200.0f , 150.0f , 50.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void BaseStage1( void )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�X�e�[�W�P�̑���̐���
-----------------------------------------------------------------------------*/
void BaseStage1( void )
{
	//	����̐���
	CreateBase( -20.0f , SCREEN_HEIGHT - 50.0f , SCREEN_WIDTH + 2600.0f , 50.0f );	
	CreateBase( SCREEN_WIDTH + 2700.0f , SCREEN_HEIGHT - 50.0f , 840.0f , 50.0f );	
	CreateBase( SCREEN_WIDTH + 3675.0f , SCREEN_HEIGHT - 50.0f , 3075.0f , 50.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 50.0f , 3100.0f , 50.0f );	

	//	����P
	CreateBase( SCREEN_WIDTH - 150.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	����Q
	CreateBase( SCREEN_WIDTH + 225.0f , SCREEN_HEIGHT - 200.0f , 200.0f , 50.0f );

	//	����R
	CreateBase( SCREEN_WIDTH + 305.0f , SCREEN_HEIGHT - 350.0f , 50.0f , 50.0f );	

	//	����S
	CreateBase( SCREEN_WIDTH + 600.0f , SCREEN_HEIGHT - 150.0f , 80.0f , 100.0f );	

	//	����T
	CreateBase( SCREEN_WIDTH + 950.0f, SCREEN_HEIGHT - 200.0f , 80.0f , 150.0f );	

	//	����U
	CreateBase( SCREEN_WIDTH + 1400.0f , SCREEN_HEIGHT - 225.0f , 80.0f , 175.0f );	

	//	����7
	CreateBase( SCREEN_WIDTH + 2000.0f , SCREEN_HEIGHT - 225.0f , 80.0f , 175.0f );	

	//	����W
	CreateBase( SCREEN_WIDTH + 3000.0f , SCREEN_HEIGHT - 225.0f , 150.0f , 50.0f );

	//	����X
	CreateBase( SCREEN_WIDTH + 3200.0f , SCREEN_HEIGHT - 380.0f , 400.0f , 50.0f );	

	//	����P�O
	CreateBase( SCREEN_WIDTH + 3700.0f , SCREEN_HEIGHT - 380.0f , 200.0f , 50.0f );	

	//	����P�P
	CreateBase( SCREEN_WIDTH + 3850.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	����P�Q
	CreateBase( SCREEN_WIDTH + 4150.0f , SCREEN_HEIGHT - 225.0f , 100.0f , 50.0f );

	//	����P�R
	CreateBase( SCREEN_WIDTH + 4450.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	����P�R
	CreateBase( SCREEN_WIDTH + 4600.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	����P�S
	CreateBase( SCREEN_WIDTH + 4600.0f , SCREEN_HEIGHT - 350.0f , 50.0f , 50.0f );	

	//	����P�T
	CreateBase( SCREEN_WIDTH + 4750.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	����P�U
	CreateBase( SCREEN_WIDTH + 5050.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	����P�V
	CreateBase( SCREEN_WIDTH + 5150.0f , SCREEN_HEIGHT - 300.0f , 150.0f , 50.0f );

	//	����P�W
	CreateBase( SCREEN_WIDTH + 5550.0f , SCREEN_HEIGHT - 300.0f , 200.0f , 50.0f );

	//	����P�X
	CreateBase( SCREEN_WIDTH + 5600.0f , SCREEN_HEIGHT - 200.0f , 100.0f , 50.0f );

	//	����P�X( �K�i�u���b�N )
	CreateBase( SCREEN_WIDTH + 5850.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 5900.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 5950.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6000.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	����Q�O( �t�K�i�u���b�N )
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	����Q�P( �K�i�u���b�N )
	CreateBase( SCREEN_WIDTH + 6550.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6600.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6650.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6700.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	����Q�Q( �t�K�i�u���b�N )
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	����Q�R
	CreateBase( SCREEN_WIDTH + 7250.0f , SCREEN_HEIGHT - 150.0f  , 50.0f , 100.0f );

	//	����Q�S
	CreateBase( SCREEN_WIDTH + 7450.0f , SCREEN_HEIGHT - 200.0f  , 200.0f , 50.0f );

	//	����Q�T
	CreateBase( SCREEN_WIDTH + 8000.0f , SCREEN_HEIGHT - 150.0f  , 50.0f , 100.0f );

	//	����Q�U( �K�i�u���b�N )
	CreateBase( SCREEN_WIDTH + 8050.0f , SCREEN_HEIGHT - 90.0f  , 450.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 8100.0f , SCREEN_HEIGHT - 130.0f  , 400.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8150.0f , SCREEN_HEIGHT - 170.0f  , 350.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8200.0f , SCREEN_HEIGHT - 210.0f  , 300.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8250.0f , SCREEN_HEIGHT - 250.0f  , 250.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8300.0f , SCREEN_HEIGHT - 290.0f  , 200.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8350.0f , SCREEN_HEIGHT - 330.0f  , 150.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8400.0f , SCREEN_HEIGHT - 370.0f  , 100.0f , 40.0f );//	g_Base[ 51 ]

	//	�K�C�h����
	CreateBase( 300.0f , SCREEN_HEIGHT - 200.0f , 150.0f , 50.0f );


}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void BaseDecision( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		����̔���
-----------------------------------------------------------------------------*/
void BaseDecision( VERTEX_2D* pVtx )
{
	//	�v���C���[���̎擾
	PLAYER *Player = GetPlayer();

	//	�v���C���[���G�Ɠ���������
	if( Player -> Hit == false )
	{
		//	�v���C���[�̑��ꔻ��
		PlayerBaseDecision( pVtx );
	}	//	end of if

	//	�G�̑��ꔻ��
	EnemyBaseDecision( pVtx );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void PlayerBaseDecision( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̑��ꔻ��
-----------------------------------------------------------------------------*/
void PlayerBaseDecision( VERTEX_2D* pVtx )
{
	int CntBase;

	//	�v���C���[���̎擾
	PLAYER *Player = GetPlayer();

	//	�v���C���[�Ƃ̔���
	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == true )
		{
			//	����Ƃ̔���
			if( g_Base[ CntBase ].pos.x <= ( Player->Pos.x + Player->Size.x ) - 35.0f &&  Player->Pos.x + 10.0f < g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x - 10.0f )
			{
				//��̔���
				if( g_Base[ CntBase ].pos.y < Player->Pos.y + Player->Size.y && Player->OldPos.y <= g_Base[ CntBase ].pos.y )
				{					

					Player->Move.y = 0.0f;
					Player->Pos.y = g_Base[ CntBase ].pos.y - Player->Size.y;
					Player->Jump = false;

				}	//	end of if

				//���̔���
				if( Player->Pos.y <= g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y && Player->OldPos.y > g_Base[ CntBase ].pos.y )
				{
					Player->Move.y = 0.0f;
					Player->Pos.y = g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y/* + 12.0f*/;
				}	//	end of if
			}	//	end of if

			//	���ǂ̔���
			if( g_Base[ CntBase ].pos.y < Player->Pos.y + Player->Size.y && Player->Pos.y < g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y )
			{

				//	���ǂƂ̔���
				if(  g_Base[ CntBase ].pos.x < Player->Pos.x + Player->Size.x - 10.0f && Player->Pos.x + Player->Size.x - 10.0f < g_Base[ CntBase ].pos.x + 12.0f )
				{
					Player->Pos.x = g_Base[ CntBase ].pos.x - Player->Size.x + 10.0f;
				}	//	end of if

				//	�E�ǂƂ̔���
				if( Player->Pos.x < g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x && g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x < Player->Pos.x + 10.0f )
				{
					Player->Pos.x = g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x + 10.0f;
				}	//	end of if
			}	//	end of if

			//	������W�X�V
			BasePos( pVtx , CntBase );

		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void EnemyBaseDecision( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�G�̑��ꔻ��
-----------------------------------------------------------------------------*/
void EnemyBaseDecision( VERTEX_2D* pVtx )
{
	int CntBase , CntEnemy;

	//	�G�̏��̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	�G�Ƃ̔���
	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == true )
		{
			for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
			{
				if( Enemy[CntEnemy].Use ==true )
				{

					//	����Ƃ̔���
					if( g_Base[ CntBase ].pos.x <= Enemy[CntEnemy].Pos.x + Enemy[CntEnemy].Size.x &&  Enemy[CntEnemy].Pos.x < ( g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x ) - 15.0f )
					{

						//��̔���
						if( g_Base[ CntBase ].pos.y < Enemy[CntEnemy].Pos.y + Enemy[CntEnemy].Size.y && Enemy[CntEnemy].OldPos.y <= g_Base[ CntBase ].pos.y )
						{
							Enemy[CntEnemy].Move.y = 0.0f;
							Enemy[CntEnemy].Pos.y = g_Base[ CntBase ].pos.y - Enemy[CntEnemy].Size.y;
						}	//	end of if

						//////���̔���
						//if( Enemy[CntEnemy].Pos.y <= g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y && Enemy[CntEnemy].OldPos.y > g_Base[ CntBase ].pos.y )
						//{
						//	Enemy[CntEnemy].Move.y = 0.0f;
						//	Enemy[CntEnemy].Pos.y = g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y + 12.0f;
						//}
					}	//	end of if

					//	���ǂ̔���
					if( g_Base[ CntBase ].pos.y < Enemy[CntEnemy].Pos.y + Enemy[CntEnemy].Size.y && Enemy[CntEnemy].Pos.y < g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y )
					{

						//	���ǂƂ̔���
						if(  g_Base[ CntBase ].pos.x < Enemy[CntEnemy].Pos.x + Enemy[CntEnemy].Size.x - 10.0f && Enemy[CntEnemy].Pos.x + Enemy[CntEnemy].Size.x - 10.0f < g_Base[ CntBase ].pos.x + 12.0f )
						{
							Enemy[CntEnemy].Pos.x = g_Base[ CntBase ].pos.x - Enemy[CntEnemy].Size.x + 10.0f;
							Enemy[CntEnemy].Move.x *= -1.0f;
						}	//	end of if

						//	�E�ǂƂ̔���
						if( Enemy[CntEnemy].Pos.x < g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x && g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x < Enemy[CntEnemy].Pos.x + 20.0f )
						{
							Enemy[CntEnemy].Pos.x = g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x + 10.0f;
							Enemy[CntEnemy].Move.x *= -1.0f;
						}	//	end of if
					}	//	end of if
				}	//	end of if

				//	������W�X�V
				BasePos( pVtx , CntBase );

			}	//	end of for
		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void GoolPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�S�[�����W�X�V
-----------------------------------------------------------------------------*/
void GoolPos( VERTEX_2D* pVtx )
{
	pVtx += MAX_GOOL * NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void BasePos( VERTEX_2D* pVtx , int IndexBase )
 ����:		VERTEX_2D* pVtx
			int IndexBase
 �߂�l:	�Ȃ�
 ����:		������W�X�V
-----------------------------------------------------------------------------*/
void BasePos( VERTEX_2D* pVtx , int IndexBase )
{
	pVtx += IndexBase * NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x , g_Base[ IndexBase ].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x+g_Base[ IndexBase ].size.x , g_Base[ IndexBase ].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x , g_Base[ IndexBase ].pos.y+g_Base[ IndexBase ].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x+g_Base[ IndexBase ].size.x , g_Base[ IndexBase ].pos.y+g_Base[ IndexBase ].size.y , 0.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	BASE *GetBase( int IndexBase )
 ����:		�Ȃ�
 �߂�l:	return &g_Base[IndexBase];
 ����:		������̎擾
-----------------------------------------------------------------------------*/
BASE *GetBase( int IndexBase )
{
	return &g_Base[IndexBase];
}	//	end of func