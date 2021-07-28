/*=============================================================================

		�w�i�`��[ bg.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/28
-------------------------------------------------------------------------------
	���@Update
		2016/07/28
			�E�w�b�_�t�@�C���ǉ�
				#include <stdio.h>
				#include "main.h"
				#include "bg.h"
			�E�萔��`�ǉ�
				#define BG_TEXTURENAME
				#define BG_POS_X ( 0.0f )	//	�w�i�̕\���ʒu�w
				#define BG_POS_Y ( 0.0f )	//	�w�i�̕\���ʒu�x
			�E�O���[�o���ϐ��ǉ�
				LPDIRECT3DTEXTURE9 g_pTexturebg = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
				LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBg = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
			�E�v���g�^�C�v�錾�ǉ�
				HRESULT MakeVertexBg( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
=============================================================================*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "bg.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define BG_TEXTURENAME "data/TEXTURE/func/bg.jpg"

#define BG_POS_X ( 0.0f )	//	�w�i�̕\���ʒu�w
#define BG_POS_Y ( 0.0f )	//	�w�i�̕\���ʒu�x
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTexturebg = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBg = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

float g_BgTex = 0.0f;
float g_Vset = 0.0f;
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBg( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void MakeBgTex( VERTEX_2D* pVtx );	//	�w�i�̃e�N�X�`�����W�ύX
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitBg( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�|���S���̏�����
-----------------------------------------------------------------------------*/
void InitBg( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N	�P��
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , BG_TEXTURENAME , &g_pTexturebg  ) ) )
	{
		MessageBox( NULL , "�w�i�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}
	MakeVertexBg( pDevice );	//	���_�̍쐬
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitBg( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�|���S���̏I��
-----------------------------------------------------------------------------*/
void UninitBg( void )
{
	//	������
	g_BgTex = 0.0f;
	g_Vset = 0.0f;


	if( g_pTexturebg != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturebg -> Release();
		g_pTexturebg = NULL;
	}	//	end of if
	if(g_pVtxBufferBg != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferBg -> Release();
		g_pVtxBufferBg  = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdataBg( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�|���S���̍X�V
-----------------------------------------------------------------------------*/
void UpdateBg( void )
{
	VERTEX_2D* pVtx;

	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferBg -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	MakeBgTex( pVtx );

	g_pVtxBufferBg -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawBg( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�|���S���̕`��
-----------------------------------------------------------------------------*/
void DrawBg( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferBg , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTexturebg );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void MakeVertexBg( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBg( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferBg , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferBg -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( BG_POS_X , BG_POS_Y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( BG_POS_X+SCREEN_WIDTH , BG_POS_Y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( BG_POS_X , BG_POS_Y+SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( BG_POS_X+SCREEN_WIDTH , BG_POS_Y+SCREEN_HEIGHT , 0.0f );

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

	g_pVtxBufferBg -> Unlock();

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void MakeBgTex( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�w�i�̃e�N�X�`�����W�ύX
-----------------------------------------------------------------------------*/
void MakeBgTex( VERTEX_2D* pVtx )
{
	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 + g_BgTex , 0 + g_Vset );
	pVtx[ 1 ].tex = D3DXVECTOR2( 0.01f + g_BgTex , 0 + g_Vset );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 + g_BgTex , 0.1f + g_Vset );
	pVtx[ 3 ].tex = D3DXVECTOR2( 0.01f + g_BgTex , 0.1f + g_Vset );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	float *GetBgTex(void)
 ����:		return &g_BgTex;
 �߂�l:	�Ȃ�
 ����:		�w�i�̃e�N�X�`��U���W�擾
-----------------------------------------------------------------------------*/
float *GetBgTex(void)
{
	return &g_BgTex;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	float *GetVTex( void )
 ����:		return &g_Vset;
 �߂�l:	�Ȃ�
 ����:		�w�i�̃e�N�X�`��V���W�擾
-----------------------------------------------------------------------------*/
float *GetVTex( void )
{
	return &g_Vset;
}