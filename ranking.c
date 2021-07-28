/*=============================================================================

		�����L���O[ ranking.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/09/05
-------------------------------------------------------------------------------
	���@�쐬��
		2016/09/05
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
		scanf ��warning�h�~ 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "result.h"
#include "ranking.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define ENABLE_PEOC_SAVE	//	�Z�[�u�����L��
#undef ENABLE_PEOC_SAVE	//	( �}�N���𖳌��ɂ��� )

#ifdef ENABLE_PEOC_SAVE
	#define FILENAME "data/RANKING_DATA/save.txt"	//	�Z�[�u���閼�O
#else
	#define FILENAME "data/RANKING_DATA/save.bin"	//	�Z�[�u���閼�O
#endif

//	����
#define RANKING_SCORE_TEXTURENAME "data/TEXTURE/ranking/number000.png"
//	����
#define RANKING_RANK_TEXTURENAME "data/TEXTURE/ranking/ranking_rank.png"

//	�����L���O�w�i
#define RANKING_BG_TEXTURENAME "data/TEXTURE/func/titleBg.jpg"
//	�����L���O����
#define RANKING_ASCII_TEXTURENAME "data/TEXTURE/ranking/RankingAscii.png"
//	�{�^��
#define RANKING_PUSH_TEXTURENAME "data/TEXTURE/func/ButtomRogo .png"

#define RANKING_PAD_TEXTURENAME "data/TEXTURE/func/ButtomPad.png"

#define MAX_RANKING ( 5 )	//	�����L���O�̍ő吔

#define TEXTURE ( 3 )	//	�����Ə��ʂ��������e�N�X�`���̐�

#define RANKING_X ( 750.0f )	//	�����L���O������X
#define RANKING_Y ( 500.0f )	//	�����L���O������Y
#define RANKING_WIDTH ( 100.0f )	//	�����L���O������WIDTH
#define RANKING_HEIGHT ( 50.0f )	//	�����L���O������HEIGHT

#define RANKING_ASCII_X ( 300.0f )	//	�����L���O������X
#define RANKING_ASCII_Y ( 0.0f )	//	�����L���O������Y
#define RANKING_ASCII_WIDTH ( 400.0f )	//	�����L���O������WIDTH
#define RANKING_ASCII_HEIGHT ( 100.0f )	//	�����L���O������HEIGHT

#define RANKING_RANK_X ( 100.0f )	//	���ʂ�X
#define RANKING_RANK_Y ( RANKING_Y )	//	���ʂ�Y
#define RANKING_RANK_WIDTH ( 80.0f )	//	���ʂ�WIDTH
#define RANKING_RANK_HEIGHT ( 100.0f )	//	���ʂ�X
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexRanking( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void RankingButtomFade( VERTEX_2D* pVtx );	//	�{�^���̃t�F�[�h
void RankTex( VERTEX_2D* pVtx );	//	���ʂ̃e�N�X�`���ƐF��ύX
void Save( void );	//	�Z�[�u
void Load( void );	//	���[�h
void BubbleSort( void );	//	�o�u���\�[�g
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRanking = NULL;	//	���_�o�b�t�@

//	�����L���O����
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�����L���O�w�i
LPDIRECT3DTEXTURE9 g_pTextureRankingBg = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�����L���O����
LPDIRECT3DTEXTURE9 g_pTextureRankingAscii = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	����
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

//	�{�^��
LPDIRECT3DTEXTURE9 g_pTextureRankingButtom = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

RANKING g_RankingList[ MAX_RANKING ] = { 0 };

FADE g_RankingFade = FADE_IN;	//	�t�F�[�h���
D3DXCOLOR g_RankingFadeColor;	//	�t�F�[�h�F
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitRanking( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�����L���O�̏�����
-----------------------------------------------------------------------------*/
void InitRanking( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�X�R�A�̎擾
	int *Hiscore = GetScore();


	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_SCORE_TEXTURENAME , &g_pTextureRanking  ) ) )
	{
		MessageBox( NULL , "�i���o�[�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_BG_TEXTURENAME , &g_pTextureRankingBg  ) ) )
	{
		MessageBox( NULL , "�����L���O�w�i�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_ASCII_TEXTURENAME , &g_pTextureRankingAscii  ) ) )
	{
		MessageBox( NULL , "�����L���O�����̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_RANK_TEXTURENAME , &g_pTextureRankingRank  ) ) )
	{
		MessageBox( NULL , "���ʂ̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	�Q�[���p�b�h������������
	if( GamePad[ 0 ] == NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_PUSH_TEXTURENAME , &g_pTextureRankingButtom  ) ) )
		{
			MessageBox( NULL , "�{�^���O�P�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	�Q�[���p�b�h����������
	else if( GamePad[ 0 ] != NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_PAD_TEXTURENAME , &g_pTextureRankingButtom  ) ) )
		{
			MessageBox( NULL , "�{�^���O�Q�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}


#ifdef _DEBUG
	for( int Cnt2 = 0 ; Cnt2 < MAX_RANKING ; Cnt2++ )
	{
		g_RankingList[ Cnt2 ].score = 0;
	}

	//	�Z�[�u
	Save();
#endif
	
	//	���[�h
	Load();

	//	�o�u���\�[�g
	BubbleSort();

	//	�����L���O�̈�ԉ��ƃQ�[���X�R�A�̔�r
	if( g_RankingList[ 0 ].score <= *Hiscore )
	{
	//	//	���O�̓���
	//	g_RankingList[ MAX_RANKING - 1 ].name[ 0 ] = 0;
	//	g_RankingList[ MAX_RANKING - 1 ].name[ 0 ] = 10;
	//	g_RankingList[ MAX_RANKING - 1 ].name[ 0 ] = 17;

		//	�����L���O�̈�ԉ��Ƀn�C�X�R�A������
		g_RankingList[ 0 ].score = *Hiscore;

		//	�o�u���\�[�g
		BubbleSort();
	}	//	end of if

	//	�Z�[�u
	Save();
		

	g_RankingFadeColor = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	���_�̍쐬
	MakeVertexRanking( pDevice );

	//	���y�Đ�
	PlaySound( SOUND_LABEL_BGM_RESULT );

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitRanking( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�����L���O�̏I��
-----------------------------------------------------------------------------*/
void UninitRanking( void )
{

	if(g_pVtxBufferRanking != NULL)	//	���_�o�b�t�@�̊J��
	{
		g_pVtxBufferRanking -> Release();
		g_pVtxBufferRanking = NULL;
	}	//	end of if

	//	�����L���O����
	if( g_pTextureRanking != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureRanking -> Release();
		g_pTextureRanking = NULL;
	}	//	end of if

	//	�����L���O�w�i
	if( g_pTextureRankingBg != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureRankingBg -> Release();
		g_pTextureRankingBg = NULL;
	}	//	end of if

	//	�����L���O����
	if( g_pTextureRankingAscii != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureRankingAscii -> Release();
		g_pTextureRankingAscii = NULL;
	}	//	end of if

	//	����
	if( g_pTextureRankingRank != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureRankingRank -> Release();
		g_pTextureRankingRank = NULL;
	}	//	end of if

	//	�{�^��
	if( g_pTextureRankingButtom != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureRankingButtom -> Release();
		g_pTextureRankingButtom = NULL;
	}	//	end of if

	UninitScore();	//	�X�R�A�̏I��
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdateRanking( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�����L���O�̍X�V
-----------------------------------------------------------------------------*/
void UpdateRanking( void )
{
	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�t�F�[�h�����ǂ���
	bool *FadeUse = GetFadeUse();

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferRanking -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	���ʂ̃e�N�X�`���ƐF��ύX
	RankTex( pVtx );

	//	�{�^���̃t�F�[�h
	RankingButtomFade( pVtx );

	g_pVtxBufferRanking -> Unlock();

	if( *FadeUse == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
		{
			if( GamePad[ CntPad ] == NULL )
			{
				//	��ʑJ��
				if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
				{
					SetFade(FADE_OUT , MODE_TITLE);

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	���y��~
					StopSound( SOUND_LABEL_BGM_RESULT );
				}	//	end of if
			}
			if( GamePad[ CntPad ] != NULL )
			{
				if( GetGamePadTrigger( BUTTOM_04 ) )
				{
					SetFade(FADE_OUT , MODE_TITLE);

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	���y��~
					StopSound( SOUND_LABEL_BGM_RESULT );
				}
			}
		}
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawRanking( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�����L���O�̕`��
-----------------------------------------------------------------------------*/
void DrawRanking( void )
{
	int Cnt;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferRanking , 0 , sizeof( VERTEX_2D ) );

	//	�����L���O�w�i

	//	�e�N�X�`���ݒ�
	pDevice -> SetTexture( 0 , g_pTextureRankingBg );

	//	�t�F�[�h�̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);

	//	�����L���O����

	//	�e�N�X�`���ݒ�
	pDevice -> SetTexture( 0 , g_pTextureRankingAscii );

	//	�t�F�[�h�̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);

	//	����
	//	�e�N�X�`���ݒ�
	pDevice -> SetTexture( 0 , g_pTextureRankingRank );

	for( Cnt = 0 ; Cnt < MAX_RANKING ; Cnt++ )
	{
		//	�t�F�[�h�̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , ( Cnt + TEXTURE ) * NUM_VERTEX , NUM_POLYGON);
	}	//	end of for

	//	�����L���O����

	//	�e�N�X�`���ݒ�
	pDevice -> SetTexture( 0 , g_pTextureRanking );

	for( Cnt = 0 ; Cnt < MAX_RANKING * ( SCORE_DIGIT - 1 ) ; Cnt++ )
	{
		//	�t�F�[�h�̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , ( Cnt + TEXTURE + MAX_RANKING ) * NUM_VERTEX , NUM_POLYGON);
	}	//	end of for

	//	�{�^��

	//	�e�N�X�`���ݒ�
	pDevice -> SetTexture( 0 , g_pTextureRankingButtom );

	//	�t�F�[�h�̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8 , NUM_POLYGON);

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexRanking( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexRanking( LPDIRECT3DDEVICE9 pDevice )
{
	int Cnt1 , Cnt2;
	int num;	//	������\������ϐ�
	int value;
	
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * ( ( SCORE_DIGIT * MAX_RANKING ) + TEXTURE + MAX_RANKING ) , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferRanking , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferRanking -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	�����L���O�w�i

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f                , 0.0f                 , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f                 , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f                , 0.0f + SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f + SCREEN_HEIGHT , 0.0f );

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

	//	�|�C���^�����炷
	pVtx += 4;

	//	�����L���O����

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_ASCII_X                       , RANKING_ASCII_Y                 , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_ASCII_X + RANKING_ASCII_WIDTH , RANKING_ASCII_Y                 , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_ASCII_X                       , RANKING_ASCII_Y + RANKING_ASCII_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_ASCII_X + RANKING_ASCII_WIDTH , RANKING_ASCII_Y + RANKING_ASCII_HEIGHT , 0.0f );

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

	//	�|�C���^�����炷
	pVtx += 4;

	//	�{�^��

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , ( RESULT_PUSH_POS_Y + 28.0f ) , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , ( RESULT_PUSH_POS_Y + 28.0f ) , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , ( RESULT_PUSH_POS_Y + 28.0f ) + RESULT_PUSH_POS_HEIGHT - 20.0f , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , ( RESULT_PUSH_POS_Y + 28.0f ) + RESULT_PUSH_POS_HEIGHT - 20.0f , 0.0f );

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

	//	�|�C���^�����炷
	pVtx += 4;

	//	����
	for( Cnt1 = 0 ; Cnt1 < MAX_RANKING ; Cnt1++ )
	{
			//	���_���W�̐ݒ�
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_RANK_X                      , ( RANKING_RANK_Y - 20.0f )                       - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_RANK_X + RANKING_RANK_WIDTH , ( RANKING_RANK_Y - 20.0f )                       - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_RANK_X                      , ( RANKING_RANK_Y - 20.0f ) + RANKING_RANK_HEIGHT - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_RANK_X + RANKING_RANK_WIDTH , ( RANKING_RANK_Y - 20.0f ) + RANKING_RANK_HEIGHT - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );

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

			//	�|�C���^�����炷
			pVtx += 4;

	}	//	end of for

	//	�����L���O����
	for( Cnt1 = 0 ; Cnt1 < MAX_RANKING ; Cnt1++ )
	{
		value = g_RankingList[ Cnt1 ].score;

		for( Cnt2 = 0 ; Cnt2 < SCORE_DIGIT - 1 ; Cnt2++ )
		{
			num = value % 10;
			value /= 10;

			//	���_���W�̐ݒ�
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_X                 - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y                  - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_X + RANKING_WIDTH - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y                  - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_X                 - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y + RANKING_HEIGHT - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_X + RANKING_WIDTH - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y + RANKING_HEIGHT - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );

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
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.1f * num        , 0 );
			pVtx[ 1 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 0 );
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.1f * num        , 1 );
			pVtx[ 3 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 1 );

			//	�|�C���^�����炷
			pVtx += 4;
		}	//	end of for

	}	//	end of for

	g_pVtxBufferRanking -> Unlock();

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void RankingButtomFade( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�{�^���̃t�F�[�h
-----------------------------------------------------------------------------*/
void RankingButtomFade( VERTEX_2D* pVtx )
{
	if( g_RankingFade == FADE_IN )	//	�t�F�[�h�C�����̏���
	{
		g_RankingFadeColor.a -= FADE_RATE;	//	a�l�����Z���Č��̉�ʂ𕂂��яオ�点��
		//	�t�F�[�h�C���̏I��
		if( g_RankingFadeColor.a < 0.0f )
		{
			g_RankingFadeColor.a = 0.0f;
			g_RankingFade = FADE_OUT;
		}	//	end od if
	}	//	end of if

	else if( g_RankingFade == FADE_OUT )	//	�t�F�[�h�A�E�g���̏���
	{
		g_RankingFadeColor.a += FADE_RATE;	//	a�l�����Z���Č��̉�ʂ������Ă���
		//	�t�F�[�h�A�E�g�̏I��
		if( g_RankingFadeColor.a > 1.0f )
		{
			g_RankingFadeColor.a = 1.0f;
			g_RankingFade = FADE_IN;	//	�t�F�[�h�C���ɏ����̐؂�ւ�
		}	//	end of if
	}	//	end of else if

	//	���_�F�̐ݒ�
	pVtx[ 8  ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
	pVtx[ 9  ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
	pVtx[ 10 ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
	pVtx[ 11 ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void RankTex( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		���ʂ̃e�N�X�`���ƐF��ύX
-----------------------------------------------------------------------------*/
void RankTex( VERTEX_2D* pVtx )
{
	//	5��

	//	���_�F�̐ݒ�
	pVtx[ 12 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 13 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 14 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 15 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 12 ].tex = D3DXVECTOR2( 0 , 0.8f );
	pVtx[ 13 ].tex = D3DXVECTOR2( 1 , 0.8f );
	pVtx[ 14 ].tex = D3DXVECTOR2( 0 , 1.0f );
	pVtx[ 15 ].tex = D3DXVECTOR2( 1 , 1.0f );

	//	4��

	//	���_�F�̐ݒ�
	pVtx[ 16 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 17 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 18 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 19 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 , 0.6f );
	pVtx[ 17 ].tex = D3DXVECTOR2( 1 , 0.6f );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 , 0.8f );
	pVtx[ 19 ].tex = D3DXVECTOR2( 1 , 0.8f );

	//	3��

	//	���_�F�̐ݒ�
	pVtx[ 20 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );
	pVtx[ 21 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );
	pVtx[ 22 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );
	pVtx[ 23 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 20 ].tex = D3DXVECTOR2( 0 , 0.4f );
	pVtx[ 21 ].tex = D3DXVECTOR2( 1 , 0.4f );
	pVtx[ 22 ].tex = D3DXVECTOR2( 0 , 0.6f );
	pVtx[ 23 ].tex = D3DXVECTOR2( 1 , 0.6f );

	//	2��

	//	���_�F�̐ݒ�
	pVtx[ 24 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );
	pVtx[ 25 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );
	pVtx[ 26 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );
	pVtx[ 27 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 24 ].tex = D3DXVECTOR2( 0 , 0.2f );
	pVtx[ 25 ].tex = D3DXVECTOR2( 1 , 0.2f );
	pVtx[ 26 ].tex = D3DXVECTOR2( 0 , 0.4f );
	pVtx[ 27 ].tex = D3DXVECTOR2( 1 , 0.4f );

	//	1��

	//	���_�F�̐ݒ�
	pVtx[ 28 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );
	pVtx[ 29 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );
	pVtx[ 30 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );
	pVtx[ 31 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 28 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 29 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 30 ].tex = D3DXVECTOR2( 0 , 0.2f );
	pVtx[ 31 ].tex = D3DXVECTOR2( 1 , 0.2f );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void Save( void )
 ����:		
 �߂�l:
 ����:		�Z�[�u
-----------------------------------------------------------------------------*/
void Save( void )
{
	FILE *fp;	//	�t�@�C���|�C���^

	fp = fopen( FILENAME , " wb " );	//	�t�@�C�����J��
									
	if( fp == NULL )	//	�G���[�`�F�b�N
	{
		MessageBox( NULL , "�t�@�C�����J���܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

//	�e�L�X�g�f�[�^�ŃZ�[�u������
#ifdef ENABLE_PEOC_SAVE
	// �f�[�^���Z�[�u������
	for( int CntSave = 0 ; CntSave < MAX_RANKING ; CntSave++ )
	{
		fprintf( fp , "%d" , g_RankingList[ CntSave ].score );
	}	//	end of for
#else	//	�o�C�i���f�[�^�ŃZ�[�u������

	fwrite( &g_RankingList[ 0 ] , sizeof( RANKING ) , MAX_RANKING , fp );

#endif
	fclose( fp );	//	�t�@�C�������

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void Load( void )
 ����:		
 �߂�l:	
 ����:		���[�h
-----------------------------------------------------------------------------*/
void Load( void )
{
	FILE *fp;	//	�t�@�C���|�C���^

	fp = fopen( FILENAME , " rb " );	//	�t�@�C�����J��
						
	//	�G���[�`�F�b�N
	if( fp == NULL )
	{
		MessageBox( NULL , "�t�@�C�����J���܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}	//	end of if

//	�e�L�X�g�f�[�^�o�͂�����
#ifdef ENABLE_PEOC_SAVE
	// �f�[�^��l�������[�h������ 
	for( int CntLoad = 0 ; CntLoad < MAX_RANKING ; CntLoad++ )
	{
		fscanf( fp , "%d" , &g_RankingList[ CntLoad ].score );
	}	//	end of func
#else	//	�o�C�i���f�[�^�o�͂�����

	fread( &g_RankingList[ 0 ] , sizeof( RANKING ) , MAX_RANKING , fp );

#endif
		
	fclose( fp );	//	�t�@�C�������
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void BubbleSort( void )
 ����:		
 �߂�l:	
 ����:		�o�u���\�[�g
-----------------------------------------------------------------------------*/
void BubbleSort( void )
{
	int Cnt1 , Cnt2;
	unsigned int MaxScore = 0;

	for( Cnt1 = 0; Cnt1 < MAX_RANKING - 1 ; Cnt1++ )
	{
		for( Cnt2 = MAX_RANKING - 1 ; Cnt2 > Cnt1 ; Cnt2-- )
		{
			if( g_RankingList[ Cnt2 ].score < g_RankingList[ Cnt2 - 1 ].score )
			{
				//	����ւ�����
				MaxScore = g_RankingList[ Cnt2 ].score;

				g_RankingList[ Cnt2 ].score = g_RankingList[ Cnt2 - 1 ].score;

				g_RankingList[ Cnt2 - 1 ].score = MaxScore;

			}	//	end of if
		}	//	end of for
	}	//	end of for
}	//	end of func