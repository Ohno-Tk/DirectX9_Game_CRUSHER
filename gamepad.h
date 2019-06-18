#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define MAX_PAD ( 1 )	//	ゲームパッドの最大数

/*-----------------------------------------------------------------------------
	構造体
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
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT InitGamePad( HINSTANCE hInstance , HWND hWnd );	//	ゲームパッドの初期化
void UninitGamePad( void );	//	ゲームパッド終了
void UpdateGamePad(void);	//	ゲームパッド更新

LPDIRECTINPUTDEVICE8 *GetGamePad( int IndexPad );	//	ゲームパッドデバイス取得

bool GetGamePadPress( int nKey );	//	ゲームパッドのプレス状態を取得
bool GetGamePadTrigger(int nKey);	//	ゲームパッドのトリガー状態を取得(押した瞬間)
bool GetGamePadRepeat(int nKey);	//	ゲームパッドのリピート状態を取得
bool GetGamePadRelease(int nKey);	//	ゲームパッドのリリ－ス状態を取得(離した瞬間)

LONG GetGamePadStick_X( void );		//	スティックX座標の取得
LONG GetGamePadStick_Y( void );		//	スティックY座標の取得

void VibrationStart( int Number , int PadNumber );	//	バイブレーションの開始
void VibrationStop( int PadNumber );			//	バイブレーションの停止
#endif