/*=============================================================================

		�Q�[�����e[ game.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/28
-------------------------------------------------------------------------------
	���@Update
		2016/07/28
			�E�w�b�_�t�@�C���ǉ�
				#include <stdio.h>
				#include "game.h"
				#include "input.h"
				#include "fade.h"
				#include "player.h"
				#include "bg.h"
			�EInitGame�ǉ�
				InitBg();	//	�w�i�̏�����
				InitPlayer();	//	�v���C���[�̏�����
			�EUninitGame�ǉ�
				UninitBg();	//	�w�i�̏I��
				UninitPlayer();	//	�v���C���[�̏I��
			�EUpdateGame�ǉ�
				UpdateBg();	//	�w�i�̍X�V
				UpdatePlayer();	//	�v���C���[�̍X�V
			�EDrawGame�ǉ�
				DrawBg();	//	�w�i�̕`��
				DrawPlayer();	//	�v���C���[�̕`��
		2016/08/02
			�E�w�b�_�t�@�C���ǉ�
				#include "time.h"
				#include "score.h"
			�EInitGame�ǉ�
				InitScore();	//	�X�R�A�̏�����
				InitTime();	//	�^�C���̏�����
			�EUninitGame�ǉ�
				UninitScore();	//	�X�R�A�̏I��
				UninitTime();	//	�^�C���̏I��
			�EUpdateGame�ǉ�
				UpdateScore();	//	�X�R�A�̍X�V
				UpdateTime();	//	�^�C���̍X�V
			�EDrawGame�ǉ�
				DrawScore();	//	�X�R�A�̕`��
				DrawTime();	//	�^�C���̕`��
		2016/08/08
			�E�w�b�_�t�@�C���ǉ�
				#include "enemy.h"
			�EInitGame�ǉ�
				InitEnemy();	//	�G�̏�����
			�EUninitGame�ǉ�
				UninitEnemy();	//	�G�̏I��
			�EUpdateGame�ǉ�
				UpdateEnemy();	//	�G�̍X�V
			�EDrawGame�ǉ�
				DrawEnemy();	//	�G�̕`��
		2016/08/22
			�E�w�b�_�t�@�C���ǉ�
				##include "base.h"
			�EInitGame�ǉ�
				InitBase();	//	����̏�����
			�EUninitGame�ǉ�
				UninitBase();	//	����̏I��
			�EUpdateGame�ǉ�
				UpdateBase();	//	����̍X�V
			�EDrawGame�ǉ�
				DrawBase();		//	����̕`��
		2016/08/28
			�E�w�b�_�t�@�C���ǉ�
				#include "sound.h"
		2016/09/07
			�E�w�b�_�t�@�C���ǉ�
				#include "effect.h"
			�EInitGame�ǉ�
				InitEffect();	//	�G�t�F�N�g�̏�����
			�EUninitGame�ǉ�
				UninitEffect();	//	�G�t�F�N�g�̏I��
			�EUpdateGame�ǉ�
				UpdateEffect();	//	�G�t�F�N�g�̍X�V
			�EDrawGame�ǉ�
				DrawEffect();	//	�G�t�F�N�g�̕`��
		2016/09/11
			�E�w�b�_�t�@�C���ǉ�
				#include "particle.h"
			�EInitGame�ǉ�
				Initparticle();	//	�p�[�e�B�N���̏�����
			�EUninitGame�ǉ�
				Uninitparticle();	//	�p�[�e�B�N���̏I��
			�EUpdateGame�ǉ�
				Updateparticle();	//	�p�[�e�B�N���̍X�V
			�EDrawGame�ǉ�
				Drawparticle();	//	�p�[�e�B�N���̕`��
		2016/09/14
			�E�w�b�_�t�@�C���ǉ�
				#include "cloud.h"
			�EInitGame�ǉ�
				InitCloud();	//	�_�̏�����
			�EUninitGame�ǉ�
				UninitCloud();	//	�_�̏I��
			�EUpdateGame�ǉ�
				UpdateCloud();	//	�_�̍X�V
			�EDrawGame�ǉ�
				DrawCloud();	//	�_�̕`��
		2016/09/19
			�E�w�b�_�t�@�C���ǉ�
				#include "pause.h"
			�EInitGame�ǉ�
				InitPause();	//	�|�[�Y�̏�����
			�EUninitGame�ǉ�
				UninitPause();	//	�|�[�Y�̏I��
			�EUpdateGame�ǉ�
				UpdatePause();	//	�|�[�Y�̍X�V
			�EDrawGame�ǉ�
				DrawPause();	//	�|�[�Y�̕`��
		2016/09/27
			�E�w�b�_�t�@�C���ǉ�
				#include "guid.h"
			�EInitGame�ǉ�
				InitGuid();	//	�ē��̏�����
			�EUninitGame�ǉ�
				UninitGuid();	//	�ē��̏I��
			�EUpdateGame�ǉ�
				UpdateGuid();	//	�ē��̍X�V
			�EDrawGame�ǉ�
				DrawGuid();	//	�ē��̕`��
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "time.h"
#include "score.h"
#include "base.h"
#include "effect.h"
#include "particle.h"
#include "cloud.h"
#include "pause.h"
#include "operation.h"
#include "guid.h"
/*-----------------------------------------------------------------------------
 �֐���:	void InitGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�Q�[���̏�����
-----------------------------------------------------------------------------*/
void InitGame( void )
{
	//	���̃��[�h
	MODE *Mode = GetMode();

	*Mode = MODE_GAME;

	InitBg();		//	�w�i�̏�����

	InitCloud();	//	�_�̏�����

	InitBase();		//	����̏�����

	InitGuid();	//	�ē��̏�����

	InitEnemy();	//	�G�̏�����

	InitPlayer();	//	�v���C���[�̏�����

	Initparticle();	//	�p�[�e�B�N���̏�����

	InitEffect();	//	�G�t�F�N�g�̏�����

	InitScore();	//	�X�R�A�̏�����

	InitTime();		//	�^�C���̏�����

	InitPause();	//	�|�[�Y�̏�����

	InitOperation();	//	������@�̏�����

	//	���y�Đ�
	PlaySound( SOUND_LABEL_BGM_GAME );
}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UninitGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�Q�[���̏I��
-----------------------------------------------------------------------------*/
void UninitGame( void )
{
	UninitBg();		//	�w�i�̏I��

	UninitCloud();	//	�_�̏I��

	UninitBase();	//	����̏I��

	UninitGuid();	//	�ē��̏I��

	UninitEnemy();	//	�G�̏I��

	UninitPlayer();	//	�v���C���[�̏I��

	Uninitparticle();	//	�p�[�e�B�N���̏I��

	UninitEffect();	//	�G�t�F�N�g�̏I��

	UninitTime();	//	�^�C���̏I��

	UninitPause();	//	�|�[�Y�̏I��

	UninitOperation();	//	������@�̏I��


}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void UpdeteGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�Q�[���̍X�V
-----------------------------------------------------------------------------*/
void UpdateGame( void )
{
	//	�^�C�����̎擾
	int *Time = GetTime();

	//	�v���C���[���̎擾
	PLAYER *Player = GetPlayer();

	//	�|�[�Y���̎擾
	bool *Pause = GetPause();
	bool *PauseUse = GetOpeFlag();

	static bool BebugPause = false;

	if(GetKeyboardTrigger(DIK_O))
	{
		BebugPause ^= 1;
	}

	//	��ʑJ��
	if( Player->gool == true || Player -> bUse == false )
	{
		SetFade(FADE_OUT , MODE_RESULT);

		//	���y��~
		StopSound( SOUND_LABEL_BGM_GAME );
	}

	if( *Pause == false && BebugPause == false )
	{
		UpdateBg();		//	�w�i�̍X�V

		UpdateCloud();	//	�_�̍X�V

		UpdateBase();	//	����̍X�V

		UpdateGuid();	//	�ē��̍X�V

		UpdateEnemy();	//	�G�̍X�V

		UpdatePlayer();	//	�v���C���[�̍X�V

		Updateparticle();	//	�p�[�e�B�N���̍X�V

		UpdateEffect();	//	�G�t�F�N�g�̍X�V

		UpdateScore();	//	�X�R�A�̍X�V

		UpdateTime();	//	�^�C���̍X�V
	}	//	end of if

	if( *PauseUse == false )
	{
		UpdatePause();	//	�|�[�Y�̍X�V
	}
	else if( *PauseUse == true )
	{
		UpdateOperation();	//	������@�̍X�V
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 �֐���:	void DrawGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�Q�[���̕`��
-----------------------------------------------------------------------------*/
void DrawGame( void )
{
	//	�|�[�Y���̎擾
	bool *PauseUse = GetOpeFlag();

	DrawBg();		//	�w�i�̕`��

	DrawCloud();	//	�_�̕`��

	DrawBase();		//	����̕`��

	DrawGuid();	//	�ē��̕`��

	DrawEnemy();	//	�G�̕`��

	DrawPlayer();	//	�v���C���[�̕`��

	Drawparticle();	//	�p�[�e�B�N���̕`��

	DrawEffect();	//	�G�t�F�N�g�̕`��

	DrawScore();	//	�X�R�A�̕`��

	DrawTime();		//	�^�C���̕`��

	if( *PauseUse == false )
	{
		DrawPause();	//	�|�[�Y�̕`��
	}
	else if( *PauseUse == true )
	{
		DrawOperation();	//	������@�̕`��
	}

}	//	end of func