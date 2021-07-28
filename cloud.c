/*=============================================================================

		�_[ cloud.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/09/14
-------------------------------------------------------------------------------
	���@Update
	2016/09/14
		�E�w�b�_�t�@�C���ǉ�
			#include <stdio.h>
			#include "main.h"
			#include "cloud.h"
		�E�萔��`�ǉ�
			#define CLOUD_TEXTURENAME
		�E�O���[�o���ϐ��ǉ�
			LPDIRECT3DTEXTURE9 g_pTextureCloud = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
			LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCloud = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

			CLOUD g_Cloud[ MAX_CLOUD ];
		�E�v���g�^�C�v�錾�ǉ�
			HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
			void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud );	//	�X�V���̉_�̍s��
			void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud );	//	���W�̍X�V
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "cloud.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define CLOUD_TEXTURENAME "data/TEXTURE/game/�_.png"

#define MAX_CLOUD ( 7 )	//	�_�̍ő吔
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureCloud = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCloud = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

CLOUD g_Cloud[ MAX_CLOUD ];
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud );	//	�X�V���̉_�̍s��
void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud );	//	���W�̍X�V
/*-----------------------------------------------------------------------------
 �֐���:	void InitCloud(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�_�̏�����
-----------------------------------------------------------------------------*/
void InitCloud(void)
{
	int CntCloud;

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , CLOUD_TEXTURENAME , &g_pTextureCloud  ) ) )
	{
		MessageBox( NULL , "�_�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	if( *Mode == MODE_TITLE )
	{
		//	������
		for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
		{
			g_Cloud[ CntCloud ].pos.x = 0.0f;
			g_Cloud[ CntCloud ].pos.y = 0.0f;
			g_Cloud[ CntCloud ].size.x = 200.0f;
			g_Cloud[ CntCloud ].size.y = 150.0f;
			g_Cloud[ CntCloud ].move_X = 2.0f + rand()%3;
			g_Cloud[ CntCloud ].use = true;
		}	//	end of for

		g_Cloud[ 0 ].pos.x = 100.0f;
		g_Cloud[ 0 ].pos.y = 0.0f;

		g_Cloud[ 1 ].pos.x = 200.0f;
		g_Cloud[ 1 ].pos.y = 100.0f;

		g_Cloud[ 2 ].pos.x = 450.0f;
		g_Cloud[ 2 ].pos.y = 50.0f;

		g_Cloud[ 3 ].pos.x = 550.0f;
		g_Cloud[ 3 ].pos.y = 80.0f;

		g_Cloud[ 4 ].pos.x = 600.0f;
		g_Cloud[ 4 ].pos.y = 30.0f;

		g_Cloud[ 5 ].pos.x = 600.0f;
		g_Cloud[ 5 ].pos.y = 90.0f;

		g_Cloud[ 6 ].pos.x = 800.0f;
		g_Cloud[ 6 ].pos.y = 100.0f;

	}

	else
	{
		//	������
		for( CntCloud = 0 ; CntCloud < MAX_CLOUD - 3 ; CntCloud++ )
		{
			g_Cloud[ CntCloud ].pos.x = rand()%800;
			g_Cloud[ CntCloud ].pos.y = rand()%100;
			g_Cloud[ CntCloud ].size.x = 200.0f;
			g_Cloud[ CntCloud ].size.y = 150.0f;
			g_Cloud[ CntCloud ].move_X = 2.0f + rand()%3;
			g_Cloud[ CntCloud ].use = true;
		}	//	end of for

		for(  ; CntCloud < MAX_CLOUD ; CntCloud++ )
		{
			g_Cloud[ CntCloud ].pos.x = rand()%800;
			g_Cloud[ CntCloud ].pos.y = rand()%10;
			g_Cloud[ CntCloud ].size.x = 200.0f;
			g_Cloud[ CntCloud ].size.y = 150.0f;
			g_Cloud[ CntCloud ].move_X = 2.0f + rand()%3;
			g_Cloud[ CntCloud ].use = true;
		}	//	end of for
	}

	MakeVertexCloud( pDevice );	//	���_�̍쐬

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitCloud(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�_�̏I��
-----------------------------------------------------------------------------*/
void UninitCloud(void)
{
	if( g_pTextureCloud != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureCloud -> Release();
		g_pTextureCloud = NULL;
	}	//	end of if
	if(g_pVtxBufferCloud != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferCloud -> Release();
		g_pVtxBufferCloud = NULL;
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateCloud(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�_�̍X�V
-----------------------------------------------------------------------------*/
void UpdateCloud(void)
{
	int CntCloud;

	VERTEX_2D* pVtx;

	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferCloud -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
	{
		if( g_Cloud[ CntCloud ].use = true )
		{
			//	�X�V���̉_�̓���
			UpdateCloudAct( pVtx , CntCloud );

		}	//	end of if
	}	//	end of for

	g_pVtxBufferCloud -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawCloud(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�_�̕`��
-----------------------------------------------------------------------------*/
void DrawCloud(void)
{
	int CntCloud;

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferCloud , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureCloud );

	for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
	{
		if( g_Cloud[CntCloud].use == true )
		{
			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntCloud * NUM_VERTEX , NUM_POLYGON);
		}	//	end of if
	}	//	end of for

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		�_�̕`��
-----------------------------------------------------------------------------*/
HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice )
{
	int CntCloud;

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_CLOUD  , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferCloud , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferCloud -> Lock ( 0 , 0 , ( void** )&pVtx ,  0 );

	for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
	{
		//	���_���W�̐ݒ�
		pVtx[ CntCloud*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y , 0.0f );
		pVtx[ CntCloud*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y , 0.0f );
		pVtx[ CntCloud*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );
		pVtx[ CntCloud*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntCloud*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ CntCloud*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ CntCloud*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntCloud*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntCloud*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntCloud*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ CntCloud*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntCloud*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntCloud*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntCloud*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	g_pVtxBufferCloud -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud )
 ����:		VERTEX_2D* pVtx
			int CntCloud
 �߂�l:	�Ȃ�
 ����:		�X�V���̉_�̓���
-----------------------------------------------------------------------------*/
void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud )
{

	g_Cloud[ CntCloud ].pos.x -= g_Cloud[ CntCloud ].move_X;

	//	��ʊO����
	if( g_Cloud[ CntCloud ].pos.x + g_Cloud[ CntCloud ].size.x < 0 )
	{
		g_Cloud[ CntCloud ].pos.x = SCREEN_WIDTH;
	}	//	end of if

	MakeVertexCloudPos( pVtx , CntCloud );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud )
 ����:		VERTEX_2D* pVtx
			int CntCloud
 �߂�l:	�Ȃ�
 ����:		���W�̍X�V
-----------------------------------------------------------------------------*/
void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud )
{
	pVtx += CntCloud * NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	CLOUD *GetCloud( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�_�̏��̎擾
-----------------------------------------------------------------------------*/
CLOUD *GetCloud( void )
{
	return &g_Cloud[ 0 ];
}	//	end of func