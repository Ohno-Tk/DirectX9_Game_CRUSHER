/*=============================================================================

		�v���C���[[ player.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/28
-------------------------------------------------------------------------------
	���@Update
		2016/07/28
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "guid.h"
#include "base.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define GUID_TEXTURENAME "data/TEXTURE/func/guid.png"

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGuid = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureGuid = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

GUIDSIGNBOARD g_Guid;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexGuid( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void GuidPos( VERTEX_2D* pVtx );	//	���W�ύX
/*-----------------------------------------------------------------------------
 �֐���:	void InitGuid( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�ē��̏�����
-----------------------------------------------------------------------------*/
void InitGuid( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	������̎擾
	BASE *Base = GetBase( 0 );

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , GUID_TEXTURENAME , &g_pTextureGuid  ) ) )
	{
		MessageBox( NULL , "�ē��̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}

	//	������
	g_Guid.pos.x = 300.0f;
	g_Guid.pos.y = SCREEN_HEIGHT - 400.0f;
	g_Guid.size.x = 150.0f;
	g_Guid.size.y = 200.0f;
	g_Guid.move_X = Base -> move_X;

	//	���_�̍쐬
	MakeVertexGuid( pDevice );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitGuid( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�ē��̏I��
-----------------------------------------------------------------------------*/
void UninitGuid( void )
{
	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	if(g_pVtxBufferGuid != NULL)
	{
		g_pVtxBufferGuid -> Release();
		g_pVtxBufferGuid = NULL;
	}	//	end of if

	//	�e�N�X�`���|���S���J��
	if( g_pTextureGuid != NULL )
	{
		g_pTextureGuid -> Release();
		g_pTextureGuid = NULL;
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateGuid( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�ē��̍X�V
-----------------------------------------------------------------------------*/
void UpdateGuid( void )
{
	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferGuid -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	���W�ύX
	GuidPos( pVtx );

	g_pVtxBufferGuid -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawGuid( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�ē��̕`��
-----------------------------------------------------------------------------*/
void DrawGuid( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferGuid , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureGuid );

	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexGuid( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexGuid( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferGuid , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferGuid -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y + g_Guid.size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y + g_Guid.size.y , 0.0f );

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


	g_pVtxBufferGuid -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void GuidPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		���W�ύX
-----------------------------------------------------------------------------*/
void GuidPos( VERTEX_2D* pVtx )
{
	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y + g_Guid.size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y + g_Guid.size.y , 0.0f );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	GUIDSIGNBOARD *GetGuid( void )
 ����:		
 �߂�l:	return &g_Guid;
 ����:		�ē����̎擾
-----------------------------------------------------------------------------*/
GUIDSIGNBOARD *GetGuid( void )
{
	return &g_Guid;
}