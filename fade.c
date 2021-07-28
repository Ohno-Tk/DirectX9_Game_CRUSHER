/*=============================================================================

		�t�F�[�h����[ fade.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/04
-------------------------------------------------------------------------------
	���@Update
		2016/04/18
=============================================================================*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define FADE_TEXTURENAME "data/TEXTURE/func/fade.png"

#define FADE_POS_X ( 0.0f )	//	�t�F�[�h�̕\���ʒu�w
#define FADE_POS_Y ( 0.0f )	//	�t�F�[�h�̕\���ʒu�x

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexFade( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void SetColorFade(D3DXCOLOR g_colorFade);	//	���_�J���[�̐ݒ�
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

D3DXCOLOR g_colorFade;	//	�t�F�[�h�F
FADE g_fade;	//	�t�F�[�h���
MODE g_modenext = MODE_TITLE;	//	���̃��[�h

bool g_FadeUse = false;
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitFade( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�t�F�[�h�̏�����
-----------------------------------------------------------------------------*/
void InitFade( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , FADE_TEXTURENAME , &g_pTextureFade  ) ) )
	{
		MessageBox( NULL , "�e�N�X�`���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}

	MakeVertexFade( pDevice );	//	���_�̍쐬

	g_colorFade = D3DXCOLOR(0.0f , 0.0f , 0.0f , 1.0f);
	g_fade = FADE_IN;

	g_FadeUse = false;
}

/*-----------------------------------------------------------------------------
 �֐���:	void UninitFade( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�t�F�[�h�̏I��
-----------------------------------------------------------------------------*/
void UninitFade( void )
{
	if( g_pTextureFade != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureFade -> Release();
		g_pTextureFade = NULL;
	}
	if(g_pVtxBufferFade != NULL)	//	���_�o�b�t�@�̊J��
	{
		g_pVtxBufferFade -> Release();
		g_pVtxBufferFade = NULL;
	}
}
/*-----------------------------------------------------------------------------
 �֐���:	void UpdataFade( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�t�F�[�h�̍X�V
-----------------------------------------------------------------------------*/
void UpdateFade( void )
{

	if(g_fade == FADE_NONE)	//	�t�F�[�h���Ȃ����̏���
	{
		return;
	}
	if(g_fade == FADE_IN)	//	�t�F�[�h�C�����̏���
	{
		g_colorFade.a -= FADE_RATE;	//	a�l�����Z���Č��̉�ʂ𕂂��яオ�点��
		//	�t�F�[�h�C���̏I��
		if(g_colorFade.a < 0.0f)
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;

			g_FadeUse = false;
		}
	}
	else if(g_fade == FADE_OUT)	//	�t�F�[�h�A�E�g���̏���
	{
		g_colorFade.a += FADE_RATE;	//	a�l�����Z���Č��̉�ʂ������Ă���

		//	�t�F�[�h�A�E�g�̏I��
		if(g_colorFade.a > 1.0f)
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;	//	�t�F�[�h�C���ɏ����̐؂�ւ�
			SetMode(g_modenext);

		}
	}
	//	���_�J���[�̐ݒ�
	SetColorFade(g_colorFade);

}
/*-----------------------------------------------------------------------------
 �֐���:	void DrawFade( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�t�F�[�h�̕`��
-----------------------------------------------------------------------------*/
void DrawFade( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferFade , 0 , sizeof( VERTEX_2D ) );

	//	�e�N�X�`���ݒ�
	pDevice -> SetTexture( 0 , g_pTextureFade );

	//pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );
	//pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	//pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	//	�t�F�[�h�̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
}

/*-----------------------------------------------------------------------------
 �֐���:	void MakeVertexFade( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexFade( LPDIRECT3DDEVICE9 pDevice )
{
	
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferFade , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferFade -> Lock ( 0 ,
		                          0 ,
								  ( void** )&pVtx ,      // ���z(�^��)�A�h���X�擾
								  0 );

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( FADE_POS_X , FADE_POS_Y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( FADE_POS_X+SCREEN_WIDTH , FADE_POS_Y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( FADE_POS_X , FADE_POS_Y+SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( FADE_POS_X+SCREEN_WIDTH , FADE_POS_Y+SCREEN_HEIGHT , 0.0f );


	//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferFade -> Unlock();

	return S_OK;

}
/*-----------------------------------------------------------------------------
 �֐���:	void SetColorFade(D3DXCOLOR g_colorFade)
 ����:		D3DXCOLOR g_colorFade
 �߂�l:	�Ȃ�
 ����:		���_�J���[�̐ݒ�
-----------------------------------------------------------------------------*/
void SetColorFade(D3DXCOLOR colorFade)
{
	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferFade -> Lock ( 0 ,
		                          0 ,
								  ( void** )&pVtx ,      // ���z(�^��)�A�h���X�擾
								  0 );
	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );
	pVtx[ 1 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );
	pVtx[ 2 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );
	pVtx[ 3 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );

	g_pVtxBufferFade -> Unlock();

}
/*-----------------------------------------------------------------------------
 �֐���:	void SetFade( FADE fade , MODE modenext )
 ����:		FADE fade
			MODE modenext
 �߂�l:	�Ȃ�
 ����:		�t�F�[�h�̐ݒ�
-----------------------------------------------------------------------------*/
void SetFade( FADE fade , MODE modenext )
{
	g_fade = fade;
	g_modenext = modenext;

	g_FadeUse = true;

}
/*-----------------------------------------------------------------------------
 �֐���:	MODE *GetNextMode( void )
 ����:		�Ȃ�
 �߂�l:	return &g_modenext;
 ����:		���̃��[�h�̒ǉ�
-----------------------------------------------------------------------------*/
MODE *GetNextMode( void )
{
	return &g_modenext;
}

/*-----------------------------------------------------------------------------
 �֐���:	bool *GetFadeUse( void )
 ����:		�Ȃ�
 �߂�l:	return &g_FadeUse;
 ����:		�t�F�[�h�����ǂ���
-----------------------------------------------------------------------------*/
bool *GetFadeUse( void )
{
	return &g_FadeUse;
}