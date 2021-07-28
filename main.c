/*=============================================================================

		�g���N����i[ main.cpp ]

-------------------------------------------------------------------------------

	���@�쐬��
		2016/07/28
-------------------------------------------------------------------------------
	���@Update
=============================================================================*/
/******************************************************************************
						scanf ��warning�h�~ 
******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"
#include "gamepad.h"
/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define CLASS_NAME     "�g���N����i"			//	�E�C���h�E�N���X�̖��O
#define WINDOW_NAME    "CRUSHER"	//	�E�C���h�E�̖��O

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3D9 g_pD3D = NULL;	//	Direct3D�C���^�[�t�F�[�X�擾
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//	Direct3D�f�o�C�X�C���^�[�t�F�[�X�i�|�C���^�j

MODE g_mode = MODE_TITLE;	//	���̃��[�h

MODE g_NowMode = g_mode;

#ifdef _DEBUG
	LPD3DXFONT g_pFont = NULL;
#endif	//	_DEBUG
int g_nCountFPS = 0;	//	FPS�J�E���^

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/
#ifdef _DEBUG
void DrawFPS( void );
#endif	//	_DEBUG
/*-----------------------------------------------------------------------------
	���C���֐�
-----------------------------------------------------------------------------*/
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR IpCmdLine , int nCmdShow )
{
	//  waring�̌x����������
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( IpCmdLine );

	//	�E�C���h�E�̃p�[�c�o�^
	WNDCLASSEX wcex =
	{
		sizeof( WNDCLASSEX ) ,					//	WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC ,							//	�\������E�C���h�E�̃X�^�C����ݒ�
		WndProc ,								//	�E�B���h�E�v���V�[�W���̃A�h���X( �֐��� )���w��@�v���g�^�C�v�錾�����֐��������i�d�v�I�I�j
		0 ,										//	�ʏ�͎g�p���Ȃ��̂�" 0 "���w��
		0 ,										//	�ʏ�͎g�p���Ȃ��̂�" 0 "���w��
		hInstance ,								//	WinMain�̃p�����[�^�̃C���X�^���X�n���h����ݒ�
		NULL ,									//	�g�p����A�C�R�����w��( Windows�������Ă���A�C�R�����g���Ȃ� )
		LoadCursor( NULL , IDC_ARROW ),			//	�}�E�X�J�[�\�����w��	�i���Ƃł͂��Ȃ��j
		( HBRUSH )( COLOR_WINDOW + 1 ) ,		//	�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�	�i���Ƃł͂��Ȃ��j
		NULL ,									//	Windows�ɂ��郁�j���[��ݒ�
		CLASS_NAME ,							//	�E�C���h�E�N���X�̖��O	�i�d�v�I�I�j
		NULL									//	�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q	�i���Ƃł͂��Ȃ��j
	};

	//	�E�C���h�E�N���X�̓o�^
	RegisterClassEx( &wcex );	//	�I���W�i���̃E�C���h�E�p�[�c�̖��O������( �J�X�^���E�C���h�E�p�̐ݒ� )

	HWND hWnd;	//	�E�C���h�E�n���h���i�@���C���n���h���@�j
	MSG msg;

	const int Windowstyle = WS_OVERLAPPEDWINDOW ^  WS_MINIMIZEBOX ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME;

	//	�`��̈�̃T�C�Y����E�C���h�E�̋�`���擾
	RECT cr = { -300 , -100 , SCREEN_WIDTH , SCREEN_HEIGHT };
	AdjustWindowRect( &cr , Windowstyle , FALSE );

	RECT dr;
	GetWindowRect( GetDesktopWindow() , &dr );

	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	int wx = ww > dw ? 0 : ( dw - ww ) / 2 ;
	int wy = wh > dh ? 0 : ( dh - wh ) / 2 ;

	//	�E�C���h�E�̍쐬
	hWnd/*���E�C���h�n���h��*/ = CreateWindowEx( 0 ,		//	�g���E�C���h�E�X�^�C���i�@������Ɛݒ肷��@�j
		CLASS_NAME ,										//	�E�C���h�E�N���X�̖��O
		WINDOW_NAME ,										//	�E�C���h�E�̖��O
		Windowstyle ,								//	�E�C���h�E�X�^�C��
		wx ,										//	�E�C���h�E�̍���w���W
		wy ,										//	�E�C���h�E�̍���x���W
		ww ,										//	�E�C���h�E�̕��i�@�t���[�������킹�����@�j
		wh ,										//	�E�C���h�E�̍����i�@�t���[�������킹�������@�j
		NULL ,												//	�e�E�C���h�E�̃n���h���i�@�|�C���^�@�j
		NULL ,												//	���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance ,											//	�C���X�^���X�n���h��
		NULL );												//	�E�C���h�E�쐬�f�[�^

	//	�E�C���h�E�̕\��
	ShowWindow( hWnd , nCmdShow );		//	�w�肳�ꂽ�E�C���h�E�̕\��
	UpdateWindow( hWnd );				//	�E�C���h�E�̏�Ԃ𒼂��ɔ��f

	Init( hInstance , hWnd , TRUE );	//	�������֐�

	//	����\�̐ݒ�i�^�C�}�[�̐��x��ǂ�����j
	timeBeginPeriod( 1 );

	//	�e�J�E���^�[�̏�����
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;	//	���݂̎���
	DWORD dwExecLastTime = timeGetTime();	//	���݂̎������~���P�ʂŎ擾
	DWORD dwFPSLastTime = dwExecLastTime;

	//	���b�Z�[�W���[�v
	//	��P�����F���b�Z�[�W���
	//	��Q�����F�E�C���h�E�̃n���h��
	//	��R�����F�ŏ��̃��b�Z�[�W
	//	��S�����F�Ō�̃��b�Z�[�W
	for( ;; )
	{
		if( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) != 0 )
		{
			//	Windows�̏���
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				//	�Ăяo�����X���b�h�̃��b�Z�[�W�L���[����
				//	���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();	//	���݂̎������~���P�ʂŎ擾
			if( (dwCurrentTime - dwFPSLastTime) >= 500 )
			{
				g_nCountFPS = ( dwFrameCount * 1000 ) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if( (dwCurrentTime - dwExecLastTime) >= (1000/60) )	//	���݂̃t���[������ǂ�ʂ������̂�( �����v�Z )	�f�o�b�O�Ōv���������Ƃ���if���R�����g�A�E�g
			{
				dwExecLastTime = dwCurrentTime;	//	�����������Ԃ�ۑ�

				//	DirectX�̏����i�Q�[�������j
				Update();	//	�X�V����

				Draw();	//	�`�揈��

				dwFrameCount++;

			}
		}
	}

	//	�E�C���h�E�N���X�̓o�^������
	//	�������F���N���X��
	//	�������F�A�v���P�[�V�����C���X�^���X
	UnregisterClass( CLASS_NAME , wcex.hInstance );

	Uninit();	//	�I������

	timeEndPeriod( 1 );	//	����\�̏I��

	return ( int )msg.wParam;
}

/*-----------------------------------------------------------------------------
				�E�C���h�E�v���V�[�W��
		( CALLBACK:Win32API�֐����Ăяo���Ƃ��̋K�� )
		hWnd :   �E�C���h�E�̃n���h��
		uMsg :   ���b�Z�[�W�̎��ʎq
		wParam : ���b�Z�[�W�̍ŏ��̃p�����[�^
		lParam : ���b�Z�[�W�̂Q�Ԗڂ̃p�����[�^
-------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	UINT nSelect;	//	Esc�L�[�̓��͔���

	switch( uMsg )
	{

	case WM_DESTROY:										//	�E�C���h�E�j���̃��b�Z�[�W
		PostQuitMessage( 0 );								//	"WM_QUIT"���b�Z�[�W��Ԃ�
		break;

	case WM_KEYDOWN:	//	�L�[����
		switch( wParam )
		{
			case VK_ESCAPE:	//	ESCAPE�L�[
			nSelect = MessageBox( NULL , "�I�����܂����H" , "�I�����b�Z�[�W" , MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
			if( nSelect == IDYES )
			{
				DestroyWindow( hWnd );
			}
			break;
		}

	default:
		break;
	}

	return DefWindowProc( hWnd , uMsg , wParam , lParam );	//	����̏������
}

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow )
 ����:		HINSTANCE hInstance
			HWND hWnd
			BOOL bWindow
 �߂�l:	S_OK
 ����:		�������֐�
-----------------------------------------------------------------------------*/
HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow )
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;


	//	D3D9�C���^�[�t�F�[�X�̎擾
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( g_pD3D == NULL )
	{
		return E_FAIL;
	}
	//	���݂̃f�B�X�v���C���[�h�̎擾
	if( FAILED( g_pD3D -> GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm ) ) )
	{
		return E_FAIL;
	}
	//	�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�(�f�o�C�X�̐ݒ�̂��߂̐ݒ�)
	ZeroMemory( &d3dpp , sizeof( d3dpp ) );
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//	�f�o�b�O�Ōv���������Ƃ���D3DPRESENT_INTERVAL_IMMEDIATE�ɂ���

	//	Direct3D�f�o�C�X�̍쐬
	//	�ǂ�����n�[�h�E�F�A����
	if( FAILED( g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , hWnd , D3DCREATE_HARDWARE_VERTEXPROCESSING , &d3dpp , &g_pD3DDevice ) ) )
	{
		//	�`�揈���̓n�[�h�E�F�A�����@���_�����̓\�t�g�E�F�A����
		if( FAILED( g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , hWnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &g_pD3DDevice ) ) )
		{
			//	�ǂ�����\�t�g�E�F�A����
			if( FAILED( g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT , D3DDEVTYPE_REF , hWnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &g_pD3DDevice ) ) )
			{
				MessageBox( NULL , "�f�o�C�X�����܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );
				return E_FAIL;
			}
		}
	}

	g_pD3DDevice -> SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );

	//	���C���[�t���[��
	//g_pD3DDevice -> SetRenderState( D3DRS_FILLMODE , D3DFILL_WIREFRAME );

	//	a�u�����h
	g_pD3DDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	g_pD3DDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	g_pD3DDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );

	//	�A���t�@�u�����h�̐ݒ�
	g_pD3DDevice -> SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_MODULATE );
	g_pD3DDevice -> SetTextureStageState( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	g_pD3DDevice -> SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );

	//	�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );

	//	�e�N�X�`���A�h���b�V���O���[�h
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP );
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP );

	InitSound( hWnd );	//	���y�̏�����

	InitKeyboard( hInstance, hWnd);	// �L�[�{�[�h�̏�����

	InitGamePad( hInstance , hWnd );	//	�Q�[���p�b�h�̏�����

	InitFade();

	SetMode(g_mode);

#ifdef _DEBUG
	D3DXCreateFont( g_pD3DDevice ,
					18 ,	//	��������
					0 ,	//	������
					0 ,	//	�����̑���
					0 ,	//	�~�b�v�}�b�v
					FALSE ,	//	�C�^���b�N
					SHIFTJIS_CHARSET ,
					OUT_DEFAULT_PRECIS ,
					DEFAULT_QUALITY ,
					DEFAULT_PITCH ,
					"Terminal" ,
					&g_pFont);
#endif	//	_DEBUG

	return S_OK;

}

/*-----------------------------------------------------------------------------
 �֐���:	void Uninit( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�I���֐�
-----------------------------------------------------------------------------*/
void Uninit( void )
{
	ModeFunc pUninitFunc[]={
	UninitTitle,
	UninitTutorial,
	UninitGame,
	UninitResult,
	UninitRanking,
	};

	if( g_pD3DDevice != NULL )
	{
		g_pD3DDevice -> Release();	//	Direct3DDevice�J��
		g_pD3DDevice = NULL;	
	}
	if( g_pD3D != NULL )
	{
		g_pD3D -> Release();	//	Direct3D�C���^�[�t�F�[�X�J��
		g_pD3D = NULL;
	}
#ifdef _DEBUG
	if( g_pFont != NULL )
	{
		g_pFont -> Release();	//	�t�H���g�̃C���^�[�t�F�[�X�J��
		g_pFont = NULL;
	}
#endif	//	_DEBUG

	UninitSound();	//	���y�̏I��

	UninitKeyboard();	// �L�[�{�[�h�̏I��

	UninitGamePad();	//	�Q�[���p�b�h�I��

	UninitFade();

	pUninitFunc[g_mode]();
}

/*-----------------------------------------------------------------------------
 �֐���:	void Update( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�X�V�֐�
-----------------------------------------------------------------------------*/
void Update( void )
{
	ModeFunc pUpdateFunc[]={
	UpdateTitle,
	UpdateTutorial,
	UpdateGame,
	UpdateResult,
	UpdateRanking,
	};

	UpdateKeyboard();	//	�L�[�{�[�h�̍X�V

	UpdateGamePad();	//	�Q�[���p�b�h�X�V

	UpdateFade();

	pUpdateFunc[g_mode]();
}

/*-----------------------------------------------------------------------------
 �֐���:	void Draw( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�`��֐�
-----------------------------------------------------------------------------*/
void Draw( void )
{
	ModeFunc pDrawFunc[]={
	DrawTitle,
	DrawTutorial,
	DrawGame,
	DrawResult,
	DrawRanking,
	};

	//	�o�b�N�o�b�t�@�Ƃy�o�b�t�@�̃N���A
	g_pD3DDevice ->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_RGBA( 255 , 255 , 255 , 255 ) , 1.0f , 0 );

	//	�`��̊J�n
	if( SUCCEEDED( g_pD3DDevice -> BeginScene() ) )
	{
		pDrawFunc[g_mode]();

		DrawFade();

#ifdef _DEBUG
		DrawFPS();
#endif	//	_DEBUG

		//	�`��I��
		g_pD3DDevice -> EndScene();
	}
	g_pD3DDevice -> Present( NULL , NULL , NULL , NULL );

}
/*-----------------------------------------------------------------------------
 �֐���:	void SetMode(MODE mode)
 ����:		MODE mode
 �߂�l:	�Ȃ�
 ����:		���[�h�ݒu
-----------------------------------------------------------------------------*/
void SetMode(MODE mode)
{
	ModeFunc pInitFunc[]={
	InitTitle,
	InitTutorial,
	InitGame,
	InitResult,
	InitRanking,
	};

	ModeFunc pUninitFunc[]={
	UninitTitle,
	UninitTutorial,
	UninitGame,
	UninitResult,
	UninitRanking,
	};

	pUninitFunc[g_mode]();
	pInitFunc[mode]();

	g_mode = mode;

}
/*-----------------------------------------------------------------------------
 �֐���:	void SetMode(MODE mode)
 ����:		MODE mode
 �߂�l:	�Ȃ�
 ����:		���[�h�ݒu
-----------------------------------------------------------------------------*/
MODE *GetMode( void )
{
	return &g_NowMode;
}
/*-----------------------------------------------------------------------------
 �֐���:	LPDIRECT3DDEVICE9 GetDevice( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		D3DDevice�̎擾
-----------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 GetDevice( void )
{
	return g_pD3DDevice;
}
/*-----------------------------------------------------------------------------
 �֐���:	void DrawFPS( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		FPS�\��
-----------------------------------------------------------------------------*/
#ifdef _DEBUG
void DrawFPS( void )
{
	RECT rect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	char aStr[ 256 ];

	sprintf( &aStr[ 0 ] , "FPS:%d" , g_nCountFPS );

	g_pFont -> DrawText( NULL ,
						&aStr[ 0 ] ,
						-1 ,
						&rect ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 0 , 255 , 0 , 255));
}
#endif	//	_DEBUG
