/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "gamepad.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/


#define NUM_BUTTOM_MAX ( 32 )	//	�{�^���̍ő吔
#define	LIMIT_BUTTOM_COUNT_REPEAT	(20)	// ���s�[�g�J�E���g���~�b�^�[

#define DURATION ( 0.5 )			//	�p������

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECTINPUT8			g_pDInput = NULL;						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevGamePad[ MAX_PAD ] = { NULL };		// ���̓f�o�C�X(�Q�[���p�b�h)�ւ̃|�C���^	�����ɂ���ɂ͔z��ɂ���?
LPDIRECTINPUTEFFECT     g_pDIEffect[ MAX_PAD ] = { NULL };	//	�G�t�F�N�g�E�I�u�W�F�N�g

DIDEVCAPS g_diDevCaps;											//	�W���C�X�e�B�b�N�̔\�͏��


BYTE					g_aPadState[NUM_BUTTOM_MAX];			// �Q�[���p�b�h�̓��͏�񃏁[�N
BYTE					g_aPadStateTrigger[NUM_BUTTOM_MAX];		// �Q�[���p�b�h�̃g���K�[��񃏁[�N
BYTE					g_aPadStateRelease[NUM_BUTTOM_MAX];		// �Q�[���p�b�h�̃����[�X��񃏁[�N
BYTE					g_aPadStateRepeat[NUM_BUTTOM_MAX];		// �Q�[���p�b�h�̃��s�[�g��񃏁[�N
int						g_aPadStateRepeatCnt[NUM_BUTTOM_MAX];	// �Q�[���p�b�h�̃��s�[�g�J�E���^

LONG g_PadStick_X;	//	�X�e�B�b�N��X
LONG g_PadStick_Y;	//	�X�e�B�b�N��Y

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
BOOL CALLBACK EnumJoyCallback( const DIDEVICEINSTANCE *pdidInstance , void *pContext );	//	�W���C�X�e�B�b�N��񋓂���R�[���o�b�N�֐�

HRESULT InitGamePadInput( HINSTANCE hInstance, HWND hWnd );								//	���͏���������
void UninitGamePadInput( void );														//	���͏����I��

HRESULT DeviceCnfiguration( void );														//	�f�o�C�X�̐ݒ�
BOOL CALLBACK EnumAxesCallBack( LPCDIDEVICEOBJECTINSTANCE Ipddoi , LPVOID pvRef );		//	�����ƂɌĂяo�����R�[���o�b�N�֐�
HRESULT BufferSize( void );																//	�o�b�t�@�T�C�Y�̐ݒ�

HRESULT InitFeedBack( HWND hWnd );														//	�t�H�[�X�t�B�[�h�o�b�N�̏�����

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitInput( HINSTANCE hInstance, HWND hWnd )
 ����:		HINSTANCE hInstance
			HWND hWnd
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���͏���������
-----------------------------------------------------------------------------*/
HRESULT InitGamePadInput( HINSTANCE hInstance, HWND hWnd )
{
	//	�k���`�F�b�N
	if( g_pDInput == NULL )
	{
		//	DirectInput8�I�u�W�F�N�g�̍쐬
		if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8, (void**)&g_pDInput, NULL ) ) )
		{
			MessageBox( NULL , "DirectInput8�I�u�W�F�N�g�̍쐬�Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );
			return E_FAIL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 �֐���:	void UninitInput( void )
 ����:		
 �߂�l:	
 ����:		���͏����I��
-----------------------------------------------------------------------------*/
void UninitGamePadInput( void )
{
	// DirectInput�I�u�W�F�N�g�̊J��
	if( g_pDInput != NULL )
	{
		g_pDInput -> Release();
		g_pDInput = NULL;
	}
}

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitGamePad( HINSTANCE hInstance , HWND hWnd )
 ����:		HINSTANCE hInstance
			HWND hWnd
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		�Q�[���p�b�h�̏�����
-----------------------------------------------------------------------------*/
HRESULT InitGamePad( HINSTANCE hInstance , HWND hWnd )
{
	//	���͏���������
	if( FAILED( InitGamePadInput(hInstance , hWnd ) ) )
	{
		//	DirectInput8�I�u�W�F�N�g�쐬�Ɏ��s
		MessageBox( NULL , "DirectInput8�I�u�W�F�N�g���Ȃ�" , "�x��" , MB_OK | MB_ICONHAND );

		return E_FAIL;
	}

	//	�W���C�X�e�B�b�N��񋓂��č쐬
	if( FAILED( g_pDInput -> EnumDevices( DI8DEVCLASS_GAMECTRL , EnumJoyCallback , NULL , DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY ) ) )
	{
		//	DirectInputDevice8�I�u�W�F�N�g�̍쐬�Ɏ��s
		MessageBox( NULL , "DirectInputDevice8�I�u�W�F�N�g�̍쐬�Ɏ��s\n �t�H�[�X�t�B�[�h�o�b�N�Ή��̃W���C�X�e�B�b�N������܂���" , "�x��" , MB_OK | MB_ICONHAND );

		return E_FAIL;
	}

	DWORD rdwAxes[ 2 ] = { DIJOFS_X , DIJOFS_Y };
	LONG  rglDirection[ 2 ] = { 0 , 0 };

	DICONSTANTFORCE cf = { 0 };
	cf.lMagnitude = 10000;

	DIEFFECT eff;

	ZeroMemory( &eff , sizeof( eff ) );
	eff.dwSize                          = sizeof( DIEFFECT );
	eff.dwFlags                         = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;	//	�f�J���g���W
	eff.dwDuration                      = ( DWORD )( DI_SECONDS * DURATION );			//	1��̎�������
	eff.dwSamplePeriod                  = 0;
	eff.dwGain                          = DI_FFNOMINALMAX;							//	�Q�C���ő�
	eff.dwTriggerButton                 = DIEB_NOTRIGGER;							//	�g���K�[�Ȃ�
	eff.dwTriggerRepeatInterval         = 0;
	eff.cAxes                           = 2;										//	2��
	eff.rgdwAxes                        = rdwAxes;
	eff.rglDirection                    = rglDirection;
	eff.lpEnvelope                      = NULL;
	eff.cbTypeSpecificParams            = sizeof( DICONSTANTFORCE );
	eff.lpvTypeSpecificParams           = &cf;
	eff.dwStartDelay                    = 0;

	
	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		//	�Q�[���p�b�h�̃f�o�C�X����������
		if( g_pDevGamePad[ CntPad ] != NULL )
		{

			//	�f�[�^�t�H�[�}�b�g�̐ݒ�

			//	�t�H�[�X�t�B�[�h�o�b�N����
			if( g_pDevGamePad[ CntPad ] -> SetDataFormat( &c_dfDIJoystick ) )
			{
				//	c_dfDIJoystick�`���̐ݒ�Ɏ��s
				MessageBox( NULL , "c_dfDIJoystick�`���̐ݒ�Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

				return E_FAIL;
			}

			//	�������[�h�̐ݒ�

			//	���[�h��ݒ�
			if( FAILED( g_pDevGamePad[ CntPad ] -> SetCooperativeLevel( hWnd , DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
			{
				//	�t�H�A�O���E���h����r�����[�h�̐ݒ�Ɏ��s
				MessageBox( NULL , "�t�H�A�O���E���h����r�����[�h�̐ݒ�Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

				return E_FAIL;
			}

			//	�f�o�C�X�̐ݒ�
			DeviceCnfiguration();

			//	�t�H�[�X�t�B�[�h�o�b�N�̏�����
			InitFeedBack( hWnd );

			// �Q�[���p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n) �f�o�C�X�ɂ���ĉ�����Ă΂Ȃ���΂Ȃ�Ȃ����Ƃ�����
			g_pDevGamePad[ CntPad ] -> Acquire();

			//	�G�t�F�N�g�E�I�u�W�F�N�g�̍쐬
			if( FAILED( g_pDevGamePad[ CntPad ] -> CreateEffect( GUID_ConstantForce ,
																&eff ,						//	�G�t�F�N�g�ɂ��Đݒ肵���\����
																&g_pDIEffect[ CntPad ] ,	//	�G�t�F�N�g�E�I�u�W�F�N�g���󂯎��|�C���^�̃|�C���^
																NULL ) ) )					//	�W���̂Ȃ�
			{
				MessageBox( NULL , "�G�t�F�N�g�E�I�u�W�F�N�g�쐬�Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

				return E_FAIL;
			}

		}
		else
		{
			g_pDevGamePad[ CntPad ] = NULL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 �֐���:	void UninitGamePad( void )
 ����:		
 �߂�l:	
 ����:		�Q�[���p�b�h�I��
-----------------------------------------------------------------------------*/
void UninitGamePad( void )
{
	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		//	�Q�[���p�b�h�̊J��
		if( g_pDevGamePad[ CntPad ] != NULL )
		{
			//	�Q�[���p�b�h�ւ̃A�N�Z�X�����J��(���͐���I��)
			g_pDevGamePad[ CntPad ] ->Unacquire();

			g_pDevGamePad[ CntPad ] ->Release();
			g_pDevGamePad[ CntPad ] = NULL;
		}
	}

	//	���͏����I��
	UninitGamePadInput();
}

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateGamePad(void)
 ����:		
 �߂�l:	
 ����:		�Q�[���p�b�h�X�V
-----------------------------------------------------------------------------*/
void UpdateGamePad(void)
{
	BYTE aPadState[NUM_BUTTOM_MAX];
	DIJOYSTATE JoyStick;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		if( g_pDevGamePad[ CntPad ] != NULL )
		{

			// �f�o�C�X����f�[�^���擾(�d�v)
			if( SUCCEEDED( g_pDevGamePad[ CntPad ] -> GetDeviceState( sizeof( DIJOYSTATE ) , &JoyStick ) ) )
			{
				for( int nCnKey = 0 ; nCnKey < NUM_BUTTOM_MAX ; nCnKey++ )
				{
					//	�X�e�B�b�N�̎擾
					g_PadStick_X = JoyStick.lX;
					g_PadStick_Y = JoyStick.lY;

					//	�{�^���̎擾
					aPadState[ nCnKey ] = JoyStick.rgbButtons[ nCnKey ];

					// �L�[�g���K�[ �����[�X���̍쐬
					// 1�t���[���O�̃L�[�󋵂ƍ��̃L�[�󋵂�XOR�������̂����̃L�[�󋵂�AND���Ƃ�
					g_aPadStateTrigger [nCnKey] = ( (g_aPadState [nCnKey] ^ aPadState [nCnKey]) & aPadState [nCnKey] );
					g_aPadStateRelease [nCnKey] = ( (g_aPadState [nCnKey] ^ aPadState [nCnKey]) & g_aPadState [nCnKey] );

					// �L�[���s�[�g���̐���
					if ( aPadState [nCnKey] )
					{
						if ( g_aPadStateRepeatCnt [nCnKey] < LIMIT_BUTTOM_COUNT_REPEAT )
						{
							g_aPadStateRepeatCnt [nCnKey]++;

							if ( g_aPadStateRepeatCnt [nCnKey] == 1 || g_aPadStateRepeatCnt [nCnKey] > LIMIT_BUTTOM_COUNT_REPEAT )
							{
								g_aPadStateRepeat [nCnKey] = aPadState [nCnKey];
							}
							else
							{
								g_aPadStateRepeat [nCnKey] = 0;
							}
						}
					}
					else
					{
						g_aPadStateRepeat [nCnKey] = 0;
						g_aPadStateRepeatCnt [nCnKey] = 0;
					}

					// �L�[�v���X����ۑ�
					g_aPadState [nCnKey] = aPadState [nCnKey];
				}
			}
			else
			{
				// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
				g_pDevGamePad[ CntPad ] -> Acquire();
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 �֐���:	BOOL CALLBACK EnumAxesCallBack( LPCDIDEVICEOBJECTINSTANCE Ipddoi , LPVOID pvRef )
 ����:		LPCDIDEVICEOBJECTINSTANCE Ipddoi
			LPVOID pvRef
 �߂�l:	return DIENUM_STOP;
			return DIENUM_CONTINUE;
 ����:		�����ƂɌĂяo�����R�[���o�b�N�֐�
-----------------------------------------------------------------------------*/
BOOL CALLBACK EnumAxesCallBack( LPCDIDEVICEOBJECTINSTANCE Ipddoi , LPVOID pvRef )
{
	DIPROPRANGE diprg;

	ZeroMemory( &diprg , sizeof( diprg ) );

	diprg.diph.dwSize       = sizeof( diprg );
	diprg.diph.dwHeaderSize = sizeof( diprg.diph );
	diprg.diph.dwObj        = Ipddoi -> dwType;
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.lMin              = -1000;
	diprg.lMax              =  1000;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		if( FAILED( g_pDevGamePad[ CntPad ] -> SetProperty( DIPROP_RANGE , &diprg.diph ) ) )
		{
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}

/*-----------------------------------------------------------------------------
 �֐���:	bool GetGamePadPress(int nKey)
 ����:		int nKey
 �߂�l:	return (g_aPadState[nKey] & 0x80) ? true: false;
 ����:		�Q�[���p�b�h�̃v���X��Ԃ��擾
-----------------------------------------------------------------------------*/
bool GetGamePadPress( int nKey )
{
	// �擪�P�r�b�g�������Ă��牟����Ă��邱�ƂɂȂ�
	return (g_aPadState[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 �֐���:	bool GetGamePadTrigger(int nKey)
 ����:		int nKey
 �߂�l:	return (g_aPadStateTrigger[nKey] & 0x80) ? true: false;
 ����:		�Q�[���p�b�h�̃g���K�[��Ԃ��擾(�������u��)
-----------------------------------------------------------------------------*/
bool GetGamePadTrigger(int nKey)
{
	return (g_aPadStateTrigger[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 �֐���:	bool GetGamePadRepeat(int nKey)
 ����:		int nKey
 �߂�l:	return (g_aPadStateRepeat[nKey] & 0x80) ? true: false;
 ����:		�Q�[���p�b�h�̃��s�[�g��Ԃ��擾
-----------------------------------------------------------------------------*/
bool GetGamePadRepeat(int nKey)
{
	return (g_aPadStateRepeat[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 �֐���:	bool GetGamePadRelease(int nKey)
 ����:		int nKey
 �߂�l:	return (g_aPadStateRelease[nKey] & 0x80) ? true: false;
 ����:		�Q�[���p�b�h�̃����|�X��Ԃ��擾(�������u��)
-----------------------------------------------------------------------------*/
bool GetGamePadRelease(int nKey)
{
	return (g_aPadStateRelease[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 �֐���:	LONG GetGamePadStick_X( void )
 ����:		
 �߂�l:	return g_PadStick_X;
 ����:		�X�e�B�b�NX���W�̎擾
-----------------------------------------------------------------------------*/
LONG GetGamePadStick_X( void )
{
	return g_PadStick_X;
}

/*-----------------------------------------------------------------------------
 �֐���:	LONG GetGamePadStick_X( void )
 ����:		
 �߂�l:	return g_PadStick_X;
 ����:		�X�e�B�b�NY���W�̎擾
-----------------------------------------------------------------------------*/
LONG GetGamePadStick_Y( void )
{
	return g_PadStick_Y;
}

/*-----------------------------------------------------------------------------
 �֐���:	BOOL CALLBACK EnumJoyCallback( const DIDEVICEINSTANCE *pdidInstance , void *pContext )
 ����:		const DIDEVICEINSTANCE *pdidInstance
			void *pContext
 �߂�l:	return DIENUM_CONTINUE;	//	�񋓂𑱂���
			return DIENUM_STOP;			//	�񋓂��I������
 ����:		�W���C�X�e�B�b�N��񋓂���R�[���o�b�N�֐�
-----------------------------------------------------------------------------*/
BOOL CALLBACK EnumJoyCallback( const DIDEVICEINSTANCE *pdidInstance , void *pContext )
{
	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{

		//	�񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�[�X���擾
		if( g_pDInput -> CreateDevice( pdidInstance -> guidInstance , &g_pDevGamePad[ CntPad ] , NULL ) )
		{
			return DIENUM_CONTINUE;	//	�񋓂𑱂���
		}

		//	�W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
		g_diDevCaps .dwSize = sizeof( DIDEVCAPS );

		if( FAILED(g_pDevGamePad[ CntPad ] -> GetCapabilities( &g_diDevCaps ) ) )
		{
			//	�W���C�X�e�B�b�N�\�͂̎擾�Ɏ��s
			g_pDevGamePad[ CntPad ] -> Release();
			g_pDevGamePad[ CntPad ] = NULL;

			return DIENUM_CONTINUE;	//	�񋓂𑱂���
		}
	}

	//	�W���C�X�e�B�b�N�̔\�͂��`�F�b�N����

	//	���̃f�o�C�X���g��

	return DIENUM_STOP;			//	�񋓂��I������
	//return DIENUM_CONTINUE;	//	���̃W���C�X�e�B�b�N��T��( �񋓂��� )�ꍇ
}

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT DeviceCnfiguration( void )
 ����:		
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		�f�o�C�X�̐ݒ�
-----------------------------------------------------------------------------*/
HRESULT DeviceCnfiguration( void )
{
	DIPROPDWORD diprop;

	diprop.diph.dwSize       = sizeof( diprop );
	diprop.diph.dwHeaderSize = sizeof( diprop.diph );
	diprop.diph.dwObj        = 0;
	diprop.diph.dwHow        = DIPH_DEVICE;
	
	//	��Βl���[�h
	diprop.dwData            = DIPROPAXISMODE_ABS;

	//	���Βl���[�h
	//diprop.dwData          = DIPROPAXISMODE_REL;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		//	�����[�h�̐ݒ�
		if( FAILED( g_pDevGamePad[ CntPad ] -> SetProperty( DIPROP_AXISMODE , &diprop.diph ) ) )
		{
			//	�����[�h�̐ݒ�Ɏ��s
			MessageBox( NULL , "�����[�h�̐ݒ�Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}

		//	���̗�
		if( FAILED( g_pDevGamePad[ CntPad ] -> EnumObjects( EnumAxesCallBack , NULL , DIDFT_AXIS ) ) )
		{
			//	���̗񋓂Ɏ��s
			MessageBox( NULL , "���̗񋓂Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT BufferSize( void )
 ����:		
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		�o�b�t�@�T�C�Y�̐ݒ�
-----------------------------------------------------------------------------*/
HRESULT BufferSize( void )
{
	DIPROPDWORD diprop;

	diprop.diph.dwSize       = sizeof( diprop );
	diprop.diph.dwHeaderSize = sizeof( diprop.diph );
	diprop.diph.dwObj        = 0;
	diprop.diph.dwHow        = DIPH_DEVICE;
	diprop.dwData            = 1000;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		if( g_pDevGamePad[ CntPad ] -> SetProperty( DIPROP_BUFFERSIZE , &diprop.diph ) )
		{
			//	�o�b�t�@�T�C�Y�̐ݒ�Ɏ��s
			MessageBox( NULL , "�o�b�t�@�T�C�Y�̐ݒ�Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT InitFeedBack( HWND hWnd )
 ����:		
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		�t�H�[�X�t�B�[�h�o�b�N�̏�����
-----------------------------------------------------------------------------*/
HRESULT InitFeedBack( HWND hWnd )
{
	//	�����Z���^�����O�̖�����
	DIPROPDWORD DIPropAutoCenter;

	DIPropAutoCenter.diph.dwSize       = sizeof( DIPropAutoCenter );
	DIPropAutoCenter.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	DIPropAutoCenter.diph.dwObj        = 0;
	DIPropAutoCenter.diph.dwHow        = DIPH_DEVICE;
	DIPropAutoCenter.dwData            = DIPROPAUTOCENTER_OFF;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		if( FAILED( g_pDevGamePad[ CntPad ] -> SetProperty( DIPROP_AUTOCENTER , &DIPropAutoCenter.diph ) ) )
		{
			MessageBox( NULL , "�����Z���^�����O�̖������Ɏ��s" , "�x��" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}
	}


	return S_OK;
}

/*-----------------------------------------------------------------------------
 �֐���:	void VibrationStart( int Number )
 ����:		int Number
 �߂�l:	
 ����:		�o�C�u���[�V�����̊J�n
-----------------------------------------------------------------------------*/
void VibrationStart( int Number , int PadNumber )
{
	g_pDIEffect[ PadNumber ] -> Start( Number , 0 );
}

/*-----------------------------------------------------------------------------
 �֐���:	void VibrationStop( void )
 ����:		
 �߂�l:	
 ����:		�o�C�u���[�V�����̒�~
-----------------------------------------------------------------------------*/
void VibrationStop( int PadNumber )
{
	g_pDIEffect[ PadNumber ] -> Stop();
}

/*-----------------------------------------------------------------------------
 �֐���:	LPDIRECTINPUTDEVICE8 *GetGamePad( int IndexPad )
 ����:		int IndexPad
 �߂�l:	return &g_pDevGamePad[ IndexPad ];
 ����:		�Q�[���p�b�h�f�o�C�X�擾
-----------------------------------------------------------------------------*/
LPDIRECTINPUTDEVICE8 *GetGamePad( int IndexPad )
{
	return &g_pDevGamePad[ IndexPad ];
}