/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "effect.h"
#include "input.h"
#include "enemy.h"
#include "usability.h"
#include "score.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define EFFECT_TEXTURENAME "data/TEXTURE/game/effect000.jpg"

#define MAX_LIFE ( 30 )

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void EffectPos( VERTEX_2D* pVtx , int IndexParticle );	//	���W�X�V
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEffect = NULL;

EFFECT g_Effect[MAX_EFFECT];

/*-----------------------------------------------------------------------------
 �֐���:	void InitEffect( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�t�F�N�g�̏�����
-----------------------------------------------------------------------------*/
void InitEffect( void )
{
	int loop;

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , EFFECT_TEXTURENAME , &g_pTextureEffect  ) ) )
	{
		MessageBox( NULL , "�G�t�F�N�g�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�p�[�e�B�N���̏���������
	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		g_Effect[loop].pos.x = 0.0f;
		g_Effect[loop].pos.y = 0.0f;
		g_Effect[loop].size.x = 50.0f;
		g_Effect[loop].size.y = 50.0f;
		g_Effect[loop].move.x = 0.0f;
		g_Effect[loop].move.y = 5.0f;
		g_Effect[loop].color.r = 128;
		g_Effect[loop].color.g = 255;
		g_Effect[loop].color.b = 128;
		g_Effect[loop].color.a = 255;
		g_Effect[loop].length = 5.0f;
		g_Effect[loop].fRadius = 10.0f;
		g_Effect[loop].fColorAlphaValue = 5.0f;
		g_Effect[loop].nLife = MAX_LIFE;
		g_Effect[loop].bUse = false;
	}	//	end of for

	MakeVertexEffect( pDevice );	//	���_�̍쐬

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitEffect( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�t�F�N�g�̏I��
-----------------------------------------------------------------------------*/
void UninitEffect( void )
{
	if( g_pTextureEffect != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureEffect -> Release();
		g_pTextureEffect = NULL;
	}	//	end of if
	if(g_pVtxBufferEffect != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferEffect -> Release();
		g_pVtxBufferEffect = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateEffect( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̍X�V
-----------------------------------------------------------------------------*/
void UpdateEffect( void )
{
	int loop;

	//	�G���̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	�X�R�A���̎擾
	int *Score = GetScore();

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferEffect -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		if( g_Effect[loop].bUse  == true )
		{

			//	�G�t�F�N�g�̃^�C�v�ɂ���čX�V��ς���

			//	�U���^�C�v��������
			if( g_Effect[loop].type == EFFECT_TYPE_ATTACK )
			{
				g_Effect[loop].pos.x += g_Effect[loop].move.x;
			}	//	end of if

			//	���������炷
			g_Effect[loop].nLife--;

			//	�������Ȃ��Ȃ�����
			if( g_Effect[loop].nLife <= 0 )
			{
				g_Effect[loop].bUse  = false;
				
			}	//	end of if

			//	��ʊO����
			if( g_Effect[loop].pos.x > SCREEN_HEIGHT || g_Effect[loop].pos.x < 0 )
			{
				g_Effect[loop].bUse  = false;
			}

			//	���W�X�V
			EffectPos( pVtx , loop );

		}	//	end of if
	}	//	end of for

	g_pVtxBufferEffect -> Unlock();

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawEffect( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�G�t�F�N�g�̕`��
-----------------------------------------------------------------------------*/
void DrawEffect( void )
{
	int loop;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferEffect , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureEffect );

	//	a�u�����h�i���Z�����j
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );	//	Init�ɏ����Ă���̂ŏȗ���
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );	//	Init�ɏ����Ă���̂ŏȗ���
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		if(g_Effect[loop].bUse  == true)
		{
			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , loop * NUM_VERTEX, NUM_POLYGON);
		}	//	end of if
	}	//	end of for

	//	���ɖ߂�
	pDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
{
	int loop = 0;

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_EFFECT , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferEffect , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferEffect -> Lock ( 0 , 0 ,( void** )&pVtx , 0 );


	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		//	���_���W�̐ݒ�
		pVtx[ loop*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x , g_Effect[loop].pos.y , 0.0f );
		pVtx[ loop*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x+g_Effect[loop].size.x , g_Effect[loop].pos.y , 0.0f );
		pVtx[ loop*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x , g_Effect[loop].pos.y+g_Effect[loop].size.y , 0.0f );
		pVtx[ loop*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x+g_Effect[loop].size.x , g_Effect[loop].pos.y+g_Effect[loop].size.y , 0.0f );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ loop*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ loop*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ loop*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ loop*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ loop*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ loop*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ loop*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ loop*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ loop*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ loop*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	g_pVtxBufferEffect -> Unlock();

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void EffectPos( VERTEX_2D* pVtx , int IndexParticle )
 ����:		VERTEX_2D* pVtx
			int IndexParticle
 �߂�l:	�Ȃ�
 ����:		�G�t�F�N�g�̈ړ�
-----------------------------------------------------------------------------*/
void EffectPos( VERTEX_2D* pVtx , int IndexParticle )
{
	pVtx += IndexParticle * NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x , g_Effect[IndexParticle].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x+g_Effect[IndexParticle].size.x , g_Effect[IndexParticle].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x , g_Effect[IndexParticle].pos.y+g_Effect[IndexParticle].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x+g_Effect[IndexParticle].size.x , g_Effect[IndexParticle].pos.y+g_Effect[IndexParticle].size.y , 0.0f );

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void SetParticle( float PosX , float PosY )
 ����:		float PosX
			float PosY
 �߂�l:	�Ȃ�
 ����:		�G�t�F�N�g�̐ݒu
-----------------------------------------------------------------------------*/
void SetEffect( float PosX , float PosY , float Speed , EFFECT_TYPE type )
{
	int loop;

	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		if( g_Effect[loop].bUse == false )
		{
			g_Effect[loop].pos.x = PosX;
			g_Effect[loop].pos.y = PosY;
			g_Effect[loop].move.x = Speed;
			g_Effect[loop].nLife = MAX_LIFE;
			g_Effect[loop].type = type;
			g_Effect[loop].bUse = true;

			//	�G�t�F�N�g�^�C�v�ɂ����Width��Height�̕ύX
			if( g_Effect[loop].type == EFFECT_TYPE_ATTACK )
			{
				g_Effect[loop].size.x = 50.0f;
				g_Effect[loop].size.y = 10.0f;
			}	//	end of if

			break;
		}	//	end of if
	}	//	end of for
}	//	end of func
EFFECT *GetEffect( void )
{
	return &g_Effect[0];
}