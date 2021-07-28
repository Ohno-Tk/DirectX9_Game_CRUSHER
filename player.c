/*=============================================================================

		�v���C���[[ player.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/28
-------------------------------------------------------------------------------
	���@Update
		2016/07/28
			�E�w�b�_�t�@�C���ǉ�
				#include <stdio.h>
				#include "main.h"
				#include "player.h"
				#include "bg.h"
			�E�萔��`�ǉ�
				#define PLAYER_TEXTURENAME
			�E�O���[�o���ϐ��ǉ�
				LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
				LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^
				PLAYER g_Player;
			�E�v���g�^�C�v�錾�ǉ�
				HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
				void MakePlayerPos( VERTEX_2D* pVtx );	//	���W�ύX
		2016/08/22
			�E�w�b�_�t�@�C���ǉ�
				#include "base.h"
		2016/08/28
			�E�w�b�_�t�@�C���ǉ�
				#include "score.h"
		2016/09/07
			�E�v���g�^�C�v�錾�ǉ�
				void PlayerOperation( void );	//	�v���C���[�̊�{����
				void GoolPlayerOperation( void );	//	�S�[���������̃v���C���[�̏���
				void GoolDecision( void );	//	�S�[������
				void ScrollObject( void );	//	�v���C���[�ړ����̑���ƓG�̃X�N���[��
		2016/09/14
			�E�v���g�^�C�v�錾�ǉ�
					//	�X�N���[������I�u�W�F�N�g�̑����ύX
				void AttackDecision( void );	//	�U������
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "usability.h"
#include "sound.h"
#include "base.h"
#include "score.h"
#include "time.h"
#include "effect.h"
#include "particle.h"
#include "tutorial.h"
#include "guid.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define PLAYER_TEXTURENAME "data/TEXTURE/game/player2.png"

#define	JUMP_VALUE ( 20.0f )		// �W�����v��

#define POS_HAMMER ( 20.0f )	//	�n���}�[�̍��W

#define UP_PLAYER_CNT ( 50 )

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//	�e�N�X�`���C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^

PLAYER g_Player;

//	�e�N�X�`���؂�ւ�
float fUesult = 0 , fVesult = 0;

int g_CntUp = 0;
int g_CntPlayerFrame = 0;

bool g_GoolMove = false;
bool g_HitUp = false;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice );	//	���_�̍쐬
void MakePlayerPos( VERTEX_2D* pVtx );					//	���W�ύX
void PlayerOperation( void );							//	�v���C���[�̊�{����
void KeyboardOperation( void );							//	�L�[�{�[�h�̊�{����
void GamePadOperation( void );							//	�Q�[���p�b�h�̊�{����
void GoolPlayerOperation( void );						//	�S�[���������̃v���C���[�̏���
void GoolDecision( void );								//	�S�[������
void GoolHeightScore( void );							//	�S�[���̍����ɂ���ăX�R�A�̕ω�
void GoolModeGame( void );								//	�Q�[�����[�h�̎��̃v���C���[����
void GoolModeTutorial( void );							//	�`���[�g���A�����[�h�̎��̃v���C���[����
void ScrollObject( void );								//	�v���C���[�ړ����̑���ƓG�̃X�N���[��
void ScrollMoveAlterObject( void );						//	�X�N���[������I�u�W�F�N�g�̑����ύX
void ScrollMoveRepositObject( void );					//	�X�N���[������I�u�W�F�N�g�̑��������ɖ߂�
void AttackDecision( void );							//	�U������
void EnemyDecision( void );								//	�G�Ƃ̓����蔻��
void HitOperation( void );								//	�G�ƃq�b�g�������̍s��

/*-----------------------------------------------------------------------------
 �֐���:	void InitPlayer(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̏�����
-----------------------------------------------------------------------------*/
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	������̎擾
	BASE *Base = GetBase( 0 );

	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PLAYER_TEXTURENAME , &g_pTexturePlayer  ) ) )
	{
		MessageBox( NULL , "�v���C���[�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
	}

	//	�v���C���[�̏�����
	g_Player.Pos.x = 100.0f;
	g_Player.Pos.y = Base -> pos . y;
	g_Player.OldPos.x = 0.0f;
	g_Player.OldPos.y = 0.0f;
	g_Player.Size.x = 100.0f;
	g_Player.Size.y = 100.0f;
	g_Player.Move.x = 2.0f;
	g_Player.Move.y = 10.0f;
	g_Player.Direct = 1.0f;
	g_Player.HitMove_Y = 5.0f;
	g_Player.gool = false;
	g_Player.Hit = false;
	g_Player.Jump = false;
	g_Player.fall = false;
	g_Player.bUse = true;

	g_GoolMove = false;
	g_HitUp = false;

	fUesult = 0.0f;
	fVesult = 0.0f;
	
	g_CntUp = 0;
	g_CntPlayerFrame = 0;

	MakeVertexPolygon( pDevice );	//	���_�̍쐬

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitPlayer(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̏I��
-----------------------------------------------------------------------------*/
void UninitPlayer(void)
{
	fUesult = 0.0f;
	fVesult = 0.0f;

	if( g_pTexturePlayer != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTexturePlayer -> Release();
		g_pTexturePlayer = NULL;
	}	//	end of if
	if(g_pVtxBufferPlayer != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferPlayer -> Release();
		g_pVtxBufferPlayer = NULL;
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdatePlayer(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̍X�V
-----------------------------------------------------------------------------*/
void UpdatePlayer(void)
{
	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	VERTEX_2D* pVtx;

	//	�O�̍��W�̎擾
	g_Player.OldPos.x = g_Player.Pos.x;
	g_Player.OldPos.y = g_Player.Pos.y;

	//�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferPlayer -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	�v���C���[���S�[�����Ă��Ȃ�������
	if( Base -> GoolFalg == false )
	{
		PlayerOperation();	//	�v���C���[�̊�{����
	}
	//	�S�[�����Ă�����
	else if( Base -> GoolFalg == true )
	{
		GoolPlayerOperation();	//	�S�[���������̃v���C���[�̏���
	}

	MakePlayerPos( pVtx );	//	�v���C���[�̍��W�ύX

	g_pVtxBufferPlayer -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawPlayer(void)
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̕`��
-----------------------------------------------------------------------------*/
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	�X�g���[����ݒ肷��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferPlayer , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTexturePlayer );

	if( g_Player.bUse == true )
	{
		//	�|���S���̕`��
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice )
 ����:		�Ȃ�
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferPlayer , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// �\���̂̃|�C���^�錾
	VERTEX_2D* pVtx;


	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferPlayer -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );

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


	g_pVtxBufferPlayer -> Unlock();

	return S_OK;
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void MakePlayerPos( VERTEX_2D* pVtx )
 ����:		VERTEX_2D* pVtx
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̍��W�ύX
-----------------------------------------------------------------------------*/
void MakePlayerPos( VERTEX_2D* pVtx )
{
	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );

	//	�v���C���[�̌����ɂ���ăe�N�X�`���̌�����ς���
	if( g_Player.Direct == 1.0f )
	{
		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( fUesult             , 0 + fVesult );
		pVtx[ 1 ].tex = D3DXVECTOR2( TEX_WIDTH + fUesult , 0 + fVesult );
		pVtx[ 2 ].tex = D3DXVECTOR2( fUesult             , TEX_HEIGHT + fVesult );
		pVtx[ 3 ].tex = D3DXVECTOR2( TEX_WIDTH + fUesult , TEX_HEIGHT + fVesult );
	}
	else if( g_Player.Direct == -1.0f )
	{
		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( TEX_WIDTH+fUesult , 0 + fVesult );
		pVtx[ 1 ].tex = D3DXVECTOR2( fUesult           , 0 + fVesult );
		pVtx[ 2 ].tex = D3DXVECTOR2( TEX_WIDTH+fUesult , TEX_HEIGHT + fVesult );
		pVtx[ 3 ].tex = D3DXVECTOR2( fUesult           , TEX_HEIGHT + fVesult );
	}
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void PlayerOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�v���C���[�̊�{����
-----------------------------------------------------------------------------*/
void PlayerOperation( void )
{
	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	if( g_Player.Hit == true )
	{
		//	�G�ƃq�b�g�������̍s��
		HitOperation();
	}
	else if( g_Player.Hit == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )

		//	�Q�[���p�b�h������������
		if( GamePad[ CntPad ] == NULL )
		{
			//	�L�[�{�[�h�̊�{����
			KeyboardOperation();
		}
		//	�Q�[���p�b�h����������
		else if( GamePad[ CntPad ] != NULL )
		{
			//	�Q�[���p�b�h�f�o�C�X�擾
			GamePadOperation();
		}

		//	�G�Ƃ̓����蔻��
		EnemyDecision();
	
		g_Player.Move.y += GRAVITY;	//	�d�͂�������
		g_Player.Pos.y += g_Player.Move.y;

		//	�S�[������
		GoolDecision();

		//	�W�����v�����Ȃ�����
		if( (g_Player.Pos.y + g_Player.Size.y) > SCREEN_HEIGHT - 25.0f )
		{
			g_Player.Jump = true;
			g_Player.fall = true;
		}

		//	��ʂ�������
		if( g_Player.Pos.y > SCREEN_HEIGHT + 200.0f )
		{
			g_Player.bUse = false;
		}	//	end of if
	}	//	end of else if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void KeyboardOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�L�[�{�[�h�̊�{����
-----------------------------------------------------------------------------*/
void KeyboardOperation( void )
{

	//	�U��
	if( GetKeyboardTrigger( DIK_SPACE ) )
	{
		fUesult = 0.5f;

		//	�U������
		AttackDecision();

		//	SE�Đ�
		//	�U����

	}	//	end of if
		
	//	�U���{�^���𗣂�����
	else if( GetKeyboardRelease( DIK_SPACE ) )
	{
		fUesult = 0;
	}	//	end of if
		
	//	�_�b�V��
	if( GetKeyboardPress( DIK_LSHIFT ) )
	{
		g_Player.Move.x = 4.0f;

		//	�X�N���[������I�u�W�F�N�g�̑����ύX
		ScrollMoveAlterObject();
	}	//	end of if
		
	//	�U���{�^������������
	else if( GetKeyboardRelease( DIK_LSHIFT ) )
	{
		g_Player.Move.x = 2.0f;

		//	�X�N���[������I�u�W�F�N�g�̑��������ɖ߂�
		ScrollMoveRepositObject();
	} // end of else if
		
	//	�W�����v
	if ( GetKeyboardPress( DIK_W ) || GetKeyboardPress(DIK_UPARROW) )
	{
		if( g_Player.Jump == false )
		{
			g_Player.Move.y = -JUMP_VALUE;
			g_Player.Jump = true;
		}	//	end of if
	}	//	end of if
		
	//	�ړ�
	if ( GetKeyboardPress( DIK_A ) || GetKeyboardPress(DIK_LEFTARROW) )	//	���ړ�
	{
		g_Player.Direct = -1.0f;	//	�����̕ύX

		//	��ʂ�������
		if( g_Player.Pos.x < 0 )
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of if
		else
		{
			g_Player.Pos.x -= g_Player.Move.x;
		}	//	end of else
	}	//	end of if
		
	if ( GetKeyboardPress( DIK_D ) || GetKeyboardPress(DIK_RIGHTARROW) )	//	�E�ړ�
	{
		//	�w�i�A�G�A������X�N���[��������
		if( g_Player.Pos.x+g_Player.Size.x > SCREEN_WIDTH*0.5f )
		{
			ScrollObject();
		}	//	end of if
		else
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of else

		g_Player.Direct = 1.0f;	//	�����̕ύX

	}	//	end of if
}

/*-----------------------------------------------------------------------------
 �֐���:	void GamePadOperation( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�Q�[���p�b�h�̊�{����
-----------------------------------------------------------------------------*/
void GamePadOperation( void )
{
	//	�X�e�B�b�N�̎擾
	LONG Stick_X = GetGamePadStick_X();

	//	�U��
	//if( GetGamePadTrigger( BUTTOM_01 ) )
	if( GetGamePadTrigger( BUTTOM_06 ) )
	{
		fUesult = 0.5f;

		//	�U������
		AttackDecision();

		//	SE�Đ�
		//	�U����

	}	//	end of if
		
	//	�U���{�^���𗣂�����
	//else if( GetGamePadRelease( BUTTOM_01 ) )
	else if( GetGamePadRelease( BUTTOM_06 ) )
	{
		fUesult = 0;
	}	//	end of if
		
	//	�_�b�V��
	if( GetGamePadPress( BUTTOM_03 ) )
	{
		g_Player.Move.x = 4.0f;

		//	�X�N���[������I�u�W�F�N�g�̑����ύX
		ScrollMoveAlterObject();
	}	//	end of if
		
	//	�U���{�^������������
	else if( GetGamePadRelease( BUTTOM_03 ) )
	{
		g_Player.Move.x = 2.0f;

		//	�X�N���[������I�u�W�F�N�g�̑��������ɖ߂�
		ScrollMoveRepositObject();
	} // end of else if
		
	//	�W�����v
	if ( GetGamePadPress( BUTTOM_04 ) )
	{
		if( g_Player.Jump == false )
		{
			g_Player.Move.y = -JUMP_VALUE;
			g_Player.Jump = true;
		}	//	end of if
	}	//	end of if
		
	//	�ړ�
	if ( Stick_X < 0 )	//	���ړ�
	{
		g_Player.Direct = -1.0f;	//	�����̕ύX

		//	��ʂ�������
		if( g_Player.Pos.x < 0 )
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of if
		else
		{
			g_Player.Pos.x -= g_Player.Move.x;
		}	//	end of else
	}	//	end of if
		
	if ( Stick_X > 0 )	//	�E�ړ�
	{
		//	�w�i�A�G�A������X�N���[��������
		if( g_Player.Pos.x+g_Player.Size.x > SCREEN_WIDTH*0.5f )
		{
			ScrollObject();
		}	//	end of if
		else
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of else

		g_Player.Direct = 1.0f;	//	�����̕ύX

	}	//	end of if
}

/*-----------------------------------------------------------------------------
 �֐���:	void GoolPlayerOperation( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�S�[���������̃v���C���[�̏���
-----------------------------------------------------------------------------*/
void GoolPlayerOperation( void )
{
	//	���̃��[�h
	MODE *Mode = GetMode();

	fUesult = 0.0f;
	fVesult = 0.0f;

	//	�Q�[�����[�h�̎�
	if( *Mode == MODE_GAME )
	{
		//	�Q�[�����[�h�̎��̃v���C���[����
		GoolModeGame();
	}	//	end of if

	//	�`���[�g���A�����[�h�̎�
	else if( *Mode == MODE_TUTORIAL )
	{
		//	�`���[�g���A�����[�h�̎��̃v���C���[����
		GoolModeTutorial();
	}	//	end of else if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void ScrollField( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�S�[������
-----------------------------------------------------------------------------*/
void GoolDecision( void )
{
	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( MAX_GOOL );

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	//	�S�[������
	if( RectangleDecision( g_Player.Pos.x , g_Player.Pos.y , g_Player.Size.x-POS_HAMMER , g_Player.Size.y , Base->pos.x+30.0f , Base->pos.y , Base->size.x-30.0f , Base->size.y ) )
	{
		Base -> GoolFalg = true;

		if( *Mode == MODE_GAME )
		{
			//	�S�[���̍����ɂ���ăX�R�A�̕ω�
			GoolHeightScore();
		}
		PlaySound( SOUND_LABEL_SE_GOOL );
	} // end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void GoolHeightScore( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�S�[���̍����ɂ���ăX�R�A�̕ω�
-----------------------------------------------------------------------------*/
void GoolHeightScore( void )
{
	//	�X�R�A�̏��̎擾
	int *Score = GetScore();

	if( 100.0f > g_Player.Pos.y )
	{
		*Score += 5000;
	}

	else if( 200.0f < g_Player.Pos.y && g_Player.Pos.y < 400.0f )
	{
		*Score += 1500;
	}

	else
	{
		*Score += 500;
	}
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void GoolModeGame( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�Q�[�����[�h�̎��̃v���C���[����
-----------------------------------------------------------------------------*/
void GoolModeGame( void )
{
	//	�X�R�A���̎擾
	int *Score = GetScore();
	bool *ScoreFlag = GetScoreFlag();

	//	�^�C�����̎擾
	int *Time = GetTime();

	if( g_GoolMove == false )
	{
		g_Player.Pos.y += 2;
		//	�S�[���̉��܂ōs������
		if( g_Player.Pos.y + g_Player.Size.y > SCREEN_HEIGHT - 40.0f )
		{
			g_GoolMove = true;

		}	//	end of if
	}	//	end of if

	if( g_GoolMove == true )
	{
		g_Player.Pos.x += g_Player.Move.x;
		//	��ʔ͈͊O�ɍs������
		if( g_Player.Pos.x > SCREEN_WIDTH && *Time <= 0 )
		{
			g_Player.gool = true;
		}	//	end of if
	}	//	end of if

	*Time -= 75;
	if( *ScoreFlag == false )
	{
		*Score += 50;
	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void GoolModeTutorial( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�`���[�g���A�����[�h�̎��̃v���C���[����
-----------------------------------------------------------------------------*/
void GoolModeTutorial( void )
{
	if( g_GoolMove == false )
	{
		g_Player.Pos.y += 2;
		//	�S�[���̉��܂ōs������
		if( g_Player.Pos.y + g_Player.Size.y > SCREEN_HEIGHT - 40.0f )
		{
			g_GoolMove = true;
		}	//	end of if
	}	//	end of if

	if( g_GoolMove == true )
	{
		g_Player.Pos.x += g_Player.Move.x;
		//	��ʔ͈͊O�ɍs������
		if( g_Player.Pos.x > SCREEN_WIDTH )
		{
			g_Player.gool = true;
		}	//	end of if
	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void ScrollObject( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�v���C���[�ړ����̑���ƓG�̃X�N���[��
-----------------------------------------------------------------------------*/
void ScrollObject( void )
{
	int CntEnemy;	//	�G��for�Ő�����
	int CntBase;	//	�����for�Ő�����
	int CntTutorial;

	//	�w�i���̎擾
	float *pBgTex = GetBgTex();
	float *pBgVTex = GetVTex();

	//	�G���̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	������̎擾
	BASE *Base = GetBase( 0 );

	//	�`���[�g���A�����̎擾
	TUTORIAL *Tutorial = GetTutorial();

	//	�ē����̎擾
	GUIDSIGNBOARD *Guid = GetGuid();

	//	�w�i�̃X�N���[��
	*pBgTex += BG_TEX_SPEED;


	if( *pBgTex >= 0.999f && *pBgTex <= 1.0f )
	{
		*pBgVTex += 0.1f;
		*pBgTex = 0.0f;
	} // end of if

	//	����̃X�N���[��	
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ , Base++ )
	{
		if( Base -> use == true )
		{
			Base -> pos . x -= Base ->move_X;
				
			if( Base -> pos . x + Base -> size . x < 0 )
			{
				Base -> use = false;
			}	//	end of if
		}	//	end of if
	}	//	end of for
				
	//	�G�̃X�N���[��	
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		if( Enemy -> Use == true )
		{
			Enemy -> Pos.x -= Enemy -> Scroll_Move_X;
		}	//	end of if
	}	//	end of for

	//	�`���[�g���A���̃X�N���[��
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ , Tutorial++ )
	{
		Tutorial -> pos.x -= Tutorial -> Move_X;
	}

	//	�ē��̃X�N���[��
	Guid -> pos.x -= Guid -> move_X;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void ScrollMoveAlterObject( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�X�N���[������I�u�W�F�N�g�̑����ύX
-----------------------------------------------------------------------------*/
void ScrollMoveAlterObject( void )
{
	int CntEnemy;	//	�G��for�Ő�����
	int CntBase;	//	�����for�Ő�����
	int CntTutorial;

	//	�`���[�g���A�����̎擾
	TUTORIAL *Tutorial = GetTutorial();

	//	�G���̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( 0 );

	//	�ē����̎擾
	GUIDSIGNBOARD *Guid = GetGuid();

	//	����
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ , Base++ )
	{
		Base->move_X = MOVE_AFTER;
	}	//	end of for

	//	�G
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		Enemy -> Scroll_Move_X = MOVE_AFTER;
	}	//	end of for

	//	�`���[�g���A��
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ , Tutorial++ )
	{
		Tutorial -> Move_X = MOVE_AFTER;
	}	//	end of for

	//	�ē��̃X�N���[��
	Guid -> move_X = MOVE_AFTER;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void ScrollMoveRepositObject( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�X�N���[������I�u�W�F�N�g�̑��������ɖ߂�
-----------------------------------------------------------------------------*/
void ScrollMoveRepositObject( void )
{
	int CntEnemy;	//	�G��for�Ő�����
	int CntBase;	//	�����for�Ő�����
	int CntTutorial;

	//	�`���[�g���A�����̎擾
	TUTORIAL *Tutorial = GetTutorial();

	//	�G���̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	������(�S�[��)�̎擾
	BASE *Base = GetBase( 0 );

	//	�ē����̎擾
	GUIDSIGNBOARD *Guid = GetGuid();

	//	����
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ , Base++ )
	{
		Base->move_X = MOVE_REPOSIT;
	}	//	end of for

	//	�G
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		Enemy -> Scroll_Move_X = MOVE_REPOSIT;
	}	//	end of for

	//	�`���[�g���A��
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ , Tutorial++ )
	{
		Tutorial -> Move_X = MOVE_REPOSIT;
	}	//	end of for

	//	�ē�
	Guid -> move_X = MOVE_REPOSIT;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void AttackDecision( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�U������
-----------------------------------------------------------------------------*/
void AttackDecision( void )
{
	int CntEnemy;	//	�G��for�Ő�����

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	�G���̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	�X�R�A�̏��̎擾
	int *Score = GetScore();

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();

	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		if( Enemy ->Use == true )
		{
			//	�����ɂ���Ĕ����ς���

			//	�E����
			if( g_Player.Direct == 1.0f )
			{
				if( RectangleDecision( ( g_Player.Pos.x + g_Player.Size.y ) - POS_HAMMER , g_Player.Pos.y , POS_HAMMER + 15.0f , g_Player.Size.y , Enemy->Pos.x , Enemy->Pos.y , Enemy->Size.x , Enemy->Size.y ) )
				{
					Enemy ->Use = false;

					if( *Mode == MODE_GAME )
					{
						//	�X�R�A�̉��Z
						*Score += Enemy->Score;
					}

					if( GamePad[ 0 ] != NULL )
					{
						//	�o�C�u���[�V�����̊J�n
						VibrationStart( 1 , 0 );
					}

					//	�G�t�F�N�g�̃Z�b�g
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_HIT );
				}
			}
			//	������
			else if( g_Player.Direct == -1.0f )
			{
				if( RectangleDecision( g_Player.Pos.x - 15.0f, g_Player.Pos.y , POS_HAMMER , g_Player.Size.y , Enemy->Pos.x , Enemy->Pos.y , Enemy->Size.x , Enemy->Size.y ) )
				{
					Enemy ->Use = false;

					if( *Mode == MODE_GAME )
					{
						//	�X�R�A�̉��Z
						*Score += Enemy->Score;
					}

					if( GamePad[ 0 ] != NULL )
					{
						//	�o�C�u���[�V�����̊J�n
						VibrationStart( 1 , 0 );
					}

					//	�G�t�F�N�g�̃Z�b�g
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );

					//	SE�Đ�
					PlaySound( SOUND_LABEL_SE_HIT );
				}	//	end of if
			}	//	end of else if
		}	//	end of if
	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void EnemyDecision( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�G�Ƃ̓����蔻��
-----------------------------------------------------------------------------*/
void EnemyDecision( void )
{
	int CntEnemy;	//	�G��for�Ő�����

	//	�G���̎擾
	ENEMY *Enemy = GetEnemy( 0 );

	//	�Q�[���p�b�h�f�o�C�X�擾
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );


	//	�G�̓����蔻��
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		if( RectangleDecision( g_Player.Pos.x , g_Player.Pos.y , g_Player.Size.x-POS_HAMMER , g_Player.Size.y , Enemy->Pos.x + 40.0f , Enemy->Pos.y + 30.0f , Enemy->Size.x - 80.0f , Enemy->Size.y - 40.0f ) && Enemy -> Use == true )
		{
			g_Player.Hit = true;

			if( GamePad[ 0 ] != NULL )
			{
				//	�o�C�u���[�V�����̊J�n
				VibrationStart( 1 , 0 );
			}

		}	//	end of if
	}	//	end of for
}	//	end of for

/*-----------------------------------------------------------------------------
 �֐���:	void HitOperation( void )
 ����:		�Ȃ�
 �߂�l:	
 ����:		�G�ƃq�b�g�������̍s��
-----------------------------------------------------------------------------*/
void HitOperation( void )
{

	//	�e�N�X�`���ύX
	fUesult = 0.0f;
	fVesult = 0.5f;

	g_Player.Pos.y -= g_Player.HitMove_Y;

	g_CntUp++;

	if( g_HitUp == false )
	{
		if( g_CntUp % UP_PLAYER_CNT == 0 )
		{
			g_Player.HitMove_Y *= -1.7f;
			g_HitUp = true;
		}
	}

	//	��ʊO����
	if( g_Player.Pos.y > SCREEN_HEIGHT )
	{
		g_Player.bUse = false;
	}	//	end of if
}

/*-----------------------------------------------------------------------------
 �֐���:	PLAYER *GetPlayer(void)
 ����:		�Ȃ�
 �߂�l:	return &g_Player;
 ����:		�v���C���[���̎擾
-----------------------------------------------------------------------------*/
PLAYER *GetPlayer(void)
{
	return &g_Player;
}	//	end of func
