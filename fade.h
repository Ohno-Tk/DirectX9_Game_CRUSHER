#ifndef _FADE_H_
#define _FADE_H_
/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef enum
{
	FADE_NONE = 0,
	FADE_IN ,
	FADE_OUT ,
	FADE_MAX
}FADE;
/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
void InitFade( void );	//	�t�F�[�h�̏�����
void UninitFade( void );	//	�t�F�[�h�̏I��
void UpdateFade( void );	//	�t�F�[�h�̍X�V
void DrawFade( void );	//	�t�F�[�h�̕`��
void SetFade( FADE fade , MODE modenext );	//	�t�F�[�h��Ԑݒ�
MODE *GetNextMode( void );	//	���̃��[�h�̒ǉ�
bool *GetFadeUse( void );	//	�t�F�[�h�����ǂ���
#endif