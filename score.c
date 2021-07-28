/*=============================================================================

		�^�C��[ score.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/04
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "score.h"
#include "time.h"
#include "base.h"
#include "enemy.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define SCORE_TEXTURENAME "data/TEXTURE/game/number01.png"

#define SCORE_MAX ( 99999 )	//	�X�R�A�̍ő�l

#define SCORE_X (200.0f)		//	�X�R�A��X���W
#define SCORE_Y (50.0f)			//	�X�R�A��Y���W
#define SCORE_WIDTH (50.0f)		//	�X�R�A��Width
#define SCORE_HEIGHT (50.0f)	//	�X�R�A��Height

#define SCORE_ASCII_X ( 20.0f )		//	�X�R�A������X���W
#define SCORE_ASCII_Y ( 0.0f )		//	�X�R�A������Y���W
#define SCORE_ASCII_WIDTH ( 200.0f )	//	�X�R�A������Width
#define SCORE_ASCII_HEIGHT ( 80.0f )	//	�X�R�A������Height

#define SCORE_ENEMY_DIGIT ( 3 )	//	�G�̃X�R�A�̉����̐����̕\��

#define UP_SCORE_POS ( 35 )	//	�ǂ̈ʃX�R�A�̍��W�̏㏸�����邩
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void UpdateScoreAdd( VERTEX_2D* pVtx );	//	�X�R�A�̉��Z����
void MakeScore( VERTEX_2D* pVtx );	//	�X�R�A�̍쐬
void UpdateEnemyScore( VERTEX_2D* pVtx , int CntEnemy );	//	�G�̃X�R�A�\���X�V
void EnemyScorePos( VERTEX_2D* pVtx , int IndexEnemy );	//	�G�̃X�R�A�̍��W�ύX
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScore = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

int g_Score;	//	���݂̃X�R�A
int g_AddScore;

bool ScoreFlag = false;

D3DXCOLOR g_ScoreColor;	//	�F�ύX

/*-----------------------------------------------------------------------------
 �֐���:	void Initscore( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C���̏�����
-----------------------------------------------------------------------------*/
void InitScore( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , SCORE_TEXTURENAME , &g_pTextureScore  ) ) )
	{
		MessageBox( NULL , "�X�R�A�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	������
	//	�X�R�A
	g_Score = 0;
	g_AddScore = 0;

	ScoreFlag = false;

	g_ScoreColor = D3DXCOLOR( 255 , 255 , 255 , 255 );

	MakeVertexScore( pDevice );	//	���_�̍쐬

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void Uninitscore( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�X�R�A�̏I��
-----------------------------------------------------------------------------*/
void UninitScore( void )
{
	g_Score = 0;

	ScoreFlag = false;


	if( g_pTextureScore != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureScore -> Release();
		g_pTextureScore = NULL;
	}
	if(g_pVtxBufferScore != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferScore -> Release();
		g_pVtxBufferScore  = NULL;
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdeteScore( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�X�R�A�̍X�V
-----------------------------------------------------------------------------*/
void UpdateScore( void )
{
	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�G�̏��̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferScore -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	if( *Mode == MODE_GAME )
	{
		UpdateScoreAdd( pVtx );	//	�X�R�A�̉��Z����
	}

	g_pVtxBufferScore -> Unlock();
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void Drawscore( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�X�R�A�̕`��
-----------------------------------------------------------------------------*/
void DrawScore( void )
{
	int loop;

	//	�G�̏��̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferScore , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureScore );

	for(loop = 0;loop < SCORE_DIGIT;loop++)
	{
		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , loop*4, NUM_POLYGON);
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice )
{
	int i;
	int num;	//	������\������ϐ�
	int value = g_AddScore , enemyscorevalue = 100;	//	�X�R�A�̒l

	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * SCORE_DIGIT , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferScore , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferScore -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	for(i = 0;i < SCORE_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	���_���W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );

		//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ i*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ i*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ i*NUM_VERTEX+3 ].rhw = 1.0f;

		//	���_�F�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ i*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ i*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ i*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.1f*num , 0 );
		pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0 );
		pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.1f*num , 0.5f );
		pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0.5f );

		//	�X�R�A�����̕\��
		if(i == SCORE_DIGIT-1)
		{
			//	���_���W�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );

			//	���W�ϊ��ςݒ��_�t���O�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].rhw = 1.0f;
			pVtx[ i*NUM_VERTEX+1 ].rhw = 1.0f;
			pVtx[ i*NUM_VERTEX+2 ].rhw = 1.0f;
			pVtx[ i*NUM_VERTEX+3 ].rhw = 1.0f;

			//	���_�F�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
			pVtx[ i*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
			pVtx[ i*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
			pVtx[ i*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

			//	�e�N�X�`�����W�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
		}
	}
	
	g_pVtxBufferScore -> Unlock();

	return S_OK;

}
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateScoreAdd( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�X�R�A�̉��Z����
-----------------------------------------------------------------------------*/
void UpdateScoreAdd( VERTEX_2D* pVtx )
{
	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	//	�^�C�����̎擾
	int *Time = GetTime();

	//	�^�C����0�ȉ��ɂȂ�����
	if( *Time <= 0 )
	{
		ScoreFlag = true;
	}

	//	�J���X�g����
	if( g_Score > SCORE_MAX )
	{
		g_Score = SCORE_MAX;
	}

	if( g_Score > g_AddScore )
	{
		if( Base -> GoolFalg == false )
		{
			g_AddScore += 10;

			g_ScoreColor.r = 0;
			g_ScoreColor.g = 0;
			g_ScoreColor.b = 255;
		}
		else if( Base -> GoolFalg == true )
		{
			g_AddScore += 200;

			g_ScoreColor.r = 0;
			g_ScoreColor.g = 0;
			g_ScoreColor.b = 255;
		}
		//	�������
		if( g_Score <= g_AddScore )
		{
			g_AddScore = g_Score;
		}	//	end of if
	}	//	end of if

	MakeScore(pVtx);
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void MakeScore( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�X�R�A�̍쐬
-----------------------------------------------------------------------------*/
void MakeScore( VERTEX_2D* pVtx )
{
	int i;
	int num;	//	������\������ϐ�
	int value = g_AddScore;

	if(value >= SCORE_MAX)	//	�J���X�g����
	{
		value = SCORE_MAX;
	}
	for(i = 0;i < SCORE_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	���_���W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );

		//	���_�F�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );
		pVtx[ i*NUM_VERTEX+1 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );
		pVtx[ i*NUM_VERTEX+2 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );
		pVtx[ i*NUM_VERTEX+3 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.1f*num , 0 );
		pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0 );
		pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.1f*num , 0.5f );
		pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0.5f );

		//	�X�R�A�����̕\��
		if(i == SCORE_DIGIT-1)
		{
			//	���_���W�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );

			//	�e�N�X�`�����W�̐ݒ�
			//pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0.5f );
			//pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.3f , 0.5f );
			//pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1.0f );
			//pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.3f , 1.0f );

			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
		}
	}
}
/*-----------------------------------------------------------------------------
 �֐���:	int *GetScore(void)
 ����:		
 �߂�l:	�Ȃ�
 ����:		�X�R�A�̏��
-----------------------------------------------------------------------------*/
int *GetScore(void)
{
	return &g_Score;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	bool *GetScoreFlag( void )
 ����:		
 �߂�l:	return &ScoreFlag;
 ����:		�X�R�A�t���O�̏��
-----------------------------------------------------------------------------*/
bool *GetScoreFlag( void )
{
	return &ScoreFlag;
}	//	end of func
