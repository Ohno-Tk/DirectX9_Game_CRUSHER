#ifndef _PAUSE_H_
#define _PAUSE_H_
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
void InitPause( void );	//	�|�[�Y�̏�����
void UninitPause( void );	//	�|�[�Y�̏I��
void UpdatePause( void );	//	�|�[�Y�̍X�V
void DrawPause( void );	//	�|�[�Y�̕`��
bool *GetPause( void );	//	�|�[�Y���Ă��邩
bool *GetOpeFlag( void );
#endif