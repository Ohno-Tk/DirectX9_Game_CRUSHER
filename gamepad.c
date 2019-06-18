/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "gamepad.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/


#define NUM_BUTTOM_MAX ( 32 )	//	ボタンの最大数
#define	LIMIT_BUTTOM_COUNT_REPEAT	(20)	// リピートカウントリミッター

#define DURATION ( 0.5 )			//	継続時間

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECTINPUT8			g_pDInput = NULL;						// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevGamePad[ MAX_PAD ] = { NULL };		// 入力デバイス(ゲームパッド)へのポインタ	複数にするには配列にする?
LPDIRECTINPUTEFFECT     g_pDIEffect[ MAX_PAD ] = { NULL };	//	エフェクト・オブジェクト

DIDEVCAPS g_diDevCaps;											//	ジョイスティックの能力情報


BYTE					g_aPadState[NUM_BUTTOM_MAX];			// ゲームパッドの入力情報ワーク
BYTE					g_aPadStateTrigger[NUM_BUTTOM_MAX];		// ゲームパッドのトリガー情報ワーク
BYTE					g_aPadStateRelease[NUM_BUTTOM_MAX];		// ゲームパッドのリリース情報ワーク
BYTE					g_aPadStateRepeat[NUM_BUTTOM_MAX];		// ゲームパッドのリピート情報ワーク
int						g_aPadStateRepeatCnt[NUM_BUTTOM_MAX];	// ゲームパッドのリピートカウンタ

LONG g_PadStick_X;	//	スティックのX
LONG g_PadStick_Y;	//	スティックのY

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
BOOL CALLBACK EnumJoyCallback( const DIDEVICEINSTANCE *pdidInstance , void *pContext );	//	ジョイスティックを列挙するコールバック関数

HRESULT InitGamePadInput( HINSTANCE hInstance, HWND hWnd );								//	入力処理初期化
void UninitGamePadInput( void );														//	入力処理終了

HRESULT DeviceCnfiguration( void );														//	デバイスの設定
BOOL CALLBACK EnumAxesCallBack( LPCDIDEVICEOBJECTINSTANCE Ipddoi , LPVOID pvRef );		//	軸ごとに呼び出されるコールバック関数
HRESULT BufferSize( void );																//	バッファサイズの設定

HRESULT InitFeedBack( HWND hWnd );														//	フォースフィードバックの初期化

/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitInput( HINSTANCE hInstance, HWND hWnd )
 引数:		HINSTANCE hInstance
			HWND hWnd
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		入力処理初期化
-----------------------------------------------------------------------------*/
HRESULT InitGamePadInput( HINSTANCE hInstance, HWND hWnd )
{
	//	ヌルチェック
	if( g_pDInput == NULL )
	{
		//	DirectInput8オブジェクトの作成
		if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8, (void**)&g_pDInput, NULL ) ) )
		{
			MessageBox( NULL , "DirectInput8オブジェクトの作成に失敗" , "警告" , MB_OK | MB_ICONHAND );
			return E_FAIL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 関数名:	void UninitInput( void )
 引数:		
 戻り値:	
 説明:		入力処理終了
-----------------------------------------------------------------------------*/
void UninitGamePadInput( void )
{
	// DirectInputオブジェクトの開放
	if( g_pDInput != NULL )
	{
		g_pDInput -> Release();
		g_pDInput = NULL;
	}
}

/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitGamePad( HINSTANCE hInstance , HWND hWnd )
 引数:		HINSTANCE hInstance
			HWND hWnd
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		ゲームパッドの初期化
-----------------------------------------------------------------------------*/
HRESULT InitGamePad( HINSTANCE hInstance , HWND hWnd )
{
	//	入力処理初期化
	if( FAILED( InitGamePadInput(hInstance , hWnd ) ) )
	{
		//	DirectInput8オブジェクト作成に失敗
		MessageBox( NULL , "DirectInput8オブジェクト作れない" , "警告" , MB_OK | MB_ICONHAND );

		return E_FAIL;
	}

	//	ジョイスティックを列挙して作成
	if( FAILED( g_pDInput -> EnumDevices( DI8DEVCLASS_GAMECTRL , EnumJoyCallback , NULL , DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY ) ) )
	{
		//	DirectInputDevice8オブジェクトの作成に失敗
		MessageBox( NULL , "DirectInputDevice8オブジェクトの作成に失敗\n フォースフィードバック対応のジョイスティックがありません" , "警告" , MB_OK | MB_ICONHAND );

		return E_FAIL;
	}

	DWORD rdwAxes[ 2 ] = { DIJOFS_X , DIJOFS_Y };
	LONG  rglDirection[ 2 ] = { 0 , 0 };

	DICONSTANTFORCE cf = { 0 };
	cf.lMagnitude = 10000;

	DIEFFECT eff;

	ZeroMemory( &eff , sizeof( eff ) );
	eff.dwSize                          = sizeof( DIEFFECT );
	eff.dwFlags                         = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;	//	デカルト座標
	eff.dwDuration                      = ( DWORD )( DI_SECONDS * DURATION );			//	1回の持続時間
	eff.dwSamplePeriod                  = 0;
	eff.dwGain                          = DI_FFNOMINALMAX;							//	ゲイン最大
	eff.dwTriggerButton                 = DIEB_NOTRIGGER;							//	トリガーなし
	eff.dwTriggerRepeatInterval         = 0;
	eff.cAxes                           = 2;										//	2軸
	eff.rgdwAxes                        = rdwAxes;
	eff.rglDirection                    = rglDirection;
	eff.lpEnvelope                      = NULL;
	eff.cbTypeSpecificParams            = sizeof( DICONSTANTFORCE );
	eff.lpvTypeSpecificParams           = &cf;
	eff.dwStartDelay                    = 0;

	
	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		//	ゲームパッドのデバイスがあったら
		if( g_pDevGamePad[ CntPad ] != NULL )
		{

			//	データフォーマットの設定

			//	フォースフィードバック無し
			if( g_pDevGamePad[ CntPad ] -> SetDataFormat( &c_dfDIJoystick ) )
			{
				//	c_dfDIJoystick形式の設定に失敗
				MessageBox( NULL , "c_dfDIJoystick形式の設定に失敗" , "警告" , MB_OK | MB_ICONHAND );

				return E_FAIL;
			}

			//	協調モードの設定

			//	モードを設定
			if( FAILED( g_pDevGamePad[ CntPad ] -> SetCooperativeLevel( hWnd , DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
			{
				//	フォアグラウンド＆非排他モードの設定に失敗
				MessageBox( NULL , "フォアグラウンド＆非排他モードの設定に失敗" , "警告" , MB_OK | MB_ICONHAND );

				return E_FAIL;
			}

			//	デバイスの設定
			DeviceCnfiguration();

			//	フォースフィードバックの初期化
			InitFeedBack( hWnd );

			// ゲームパッドへのアクセス権を獲得(入力制御開始) デバイスによって何回も呼ばなければならないことがある
			g_pDevGamePad[ CntPad ] -> Acquire();

			//	エフェクト・オブジェクトの作成
			if( FAILED( g_pDevGamePad[ CntPad ] -> CreateEffect( GUID_ConstantForce ,
																&eff ,						//	エフェクトについて設定した構造体
																&g_pDIEffect[ CntPad ] ,	//	エフェクト・オブジェクトを受け取るポインタのポインタ
																NULL ) ) )					//	集合体なし
			{
				MessageBox( NULL , "エフェクト・オブジェクト作成に失敗" , "警告" , MB_OK | MB_ICONHAND );

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
 関数名:	void UninitGamePad( void )
 引数:		
 戻り値:	
 説明:		ゲームパッド終了
-----------------------------------------------------------------------------*/
void UninitGamePad( void )
{
	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		//	ゲームパッドの開放
		if( g_pDevGamePad[ CntPad ] != NULL )
		{
			//	ゲームパッドへのアクセス権を開放(入力制御終了)
			g_pDevGamePad[ CntPad ] ->Unacquire();

			g_pDevGamePad[ CntPad ] ->Release();
			g_pDevGamePad[ CntPad ] = NULL;
		}
	}

	//	入力処理終了
	UninitGamePadInput();
}

/*-----------------------------------------------------------------------------
 関数名:	void UpdateGamePad(void)
 引数:		
 戻り値:	
 説明:		ゲームパッド更新
-----------------------------------------------------------------------------*/
void UpdateGamePad(void)
{
	BYTE aPadState[NUM_BUTTOM_MAX];
	DIJOYSTATE JoyStick;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		if( g_pDevGamePad[ CntPad ] != NULL )
		{

			// デバイスからデータを取得(重要)
			if( SUCCEEDED( g_pDevGamePad[ CntPad ] -> GetDeviceState( sizeof( DIJOYSTATE ) , &JoyStick ) ) )
			{
				for( int nCnKey = 0 ; nCnKey < NUM_BUTTOM_MAX ; nCnKey++ )
				{
					//	スティックの取得
					g_PadStick_X = JoyStick.lX;
					g_PadStick_Y = JoyStick.lY;

					//	ボタンの取得
					aPadState[ nCnKey ] = JoyStick.rgbButtons[ nCnKey ];

					// キートリガー リリース情報の作成
					// 1フレーム前のキー状況と今のキー状況をXORしたものを今のキー状況とANDをとる
					g_aPadStateTrigger [nCnKey] = ( (g_aPadState [nCnKey] ^ aPadState [nCnKey]) & aPadState [nCnKey] );
					g_aPadStateRelease [nCnKey] = ( (g_aPadState [nCnKey] ^ aPadState [nCnKey]) & g_aPadState [nCnKey] );

					// キーリピート情報の生成
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

					// キープレス情報を保存
					g_aPadState [nCnKey] = aPadState [nCnKey];
				}
			}
			else
			{
				// キーボードへのアクセス権を取得
				g_pDevGamePad[ CntPad ] -> Acquire();
			}
		}
	}
}

/*-----------------------------------------------------------------------------
 関数名:	BOOL CALLBACK EnumAxesCallBack( LPCDIDEVICEOBJECTINSTANCE Ipddoi , LPVOID pvRef )
 引数:		LPCDIDEVICEOBJECTINSTANCE Ipddoi
			LPVOID pvRef
 戻り値:	return DIENUM_STOP;
			return DIENUM_CONTINUE;
 説明:		軸ごとに呼び出されるコールバック関数
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
 関数名:	bool GetGamePadPress(int nKey)
 引数:		int nKey
 戻り値:	return (g_aPadState[nKey] & 0x80) ? true: false;
 説明:		ゲームパッドのプレス状態を取得
-----------------------------------------------------------------------------*/
bool GetGamePadPress( int nKey )
{
	// 先頭１ビットが立ってたら押されていることになる
	return (g_aPadState[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 関数名:	bool GetGamePadTrigger(int nKey)
 引数:		int nKey
 戻り値:	return (g_aPadStateTrigger[nKey] & 0x80) ? true: false;
 説明:		ゲームパッドのトリガー状態を取得(押した瞬間)
-----------------------------------------------------------------------------*/
bool GetGamePadTrigger(int nKey)
{
	return (g_aPadStateTrigger[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 関数名:	bool GetGamePadRepeat(int nKey)
 引数:		int nKey
 戻り値:	return (g_aPadStateRepeat[nKey] & 0x80) ? true: false;
 説明:		ゲームパッドのリピート状態を取得
-----------------------------------------------------------------------------*/
bool GetGamePadRepeat(int nKey)
{
	return (g_aPadStateRepeat[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 関数名:	bool GetGamePadRelease(int nKey)
 引数:		int nKey
 戻り値:	return (g_aPadStateRelease[nKey] & 0x80) ? true: false;
 説明:		ゲームパッドのリリ－ス状態を取得(離した瞬間)
-----------------------------------------------------------------------------*/
bool GetGamePadRelease(int nKey)
{
	return (g_aPadStateRelease[nKey] & 0x80) ? true: false;
}

/*-----------------------------------------------------------------------------
 関数名:	LONG GetGamePadStick_X( void )
 引数:		
 戻り値:	return g_PadStick_X;
 説明:		スティックX座標の取得
-----------------------------------------------------------------------------*/
LONG GetGamePadStick_X( void )
{
	return g_PadStick_X;
}

/*-----------------------------------------------------------------------------
 関数名:	LONG GetGamePadStick_X( void )
 引数:		
 戻り値:	return g_PadStick_X;
 説明:		スティックY座標の取得
-----------------------------------------------------------------------------*/
LONG GetGamePadStick_Y( void )
{
	return g_PadStick_Y;
}

/*-----------------------------------------------------------------------------
 関数名:	BOOL CALLBACK EnumJoyCallback( const DIDEVICEINSTANCE *pdidInstance , void *pContext )
 引数:		const DIDEVICEINSTANCE *pdidInstance
			void *pContext
 戻り値:	return DIENUM_CONTINUE;	//	列挙を続ける
			return DIENUM_STOP;			//	列挙を終了する
 説明:		ジョイスティックを列挙するコールバック関数
-----------------------------------------------------------------------------*/
BOOL CALLBACK EnumJoyCallback( const DIDEVICEINSTANCE *pdidInstance , void *pContext )
{
	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{

		//	列挙されたジョイスティックへのインターフェースを取得
		if( g_pDInput -> CreateDevice( pdidInstance -> guidInstance , &g_pDevGamePad[ CntPad ] , NULL ) )
		{
			return DIENUM_CONTINUE;	//	列挙を続ける
		}

		//	ジョイスティックの能力を調べる
		g_diDevCaps .dwSize = sizeof( DIDEVCAPS );

		if( FAILED(g_pDevGamePad[ CntPad ] -> GetCapabilities( &g_diDevCaps ) ) )
		{
			//	ジョイスティック能力の取得に失敗
			g_pDevGamePad[ CntPad ] -> Release();
			g_pDevGamePad[ CntPad ] = NULL;

			return DIENUM_CONTINUE;	//	列挙を続ける
		}
	}

	//	ジョイスティックの能力をチェックする

	//	このデバイスを使う

	return DIENUM_STOP;			//	列挙を終了する
	//return DIENUM_CONTINUE;	//	他のジョイスティックを探す( 列挙する )場合
}

/*-----------------------------------------------------------------------------
 関数名:	HRESULT DeviceCnfiguration( void )
 引数:		
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		デバイスの設定
-----------------------------------------------------------------------------*/
HRESULT DeviceCnfiguration( void )
{
	DIPROPDWORD diprop;

	diprop.diph.dwSize       = sizeof( diprop );
	diprop.diph.dwHeaderSize = sizeof( diprop.diph );
	diprop.diph.dwObj        = 0;
	diprop.diph.dwHow        = DIPH_DEVICE;
	
	//	絶対値モード
	diprop.dwData            = DIPROPAXISMODE_ABS;

	//	相対値モード
	//diprop.dwData          = DIPROPAXISMODE_REL;

	for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ )
	{
		//	軸モードの設定
		if( FAILED( g_pDevGamePad[ CntPad ] -> SetProperty( DIPROP_AXISMODE , &diprop.diph ) ) )
		{
			//	軸モードの設定に失敗
			MessageBox( NULL , "軸モードの設定に失敗" , "警告" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}

		//	軸の列挙
		if( FAILED( g_pDevGamePad[ CntPad ] -> EnumObjects( EnumAxesCallBack , NULL , DIDFT_AXIS ) ) )
		{
			//	軸の列挙に失敗
			MessageBox( NULL , "軸の列挙に失敗" , "警告" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 関数名:	HRESULT BufferSize( void )
 引数:		
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		バッファサイズの設定
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
			//	バッファサイズの設定に失敗
			MessageBox( NULL , "バッファサイズの設定に失敗" , "警告" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}
	}

	return S_OK;
}

/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitFeedBack( HWND hWnd )
 引数:		
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		フォースフィードバックの初期化
-----------------------------------------------------------------------------*/
HRESULT InitFeedBack( HWND hWnd )
{
	//	自動センタリングの無効化
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
			MessageBox( NULL , "自動センタリングの無効化に失敗" , "警告" , MB_OK | MB_ICONHAND );

			return E_FAIL;
		}
	}


	return S_OK;
}

/*-----------------------------------------------------------------------------
 関数名:	void VibrationStart( int Number )
 引数:		int Number
 戻り値:	
 説明:		バイブレーションの開始
-----------------------------------------------------------------------------*/
void VibrationStart( int Number , int PadNumber )
{
	g_pDIEffect[ PadNumber ] -> Start( Number , 0 );
}

/*-----------------------------------------------------------------------------
 関数名:	void VibrationStop( void )
 引数:		
 戻り値:	
 説明:		バイブレーションの停止
-----------------------------------------------------------------------------*/
void VibrationStop( int PadNumber )
{
	g_pDIEffect[ PadNumber ] -> Stop();
}

/*-----------------------------------------------------------------------------
 関数名:	LPDIRECTINPUTDEVICE8 *GetGamePad( int IndexPad )
 引数:		int IndexPad
 戻り値:	return &g_pDevGamePad[ IndexPad ];
 説明:		ゲームパッドデバイス取得
-----------------------------------------------------------------------------*/
LPDIRECTINPUTDEVICE8 *GetGamePad( int IndexPad )
{
	return &g_pDevGamePad[ IndexPad ];
}