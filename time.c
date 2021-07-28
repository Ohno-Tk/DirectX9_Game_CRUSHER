/*=============================================================================

		�^�C��[ time.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/04
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "time.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define TIME_TEXTURENAME "data/TEXTURE/game/number01.png"

#define TIME_DIGIT (4)	//	�����̐����̕\��
#define TIME_MIN (0)	//	�^�C���̍ŏ��l
#define SECOND ( 401 )	//	�b��
#define FRAME (60)	//	�t���[����


#define TIME_X ( SCREEN_WIDTH - 75.0f )	//	�^�C����X���W
#define TIME_Y (50.0f)	//	�^�C����Y���W
#define TIME_WIDTH (50.0f)	//	�^�C����Width
#define TIME_HEIGHT (50.0f)	//	�^�C����Height

#define TIME_ASCII_X ( 750.0f )	//	�^�C��������X���W
#define TIME_ASCII_Y ( 0.0f )	//	�^�C��������Y���W
#define TIME_ASCII_WIDTH ( 200.0f )	//	�^�C��������Width
#define TIME_ASCII_HEIGHT ( 80.0f )	//	�^�C��������Height
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void MakeTime( VERTEX_2D* pVtx );	//	�^�C���̃e�N�X�`���X�V
/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

int g_Time;
/*-----------------------------------------------------------------------------
 �֐���:	void InitTime( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C���̏�����
-----------------------------------------------------------------------------*/
void InitTime( void )
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MakeVertexTime( pDevice );	//	���_�̍쐬

	//	���Ԃ̏���������
	g_Time = SECOND*FRAME;

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , TIME_TEXTURENAME , &g_pTextureTime  ) ) )
	{
		MessageBox( NULL , "�^�C���̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitTime( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C���̏I��
-----------------------------------------------------------------------------*/
void UninitTime( void )
{
	if( g_pTextureTime != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureTime -> Release();
		g_pTextureTime = NULL;
	}
	if(g_pVtxBufferTime != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferTime -> Release();
		g_pVtxBufferTime  = NULL;
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdeteTime( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C���̍X�V
-----------------------------------------------------------------------------*/
void UpdateTime( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTime -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	�^�C����0�ȉ��ɂȂ�����
	if( g_Time <= 0 )
	{
		g_Time = TIME_MIN;
	}
	else
	{
		g_Time--;	//	�^�C�������炷
	}

	MakeTime(pVtx);
	g_pVtxBufferTime -> Unlock();
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawTime( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�^�C���̕`��
-----------------------------------------------------------------------------*/
void DrawTime( void )
{
	int loop;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTime , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureTime );

	for(loop = 0;loop < TIME_DIGIT;loop++)
	{
		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , loop*4, NUM_POLYGON);
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice
 �߂�l:	�Ȃ�
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
{
	int i;
	int num;	//	������\������ϐ�
	int value = g_Time/60;	//	�^�C���̒l
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * TIME_DIGIT , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTime , NULL ) ) )
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;

	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferTime -> Lock ( 0 ,
		                          0 ,
								  ( void** )&pVtx ,      // ���z(�^��)�A�h���X�擾
								  0 );


	for(i = 0;i < TIME_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	���_���W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );

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

		//	�^�C������
		if(i == TIME_DIGIT-1)
		{
			//	���_���W�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );

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
			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.3f , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.3f , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
		}

	}

	g_pVtxBufferTime -> Unlock();

	return S_OK;

}
/*-----------------------------------------------------------------------------
 �֐���:	void MakeTime( void )
 ����:		
 �߂�l:	�Ȃ�
 ����:		�^�C���̃e�N�X�`���X�V
-----------------------------------------------------------------------------*/
void MakeTime( VERTEX_2D* pVtx )
{
	int i;
	int num;	//	������\������ϐ�
	int value = g_Time/60;	//	�^�C���̒l

	for(i = 0;i < TIME_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	���_���W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.1f*num , 0 );
		pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0 );
		pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.1f*num , 0.5f );
		pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0.5f );

		//	�^�C������
		if(i == TIME_DIGIT-1)
		{
			//	���_���W�̐ݒ�
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );

			//	�e�N�X�`�����W�̐ݒ�
			//pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.3f , 0.5f );
			//pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			//pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.3f , 1.0f );
			//pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );

			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1.0f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1.0f , 1.0f );
		}
	}

}
/*-----------------------------------------------------------------------------
 �֐���:	int GetTime( void )
 ����:		
 �߂�l:	return g_Time;
 ����:		�^�C���̏��̎擾
-----------------------------------------------------------------------------*/
int *GetTime( void )
{
	return &g_Time;
}