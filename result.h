#ifndef _RESULT_H_
#define _RESULT_H_
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define RESULT_PUSH_POS_X ( 550.0f )	//	�{�^����X
#define RESULT_PUSH_POS_Y ( 480.0f )	//	�{�^����Y
#define RESULT_PUSH_POS_WIDTH ( 400.0f )	//	�{�^����WIDTH
#define RESULT_PUSH_POS_HEIGHT ( 100.0f )	//	�{�^����HEIGHT
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
void InitResult( void );	//	���U���g�̏�����
void UninitResult( void );	//	���U���g�̏I��
void UpdateResult( void );	//	���U���g�̍X�V
void DrawResult( void );	//	���U���g�̕`��
#endif