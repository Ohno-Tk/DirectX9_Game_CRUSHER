#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define MAX_PAD ( 1 )	//	�Q�[���p�b�h�̍ő吔

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef enum
{
	BUTTOM_01 = 0,
	BUTTOM_02,
	BUTTOM_03,
	BUTTOM_04,
	BUTTOM_05,
	BUTTOM_06,
	BUTTOM_07,
	BUTTOM_08,
	BUTTOM_09,
	BUTTOM_10,
	BUTTOM_11,
	BUTTOM_12,
	MAX_BUTTOM,
}PAD_BUTTOM;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
HRESULT InitGamePad( HINSTANCE hInstance , HWND hWnd );	//	�Q�[���p�b�h�̏�����
void UninitGamePad( void );	//	�Q�[���p�b�h�I��
void UpdateGamePad(void);	//	�Q�[���p�b�h�X�V

LPDIRECTINPUTDEVICE8 *GetGamePad( int IndexPad );	//	�Q�[���p�b�h�f�o�C�X�擾

bool GetGamePadPress( int nKey );	//	�Q�[���p�b�h�̃v���X��Ԃ��擾
bool GetGamePadTrigger(int nKey);	//	�Q�[���p�b�h�̃g���K�[��Ԃ��擾(�������u��)
bool GetGamePadRepeat(int nKey);	//	�Q�[���p�b�h�̃��s�[�g��Ԃ��擾
bool GetGamePadRelease(int nKey);	//	�Q�[���p�b�h�̃����|�X��Ԃ��擾(�������u��)

LONG GetGamePadStick_X( void );		//	�X�e�B�b�NX���W�̎擾
LONG GetGamePadStick_Y( void );		//	�X�e�B�b�NY���W�̎擾

void VibrationStart( int Number , int PadNumber );	//	�o�C�u���[�V�����̊J�n
void VibrationStop( int PadNumber );			//	�o�C�u���[�V�����̒�~
#endif