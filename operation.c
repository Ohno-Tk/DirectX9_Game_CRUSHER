/*=============================================================================

		操作方法[ operation.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/09/21
-------------------------------------------------------------------------------
	■　作成日
		2016/09/21
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "operation.h"
#include "result.h"
#include "pause.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define OPERATION_KEYBOARD_TEXTURENAME "data/TEXTURE/operation/操作方法01.jpg"
#define OPERATION_GAMEPAD_TEXTURENAME "data/TEXTURE/operation/操作方法02.jpg"
#define OPERATION_BUTTOM_TEXTURENAME "data/TEXTURE/func/ButtomRogo .png"
#define OPERATION_PAD_TEXTURENAME "data/TEXTURE/func/ButtomPad.png"

#define MAX_OPERATION_TEXTURE ( 2 )
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexOperation( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void ButtomFade( VERTEX_2D* pVtx );	//	ボタンのフェード
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferOperation = NULL;

//	キーボード操作
LPDIRECT3DTEXTURE9 g_pTextureOperation_Keyboard = NULL;//	テクスチャインターフェース

//	ゲームパッド
LPDIRECT3DTEXTURE9 g_pTextureOperation_GamePad = NULL;//	テクスチャインターフェース

//	ボタン
LPDIRECT3DTEXTURE9 g_pTextureOperation_Buttom = NULL;//	テクスチャインターフェース

FADE g_OperationFade = FADE_IN;	//	フェード状態
D3DXCOLOR g_OperationFadeColor;	//	フェード色

/*-----------------------------------------------------------------------------
 関数名:	void InitOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		操作方法の初期化
-----------------------------------------------------------------------------*/
void InitOperation( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	エラーチェック

	//	ゲームパッドが無かったら
	if( GamePad[ 0 ] == NULL )
	{
		//	キーボード
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_KEYBOARD_TEXTURENAME , &g_pTextureOperation_Keyboard  ) ) )
		{
			MessageBox( NULL , "操作方法０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if

		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_BUTTOM_TEXTURENAME , &g_pTextureOperation_Buttom  ) ) )
		{
			MessageBox( NULL , "操作ボタン０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	ゲームパッドがあったら
	else if( GamePad[ 0 ] != NULL )
	{
		//	ゲームパッド
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_GAMEPAD_TEXTURENAME , &g_pTextureOperation_Keyboard  ) ) )
		{
			MessageBox( NULL , "操作方法０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if

		if( FAILED( D3DXCreateTextureFromFile(  pDevice , OPERATION_PAD_TEXTURENAME , &g_pTextureOperation_Buttom  ) ) )
		{
			MessageBox( NULL , "操作ボタン０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	

	//	初期化
	g_OperationFadeColor = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	頂点の作成
	MakeVertexOperation( pDevice );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		操作方法の終了
-----------------------------------------------------------------------------*/
void UninitOperation( void )
{
	if( g_pVtxBufferOperation != NULL )
	{
		g_pVtxBufferOperation -> Release();
		g_pVtxBufferOperation = NULL;
	}	//	end of if

	//	操作方法０１
	if( g_pTextureOperation_Keyboard != NULL )
	{
		g_pTextureOperation_Keyboard -> Release();
		g_pTextureOperation_Keyboard = NULL;
	}	//	end of if

	//	ボタン
	if( g_pTextureOperation_Buttom != NULL )
	{
		g_pTextureOperation_Buttom -> Release();
		g_pTextureOperation_Buttom = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		操作方法の更新
-----------------------------------------------------------------------------*/
void UpdateOperation( void )
{
	//	ポーズ使用フラグ取得
	bool *PauseUse = GetOpeFlag();
	bool *Pause = GetPause();

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferOperation -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	ボタンのフェード
	ButtomFade( pVtx );

	g_pVtxBufferOperation -> Unlock();

	if( GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN) )
	{
		*PauseUse = false;
		//*Pause = true;
	}

	if( GetGamePadTrigger( BUTTOM_04 ) )
	{
		*PauseUse = false;
		//*Pause = true;
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		操作方法の描画
-----------------------------------------------------------------------------*/
void DrawOperation( void )
{
	//	ポーズ使用フラグ取得
	bool *PauseUse = GetOpeFlag();

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferOperation , 0 , sizeof( VERTEX_2D ) );

	if( *PauseUse ==true )
	{
		//	操作方法０１
		pDevice -> SetTexture( 0 , g_pTextureOperation_Keyboard );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

		//	操作方法０２
		pDevice -> SetTexture( 0 , g_pTextureOperation_GamePad );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);

		//	ボタン
		pDevice -> SetTexture( 0 , g_pTextureOperation_Buttom );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
	}


}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexOperation( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexOperation( LPDIRECT3DDEVICE9 pDevice )
{

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_OPERATION_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferOperation , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferOperation -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	ボタン

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

	//	操作方法０１

	//	頂点座標の設定
	pVtx[ 4 ].pos = D3DXVECTOR3( 0.0f                , 0.0f                 , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f                 , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( 0.0f                , 0.0f + SCREEN_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f + SCREEN_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 4 ].rhw = 1.0f;
	pVtx[ 5 ].rhw = 1.0f;
	pVtx[ 6 ].rhw = 1.0f;
	pVtx[ 7 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 4 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );
	pVtx[ 5 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );
	pVtx[ 6 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );
	pVtx[ 7 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 250 );

	//	テクスチャ座標の設定
	pVtx[ 4 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 5 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 6 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 7 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferOperation -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void ButtomFade( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		ボタンのフェード
-----------------------------------------------------------------------------*/
void ButtomFade( VERTEX_2D* pVtx )
{
	if( g_OperationFade == FADE_IN )	//	フェードイン時の処理
	{
		g_OperationFadeColor.a -= FADE_RATE;	//	a値を減算して後ろの画面を浮かび上がらせる
		//	フェードインの終了
		if( g_OperationFadeColor.a < 0.0f )
		{
			g_OperationFadeColor.a = 0.0f;
			g_OperationFade = FADE_OUT;
		}	//	end od if
	}	//	end of if

	else if( g_OperationFade == FADE_OUT )	//	フェードアウト時の処理
	{
		g_OperationFadeColor.a += FADE_RATE;	//	a値を加算して後ろの画面を消していく
		//	フェードアウトの終了
		if( g_OperationFadeColor.a > 1.0f )
		{
			g_OperationFadeColor.a = 1.0f;
			g_OperationFade = FADE_IN;	//	フェードインに処理の切り替え
		}	//	end of if
	}	//	end of else if

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_OperationFadeColor.r , g_OperationFadeColor.g , g_OperationFadeColor.b , g_OperationFadeColor.a );

}