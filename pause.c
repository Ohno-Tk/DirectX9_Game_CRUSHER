/*=============================================================================

		ポーズ画面[ pause.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/09/19
-------------------------------------------------------------------------------
	■　Update
		2016/08/22
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "usability.h"
#include "fade.h"
#include "pause.h"
#include "title.h"
#include "gamepad.h"
#include "player.h"
#include "base.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define PAUSE_BG_TEXTURENAME "data/TEXTURE/func/titleBg.jpg"
#define PAUSE_FRAME_TEXTURENAME "data/TEXTURE/game/pause/pauseBg01.jpg"
#define PAUSE_SELECT_TEXTURENAME "data/TEXTURE/game/pause/矢印.png"
#define PAUSE_CONTINUE_TEXTURENAME "data/TEXTURE/game/pause/continue.png"
#define PAUSE_RETRY_TEXTURENAME "data/TEXTURE/game/pause/retry.png"
#define PAUSE_QUIT_TEXTURENAME "data/TEXTURE/game/pause/quit.png"
#define PAUSE_OPERATION_TEXTURENAME "data/TEXTURE/game/pause/operation.png"
#define PAUSE_SCROLL_TEXTURENAME "data/TEXTURE/game/pause/pausescroll.png"

#define MAX_PAUSE_TEXTURENAME ( 8 )	//	ポーズの最大テクスチャ数

#define PAUSE_BG_POS_X ( SCREEN_WIDTH * 0.3f )	//	ポーズ背景X
#define PAUSE_BG_POS_Y ( SCREEN_HEIGHT * 0.05f )	//	ポーズ背景Y
#define PAUSE_BG_WIDTH ( 350.0f )				//	ポーズ背景WIDTH
#define PAUSE_BG_HEIGHT ( 500.0f )				//	ポーズ背景HEIGHT

#define PAUSE_CONTINUE_POS_X ( 400.0f )		//	コンティニューX
#define PAUSE_CONTINUE_POS_Y ( 80.0f )		//	コンティニューY
#define PAUSE_CONTINUE_WIDTH ( 200.0f )		//	コンティニューWIDTH
#define PAUSE_CONTINUE_HEIGHT ( 100.0f )	//	コンティニューHEIGHT

#define PAUSE_RETRY_POS_X ( PAUSE_CONTINUE_POS_X )			//	リトライX
#define PAUSE_RETRY_POS_Y ( PAUSE_CONTINUE_POS_Y + 125.0f )	//	リトライY
#define PAUSE_RETRY_WIDTH ( PAUSE_CONTINUE_WIDTH )			//	リトライWIDTH
#define PAUSE_RETRY_HEIGHT ( PAUSE_CONTINUE_HEIGHT )		//	リトライHEIGHT

#define PAUSE_QUIT_POS_X ( PAUSE_CONTINUE_POS_X )			//	終了X
#define PAUSE_QUIT_POS_Y ( PAUSE_CONTINUE_POS_Y + 325.0f )	//	終了Y
#define PAUSE_QUIT_WIDTH ( PAUSE_CONTINUE_WIDTH )			//	終了WIDTH
#define PAUSE_QUIT_HEIGHT ( PAUSE_CONTINUE_HEIGHT )			//	終了HEIGHT

#define PAUSE_OPERATION_POS_X ( PAUSE_CONTINUE_POS_X )			//	操作方法X
#define PAUSE_OPERATION_POS_Y ( PAUSE_CONTINUE_POS_Y + 225.0f )	//	操作方法Y
#define PAUSE_OPERATION_WIDTH ( PAUSE_CONTINUE_WIDTH )			//	操作方法WIDTH
#define PAUSE_OPERATION_HEIGHT ( PAUSE_CONTINUE_HEIGHT )		//	操作方法HEIGHT

#define PAUSE_SCROLL_POS_X ( SCREEN_WIDTH * 0.31f )	//	ポーズスクロールX
#define PAUSE_SCROLL_POS_Y ( SCREEN_HEIGHT * 0.08f )	//	ポーズスクロールY
#define PAUSE_SCROLL_WIDTH ( 330.0f )				//	ポーズスクロールWIDTH
#define PAUSE_SCROLL_HEIGHT ( 475.0f )				//	ポーズスクロールHEIGHT
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice );						//	頂点の作成
void PauseCursorKeyBoard( void );	//	ボーズカーソル移動
void PauseCursorGamePad( void );	//	ゲームパッドカーソル移動
void PauseCursorPos( VERTEX_2D* pVtx );	//	ボーズカーソル座標変更
void PauseScreenTransition( void );	//	画面遷移
void GamePadScreenTransition( void );	//	ゲームパッドの画面遷移
void PauseScoll( VERTEX_2D* pVtx );	//	ポーズスクロール
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPause = NULL;	//頂点バッファのインターフェースポインタ
//	ポーズ背景ロゴ
LPDIRECT3DTEXTURE9 g_pTexturePauseBg = NULL;//	テクスチャインターフェース

//	ポーズ背景ロゴ
LPDIRECT3DTEXTURE9 g_pTexturePauseFrame = NULL;//	テクスチャインターフェース

//	カーソル
LPDIRECT3DTEXTURE9 g_pTexturePauseSelect = NULL;//	テクスチャインターフェース

//	コンティニュー
LPDIRECT3DTEXTURE9 g_pTexturePauseContinue = NULL;//	テクスチャインターフェース

//	リトライ
LPDIRECT3DTEXTURE9 g_pTexturePauseRetry = NULL;//	テクスチャインターフェース

//	終了
LPDIRECT3DTEXTURE9 g_pTexturePauseQuit = NULL;//	テクスチャインターフェース

//	操作方法
LPDIRECT3DTEXTURE9 g_pTexturePauseOperation = NULL;//	テクスチャインターフェース

//	スクロールポーズ
LPDIRECT3DTEXTURE9 g_pTexturePauseScroll = NULL;//	テクスチャインターフェース

TITLE g_PauseSelect;

//	ディレイ
int CntPuseFrame = 0;

float Pause_U = 0.0f , Pause_V = 0.0f;

//	使用フラグ
bool g_PadKeyFlag = false;

bool g_use = false;
bool g_OperationFlag = false;

bool g_Retry = false;
bool g_Quit = false;
bool g_Operation = false;
/*-----------------------------------------------------------------------------
 関数名:	void InitPause( void )
 引数:		
 戻り値:	
 説明:		ポーズの初期化
-----------------------------------------------------------------------------*/
void InitPause( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック
	//	ポーズ背景
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_BG_TEXTURENAME , &g_pTexturePauseBg  ) ) )
	{
		MessageBox( NULL , "ポーズ背景ロゴの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	ポーズフレーム
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_FRAME_TEXTURENAME , &g_pTexturePauseFrame  ) ) )
	{
		MessageBox( NULL , "ポーズフレームの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	カーソル
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_SELECT_TEXTURENAME , &g_pTexturePauseSelect  ) ) )
	{
		MessageBox( NULL , "ポーズカーソルの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	コンティニュー
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_CONTINUE_TEXTURENAME , &g_pTexturePauseContinue  ) ) )
	{
		MessageBox( NULL , "コンティニューの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	リトライ
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_RETRY_TEXTURENAME , &g_pTexturePauseRetry  ) ) )
	{
		MessageBox( NULL , "リトライの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	終了
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_QUIT_TEXTURENAME , &g_pTexturePauseQuit  ) ) )
	{
		MessageBox( NULL , "終了の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	操作方法
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_OPERATION_TEXTURENAME , &g_pTexturePauseOperation  ) ) )
	{
		MessageBox( NULL , "ポーズの操作方法の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	ポーズスクロール
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PAUSE_SCROLL_TEXTURENAME , &g_pTexturePauseScroll  ) ) )
	{
		MessageBox( NULL , "ポーズスクロールの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	初期化

	//	カーソル初期化
	g_PauseSelect.Pos.x = PAUSE_CONTINUE_POS_X - PAUSE_CONTINUE_WIDTH * 0.5f;
	g_PauseSelect.Pos.y = PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT * 0.2f;
	g_PauseSelect.Size.x = PAUSE_CONTINUE_WIDTH * 0.6f;
	g_PauseSelect.Size.y = PAUSE_CONTINUE_HEIGHT * 0.7f;

	g_PadKeyFlag = false;

	g_use = false;
	g_OperationFlag = false;

	g_Retry = false;
	g_Quit = false;
	g_Operation = false;

	CntPuseFrame = 0;

	Pause_U = 0.0f;
	Pause_V = 0.0f;

	//	頂点の作成
	MakeVertexPause( pDevice );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitPause( void )
 引数:		
 戻り値:	
 説明:		ポーズの終了
-----------------------------------------------------------------------------*/
void UninitPause( void )
{
	if( g_pVtxBufferPause != NULL )	//	頂点バッファ開放
	{
		g_pVtxBufferPause -> Release();
		g_pVtxBufferPause = NULL;
	}	//	end of if

	//	ポーズ背景ロゴ
	if( g_pTexturePauseBg != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseBg -> Release();
		g_pTexturePauseBg = NULL;
	}	//	end of if

	//	ポーズフレーム
	if( g_pTexturePauseFrame != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseFrame -> Release();
		g_pTexturePauseFrame = NULL;
	}	//	end of if

	//	ポーズカーソル
	if( g_pTexturePauseSelect != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseSelect -> Release();
		g_pTexturePauseSelect = NULL;
	}	//	end of if

	//	コンティニュー
	if( g_pTexturePauseContinue != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseContinue -> Release();
		g_pTexturePauseContinue = NULL;
	}	//	end of if

	//	リトライ
	if( g_pTexturePauseRetry != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseRetry -> Release();
		g_pTexturePauseRetry = NULL;
	}	//	end of if

	//	終了
	if( g_pTexturePauseQuit != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseQuit -> Release();
		g_pTexturePauseQuit = NULL;
	}	//	end of if

	//	操作方法
	if( g_pTexturePauseOperation != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseOperation -> Release();
		g_pTexturePauseOperation = NULL;
	}	//	end of if

	//	ポーズスクロール
	if( g_pTexturePauseScroll != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePauseScroll -> Release();
		g_pTexturePauseScroll = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdatePause( void )
 引数:		
 戻り値:	
 説明:		ポーズの更新
-----------------------------------------------------------------------------*/
void UpdatePause( void )
{
	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	プレイヤーの情報の取得
	PLAYER *Player = GetPlayer();

	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	if( Player -> Hit == false && Base -> GoolFalg == false )
	{

		Pause_U += 0.004f;
		Pause_V -= 0.004f;

		if( g_use == false )
		{
			if( GetKeyboardRelease( DIK_P ) )
			{
				g_use = true;
			}
			if( GetGamePadTrigger( BUTTOM_12 ) )
			{
				g_use = true;
			}
		}
		else if( g_use == true )
		{
			if( GetKeyboardRelease( DIK_P ) )
			{
				g_use = false;
			}
			if( GetGamePadTrigger( BUTTOM_12 ) )
			{
				g_use = false;
			}
		}

		// バッファをロックし、仮想アドレスを取得
		g_pVtxBufferPause -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

		//	ボーズカーソル座標変更
		PauseCursorPos( pVtx );

		//	ポーズスクロール
		PauseScoll( pVtx );

		g_pVtxBufferPause -> Unlock();

		if( g_use == true )
		{
			for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
			{
				if( GamePad[ CntPad ] == NULL )
				{
					//	ボーズカーソル移動
					PauseCursorKeyBoard();
				}
				else if( GamePad[ CntPad ] != NULL )
				{
					//	ゲームパッドカーソル移動
					PauseCursorGamePad();
				}
			}
		}
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawPause( void )
 引数:		
 戻り値:	
 説明:		ポーズの描画
-----------------------------------------------------------------------------*/
void DrawPause( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferPause , 0 , sizeof( VERTEX_2D ) );

	if( g_use == true )
	{
		//	ポーズ背景
		pDevice -> SetTexture( 0 , g_pTexturePauseBg );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);


		//	ポーズフレーム
		pDevice -> SetTexture( 0 , g_pTexturePauseFrame );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);


		//	ポーズスクロール
		pDevice -> SetTexture( 0 , g_pTexturePauseScroll );
		
		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 28 , NUM_POLYGON);

		//	コンティニュー
		pDevice -> SetTexture( 0 , g_pTexturePauseContinue );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12 , NUM_POLYGON);


		//	リトライ
		pDevice -> SetTexture( 0 , g_pTexturePauseRetry );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16 , NUM_POLYGON);


		//	終了
		pDevice -> SetTexture( 0 , g_pTexturePauseQuit );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 20 , NUM_POLYGON);


		//	操作方法
		pDevice -> SetTexture( 0 , g_pTexturePauseOperation );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 24 , NUM_POLYGON);


		//	ポーズカーソル
		pDevice -> SetTexture( 0 , g_pTexturePauseSelect );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8 , NUM_POLYGON);
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		ポーズの描画
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice )
{
	//	エラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_PAUSE_TEXTURENAME , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferPause , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferPause -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	ポーズ背景

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f                  , 0.0f , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH   , 0.0f , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f                  , 0.0f + SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH   , 0.0f + SCREEN_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

	//	ポーズフレーム
	
	//	頂点座標の設定
	pVtx[ 4 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X                  , PAUSE_BG_POS_Y , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X + PAUSE_BG_WIDTH , PAUSE_BG_POS_Y , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X                  , PAUSE_BG_POS_Y + PAUSE_BG_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( PAUSE_BG_POS_X + PAUSE_BG_WIDTH , PAUSE_BG_POS_Y + PAUSE_BG_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 4 ].rhw = 1.0f;
	pVtx[ 5 ].rhw = 1.0f;
	pVtx[ 6 ].rhw = 1.0f;
	pVtx[ 7 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 4 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 5 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 6 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 7 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 4 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 5 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 6 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 7 ].tex = D3DXVECTOR2( 1 , 1 );

	//	ポーズカーソル
	
	//	頂点座標の設定
	pVtx[ 8  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 8  ].rhw = 1.0f;
	pVtx[ 9  ].rhw = 1.0f;
	pVtx[ 10 ].rhw = 1.0f;
	pVtx[ 11 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 8  ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 9  ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 10 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 11 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 8  ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 9  ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 10 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 11 ].tex = D3DXVECTOR2( 1 , 1 );

	//	コンティニュー
	
	//	頂点座標の設定
	pVtx[ 12 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X                        , PAUSE_CONTINUE_POS_Y , 0.0f );
	pVtx[ 13 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X + PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_POS_Y , 0.0f );
	pVtx[ 14 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X                        , PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT , 0.0f );
	pVtx[ 15 ].pos = D3DXVECTOR3( PAUSE_CONTINUE_POS_X + PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 12 ].rhw = 1.0f;
	pVtx[ 13 ].rhw = 1.0f;
	pVtx[ 14 ].rhw = 1.0f;
	pVtx[ 15 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 12 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 13 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 14 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 15 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 12 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 13 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 14 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 15 ].tex = D3DXVECTOR2( 1 , 1 );

	//	リトライ

	//	頂点座標の設定
	pVtx[ 16 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X                     , PAUSE_RETRY_POS_Y , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X + PAUSE_RETRY_WIDTH , PAUSE_RETRY_POS_Y , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X                     , PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( PAUSE_RETRY_POS_X + PAUSE_RETRY_WIDTH , PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 16 ].rhw = 1.0f;
	pVtx[ 17 ].rhw = 1.0f;
	pVtx[ 18 ].rhw = 1.0f;
	pVtx[ 19 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 16 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 17 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 18 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 19 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 17 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 19 ].tex = D3DXVECTOR2( 1 , 1 );

	//	終了

	//	頂点座標の設定
	pVtx[ 20 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X                     , PAUSE_QUIT_POS_Y , 0.0f );
	pVtx[ 21 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X + PAUSE_QUIT_WIDTH  , PAUSE_QUIT_POS_Y , 0.0f );
	pVtx[ 22 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X                     , PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT , 0.0f );
	pVtx[ 23 ].pos = D3DXVECTOR3( PAUSE_QUIT_POS_X + PAUSE_QUIT_WIDTH  , PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 20 ].rhw = 1.0f;
	pVtx[ 21 ].rhw = 1.0f;
	pVtx[ 22 ].rhw = 1.0f;
	pVtx[ 23 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 20 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 21 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 22 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 23 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 20 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 21 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 22 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 23 ].tex = D3DXVECTOR2( 1 , 1 );

	//	操作方法

	//	頂点座標の設定
	pVtx[ 24 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X                          , PAUSE_OPERATION_POS_Y , 0.0f );
	pVtx[ 25 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X + PAUSE_OPERATION_WIDTH  , PAUSE_OPERATION_POS_Y , 0.0f );
	pVtx[ 26 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X                          , PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT , 0.0f );
	pVtx[ 27 ].pos = D3DXVECTOR3( PAUSE_OPERATION_POS_X + PAUSE_OPERATION_WIDTH  , PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 24 ].rhw = 1.0f;
	pVtx[ 25 ].rhw = 1.0f;
	pVtx[ 26 ].rhw = 1.0f;
	pVtx[ 27 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 24 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 25 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 26 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 27 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 24 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 25 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 26 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 27 ].tex = D3DXVECTOR2( 1 , 1 );

	//	ポーズスクロール

	//	頂点座標の設定
	pVtx[ 28 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X                      , PAUSE_SCROLL_POS_Y , 0.0f );
	pVtx[ 29 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X + PAUSE_SCROLL_WIDTH , PAUSE_SCROLL_POS_Y , 0.0f );
	pVtx[ 30 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X                      , PAUSE_SCROLL_POS_Y + PAUSE_SCROLL_HEIGHT , 0.0f );
	pVtx[ 31 ].pos = D3DXVECTOR3( PAUSE_SCROLL_POS_X + PAUSE_SCROLL_WIDTH , PAUSE_SCROLL_POS_Y + PAUSE_SCROLL_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 28 ].rhw = 1.0f;
	pVtx[ 29 ].rhw = 1.0f;
	pVtx[ 30 ].rhw = 1.0f;
	pVtx[ 31 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 28 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 29 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 30 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 31 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

	//	テクスチャ座標の設定
	pVtx[ 28 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 29 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 30 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 31 ].tex = D3DXVECTOR2( 1 , 1 );

	g_pVtxBufferPause -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void PauseCursorMove( void )
 引数:		
 戻り値:	
 説明:		ボーズカーソル移動
-----------------------------------------------------------------------------*/
void PauseCursorKeyBoard( void )
{
	//	上移動	
	if( GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UPARROW) )
	{
		//	現在のカーソルの位置によって位置が変わる

		//	カーソルがリトライにある時
		if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
		{
			//	コンティニューに移動
			g_PauseSelect.Pos.y = PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT * 0.2f;
		}	//	end of if

		//	カーソルが操作方法にある時
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
		{
			//	リトライに移動
			g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;
		}	//	end of if

		//	カーソルが終了にある時
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_QUIT_POS_X , PAUSE_QUIT_POS_Y , PAUSE_QUIT_WIDTH , PAUSE_QUIT_HEIGHT ) )
		{
			//	操作方法に移動
			g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;
		}	//	end of else if

		//	SE再生
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if

	//	下移動
	if(GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWNARROW) )
	{
		//	現在のカーソルの位置によって位置が変わる

		//	カーソルがコンティニューにある時
		if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_CONTINUE_POS_X , PAUSE_CONTINUE_POS_Y , PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_HEIGHT ) )
		{
			//	リトライに移動
			g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;
		}	//	end of if

		//	カーソルがリトライにある時
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
		{
			//	操作方法に移動
			g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;
		}	//	emd of else if

		//	カーソルが操作方法にある時
		else if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
		{
			//	終了に移動
			g_PauseSelect.Pos.y = PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT * 0.2f;
		}	//	end of if

		//	SE再生
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if

	//	画面遷移
	PauseScreenTransition();

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void PauseCursorMove( void )
 引数:		
 戻り値:	
 説明:		ゲームパッドカーソル移動
-----------------------------------------------------------------------------*/
void PauseCursorGamePad( void )
{
	//	スティックの取得
	LONG Stick_Y = GetGamePadStick_Y();

	CntPuseFrame++;

	if( CntPuseFrame % DELAY_PAD == 0 )
	{
		//	上移動	
		if( Stick_Y < -999 )
		{
			g_PadKeyFlag = true;

			if( g_PadKeyFlag == true )
			{
				//	現在のカーソルの位置によって位置が変わる

				//	カーソルが終了にある時
				if( g_Quit == true )
				{
					//	操作方法に移動
					g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;

					g_Quit = false;
				}

				//	カーソルが操作方法にある時
				else if( g_Operation == true )
				{
					//	リトライに移動
					g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;

					g_Operation = false;
				}

				//	カーソルがリトライにある時
				else if( g_Retry == true )
				{
					//	コンティニューに移動
					g_PauseSelect.Pos.y = PAUSE_CONTINUE_POS_Y + PAUSE_CONTINUE_HEIGHT * 0.2f;

					g_Retry = false;
				}
			}

			//	SE再生
			PlaySound( SOUND_LABEL_SE_SERECT );
		}	//	end of if

		//	下移動
		if( Stick_Y > 999 )
		{
			g_PadKeyFlag = false;

			if( g_PadKeyFlag == false )
			{
				//	現在のカーソルの位置によって位置が変わる

				//	カーソルがリトライにある時
				if( g_Retry == false )
				{
					//	リトライに移動
					g_PauseSelect.Pos.y = PAUSE_RETRY_POS_Y + PAUSE_RETRY_HEIGHT * 0.2f;

					g_Retry = true;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	カーソルが操作方法にある時
				else if( g_Operation == false )
				{
					//	操作方法に移動
					g_PauseSelect.Pos.y = PAUSE_OPERATION_POS_Y + PAUSE_OPERATION_HEIGHT * 0.2f;

					g_Operation = true;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	カーソルが終了にある時
				else if( g_Quit == false )
				{
					//	コンティニューに移動
					g_PauseSelect.Pos.y = PAUSE_QUIT_POS_Y + PAUSE_QUIT_HEIGHT * 0.2f;

					g_Quit = true;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}	//	end of else if
			}	//	end of if
		}	//	end of if
	}	//	end of if

	//	ゲームパッドの画面遷移
	GamePadScreenTransition();

}
/*-----------------------------------------------------------------------------
 関数名:	void PauseScreenTransition( void )
 引数:		
 戻り値:	
 説明:		画面遷移
-----------------------------------------------------------------------------*/
void PauseScreenTransition( void )
{
	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	コンティニュー
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_CONTINUE_POS_X , PAUSE_CONTINUE_POS_Y , PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_use = false;
		}	//	end of if
	}	//	end of if

	//	リトライ
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			if( *Mode == MODE_GAME )
			{
				SetFade( FADE_OUT , MODE_GAME );
			}
			else if( *Mode == MODE_TUTORIAL )
			{
				SetFade( FADE_OUT , MODE_TUTORIAL );
			}

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_GAME );

		}	//	end of if
	}	//	end of if

	//	操作方法
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_OperationFlag = true;

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if

	//	終了
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_QUIT_POS_X , PAUSE_QUIT_POS_Y , PAUSE_QUIT_WIDTH , PAUSE_QUIT_HEIGHT ) )
	{
		if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			SetFade( FADE_OUT , MODE_TITLE );

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_GAME );

		}	//	end of if
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void GamePadScreenTransition( void )
 引数:		
 戻り値:	
 説明:		ゲームパッドの画面遷移
-----------------------------------------------------------------------------*/
void GamePadScreenTransition( void )
{
	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	コンティニュー
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_CONTINUE_POS_X , PAUSE_CONTINUE_POS_Y , PAUSE_CONTINUE_WIDTH , PAUSE_CONTINUE_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_use = false;
		}	//	end of if
	}	//	end of if

	//	リトライ
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_RETRY_POS_X , PAUSE_RETRY_POS_Y , PAUSE_RETRY_WIDTH , PAUSE_RETRY_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			if( *Mode == MODE_GAME )
			{
				SetFade( FADE_OUT , MODE_GAME );
			}
			else if( *Mode == MODE_TUTORIAL )
			{
				SetFade( FADE_OUT , MODE_TUTORIAL );
			}

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if

	//	操作方法
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_OPERATION_POS_X , PAUSE_OPERATION_POS_Y , PAUSE_OPERATION_WIDTH , PAUSE_OPERATION_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_OperationFlag = true;

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if

	//	終了
	if( RectangleDecision( g_PauseSelect.Pos.x , g_PauseSelect.Pos.y , g_PauseSelect.Size.x , g_PauseSelect.Size.y , PAUSE_QUIT_POS_X , PAUSE_QUIT_POS_Y , PAUSE_QUIT_WIDTH , PAUSE_QUIT_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			SetFade( FADE_OUT , MODE_TITLE );

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

		}	//	end of if
	}	//	end of if
}
/*-----------------------------------------------------------------------------
 関数名:	void PauseCursorPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	
 説明:		ボーズカーソル座標変更
-----------------------------------------------------------------------------*/
void PauseCursorPos( VERTEX_2D* pVtx )
{
	//	頂点座標の設定
	pVtx[ 8  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x                        , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( g_PauseSelect.Pos.x + g_PauseSelect.Size.x , g_PauseSelect.Pos.y + g_PauseSelect.Size.y , 0.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void PauseScoll( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	
 説明:		ボーズスクロール
-----------------------------------------------------------------------------*/
void PauseScoll( VERTEX_2D* pVtx )
{
	//	テクスチャ座標の設定
	pVtx[ 28 ].tex = D3DXVECTOR2( 0 + Pause_U , 0 + Pause_V );
	pVtx[ 29 ].tex = D3DXVECTOR2( 1 + Pause_U , 0 + Pause_V );
	pVtx[ 30 ].tex = D3DXVECTOR2( 0 + Pause_U , 1 + Pause_V );
	pVtx[ 31 ].tex = D3DXVECTOR2( 1 + Pause_U , 1 + Pause_V );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	bool *GetPause( void )
 引数:		
 戻り値:	
 説明:		ボーズしているか
-----------------------------------------------------------------------------*/
bool *GetPause( void )
{
	return &g_use;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	bool *GetPause( void )
 引数:		
 戻り値:	
 説明:		ボーズしているか
-----------------------------------------------------------------------------*/
bool *GetOpeFlag( void )
{
	return &g_OperationFlag;
}