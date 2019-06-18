/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "main.h"
#include "score.h"
#include "particle.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define PARTICLE_TEXTURENAME "data/TEXTURE/game/effect001.png"

#define MAX_PARTICLE ( 70 )

#define PARTICLE_SPEED ( 5.0f )	//	�p�[�e�B�N���X�s�[�h
/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR2 pos;	//	���W
	D3DXVECTOR2 size;	//	�傫��
	D3DXVECTOR2 move;	//	�ړ���
	D3DXVECTOR2 rot;	//	����
	D3DXCOLOR color;	//	�J���[
	float length;	//	����
	float fRadius;	//	���a
	float fRadiusValue;	//���a�̕ω���
	float fColorAlphaValue;	//	�F�̕ω���
	int nLife;	//	����
	bool bUse;	//	�g�p�t���O
}PARTICLE;
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void ParticlePos( VERTEX_2D* pVtx , int IndexParticle );	//	���W�X�V
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticle = NULL;

PARTICLE g_Particle[MAX_PARTICLE];

/*-----------------------------------------------------------------------------
 �֐���:	void Initparticle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̏�����
-----------------------------------------------------------------------------*/
void Initparticle( void )
{
	int loop;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PARTICLE_TEXTURENAME , &g_pTextureParticle  ) ) )
	{
		MessageBox( NULL , "�p�[�e�B�N���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�p�[�e�B�N���̏���������
	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		g_Particle[loop].pos.x = 0.0f;
		g_Particle[loop].pos.y = 0.0f;
		g_Particle[loop].size.x = 0.0f;
		g_Particle[loop].size.y = 0.0f;
		g_Particle[loop].move.x = 0.0f;
		g_Particle[loop].move.y = 0.0f;
		g_Particle[loop].color.r = 255;
		g_Particle[loop].color.g = 255;
		g_Particle[loop].color.b = 0;
		g_Particle[loop].color.a = 256;
		g_Particle[loop].length = 5.0f;
		g_Particle[loop].fRadius = 30.0f;
		g_Particle[loop].fColorAlphaValue = 5.0f;
		g_Particle[loop].nLife = 0;
		g_Particle[loop].bUse = false;
	}	//	end of for

	MakeVertexParticle( pDevice );	//	���_�̍쐬

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void Uninitparticle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̏I��
-----------------------------------------------------------------------------*/
void Uninitparticle( void )
{
	if( g_pTextureParticle != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureParticle -> Release();
		g_pTextureParticle = NULL;
	}	//	end of if
	if(g_pVtxBufferParticle != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferParticle -> Release();
		g_pVtxBufferParticle = NULL;
	}	//	end of if
}
/*-----------------------------------------------------------------------------
 �֐���:	void Updateparticle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̍X�V
-----------------------------------------------------------------------------*/
void Updateparticle( void )
{
	int loop;

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferParticle -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		if( g_Particle[loop].bUse == true )
		{
			//	�n�G���ۂ�
			//g_Particle[loop].move.x = sinf( ( ( rand()%628 ) - 314 ) / 100 ) * g_Particle[loop].length;
			//g_Particle[loop].move.y = cosf( ( ( rand()%628 ) - 314 ) / 100 ) * g_Particle[loop].length;

			g_Particle[loop].pos.x += g_Particle[loop].move.x;
			g_Particle[loop].pos.y += g_Particle[loop].move.y;

			g_Particle[loop].nLife--;	//	���C�t�����炷

			g_Particle[loop].fRadius -= g_Particle[loop].fRadiusValue;

			//	���C�t���Ȃ��Ȃ�����
			if( g_Particle[loop].nLife <= 0 )
			{
				g_Particle[loop].bUse  = false;
			}	//	end of if

			//	��ʊO����
			if( g_Particle[loop].pos.x + g_Particle[loop].size.x < 0 || g_Particle[loop].pos.x > SCREEN_WIDTH || g_Particle[loop].pos.y + g_Particle[loop].size.x < 0 || g_Particle[loop].pos.y > SCREEN_HEIGHT )
			{
				g_Particle[loop].bUse  = false;
			}	//	end of if

			ParticlePos( pVtx , loop );	//	���W�X�V
		}	//	end of if
	}	//	end of for

	g_pVtxBufferParticle -> Unlock();

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void Drawparticle( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̕`��
-----------------------------------------------------------------------------*/
void Drawparticle( void )
{
	int loop;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferParticle , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureParticle );

	//	a�u�����h�i���Z�����j
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );	//	Init�ɏ����Ă���̂ŏȗ���
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );	//	Init�ɏ����Ă���̂ŏȗ���
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		if(g_Particle[loop].bUse  == true)
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
 �֐���:	HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̕`��
-----------------------------------------------------------------------------*/
HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice )
{
	int loop = 0;

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_PARTICLE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferParticle , NULL ) ) )
	{
		return E_FAIL;
	}	 //	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferParticle -> Lock ( 0 , 0 ,( void** )&pVtx , 0 );


	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		//	���_���W�̐ݒ�
		pVtx[ loop*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Particle[loop].pos.x - g_Particle[loop].fRadius                                        , g_Particle[loop].pos.y - g_Particle[loop].fRadius                                        , 0.0f );
		pVtx[ loop*NUM_VERTEX+1 ].pos = D3DXVECTOR3( ( g_Particle[loop].pos.x + g_Particle[loop].size.x ) - g_Particle[loop].fRadius , g_Particle[loop].pos.y - g_Particle[loop].fRadius                                        , 0.0f );
		pVtx[ loop*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Particle[loop].pos.x - g_Particle[loop].fRadius                                        , ( g_Particle[loop].pos.y + g_Particle[loop].size.y ) - g_Particle[loop].fRadius , 0.0f );
		pVtx[ loop*NUM_VERTEX+3 ].pos = D3DXVECTOR3( ( g_Particle[loop].pos.x + g_Particle[loop].size.x ) - g_Particle[loop].fRadius , ( g_Particle[loop].pos.y + g_Particle[loop].size.y ) - g_Particle[loop].fRadius , 0.0f );

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

	g_pVtxBufferParticle -> Unlock();

	return S_OK;
}
/*-----------------------------------------------------------------------------
 �֐���:	void ParticlePos( VERTEX_2D* pVtx , int IndexParticle )
 ����:		VERTEX_2D* pVtx
			int IndexParticle
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̍��W�X�V
-----------------------------------------------------------------------------*/
void ParticlePos( VERTEX_2D* pVtx , int IndexParticle )
{
	pVtx += IndexParticle * NUM_VERTEX;

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Particle[IndexParticle].pos.x - g_Particle[IndexParticle].fRadius                                        , g_Particle[IndexParticle].pos.y - g_Particle[IndexParticle].fRadius                                        , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( ( g_Particle[IndexParticle].pos.x + g_Particle[IndexParticle].size.x ) - g_Particle[IndexParticle].fRadius , g_Particle[IndexParticle].pos.y - g_Particle[IndexParticle].fRadius                                        , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Particle[IndexParticle].pos.x - g_Particle[IndexParticle].fRadius                                        , ( g_Particle[IndexParticle].pos.y + g_Particle[IndexParticle].size.y ) - g_Particle[IndexParticle].fRadius , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( ( g_Particle[IndexParticle].pos.x + g_Particle[IndexParticle].size.x ) - g_Particle[IndexParticle].fRadius , ( g_Particle[IndexParticle].pos.y + g_Particle[IndexParticle].size.y ) - g_Particle[IndexParticle].fRadius , 0.0f );

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void SetParticle( float PosX , float PosY )
 ����:		float PosX
			float PosY
 �߂�l:	�Ȃ�
 ����:		�p�[�e�B�N���̃Z�b�g
-----------------------------------------------------------------------------*/
void SetParticle( float PosX , float PosY )
{
	int loop;

	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		if( g_Particle[loop].bUse == false )
		{

			g_Particle[loop].pos.x = PosX + rand()%20;
			g_Particle[loop].pos.y = PosY + rand()%20;

			g_Particle[loop].move.x = sinf( ( ( rand()%628 ) - 314 ) / 100 ) * ( g_Particle[loop].length + rand()%4 );
			g_Particle[loop].move.y = cosf( ( ( rand()%628 ) - 314 ) / 100 ) * ( g_Particle[loop].length + rand()%4 );

			g_Particle[loop].size.x = 50.0f;
			g_Particle[loop].size.y = 50.0f;

			g_Particle[loop].nLife = 25 + rand()%5;

			g_Particle[loop].fRadiusValue = g_Particle[loop].fRadius / g_Particle[loop].nLife;
			g_Particle[loop].fColorAlphaValue = 1.0f / g_Particle[loop].nLife;
			g_Particle[loop].bUse = true;

			break;
		}	//	end of if
	}	//	end of for
}	//	end of func