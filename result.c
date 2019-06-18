/*=============================================================================

		リザルト[ result.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/06/29
-------------------------------------------------------------------------------
	■　作成日
		2016/08/23
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "result.h"
#include "fade.h"
#include "player.h"
#include "base.h"
#include "time.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define RESULT_TEXTURENAME "data/TEXTURE/result/result.jpg"

#define RESULT_PUSH_TEXTURENAME "data/TEXTURE/func/ButtomRogo .png"

#define RESULT_CLEAR_TEXTURENAME "data/TEXTURE/result/clear.png"

#define RESULT_GAMEOVER_TEXTURENAME "data/TEXTURE/result/gameover.png"

#define RESULT_PLAYER_TEXTURENAME "data/TEXTURE/game/player2.png"

#define RESULT_PAD_TEXTURENAME "data/TEXTURE/func/ButtomPad.png"


#define MAX_RESULT_TEXTURE ( 5 )	//	テクスチャの最大数

#define RESULT_POS_X ( 0.0f )		//	ポリゴンの表示位置Ｘ
#define RESULT_POS_Y ( 0.0f )		//	ポリゴンの表示位置Ｙ
#define RESULT_WIDTH ( 800.0f )		//	ポリゴンの幅
#define RESULT_HEIGHT ( 600.0f )	//	ポリゴンの高さ

#define RESULT_CLEAR_POS_X ( 250.0f )	//	クリアPosX
#define RESULT_CLEAR_POS_Y ( 0.0f )		//	クリアPosY
#define RESULT_CLEAR_WIDTH ( 500.0f )	//	クリアWIDTH
#define RESULT_CLEAR_HEIGHT ( 250.0f )	//	クリアHEIGHT

#define RESULT_GAMEOVER_POS_X ( 200.0f )	//	ゲームオーバーPosX
#define RESULT_GAMEOVER_POS_Y ( 0.0f )		//	ゲームオーバーPosY
#define RESULT_GAMEOVER_WIDTH ( 500.0f )	//	ゲームオーバーWIDTH
#define RESULT_GAMEOVER_HEIGHT ( 250.0f )	//	ゲームオーバーHEIGHT

#define RESULT_PLAYER_POS_X ( 300.0f )	//	プレイヤーPosX
#define RESULT_PLAYER_POS_Y ( 200.0f )	//	プレイヤーPosY
#define RESULT_PLAYER_WIDTH ( 400.0f )	//	プレイヤーWIDTH
#define RESULT_PLAYER_HEIGHT ( 300.0f )	//	プレイヤーHEIGHT

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexresult( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void MakeVertexTex( VERTEX_2D* pVtx );	//	テクスチャ変更
void PushBottomFade( VERTEX_2D* pVtx );	//	プッシュボタンのフェード
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferresult = NULL;

//	リザルト背景
LPDIRECT3DTEXTURE9 g_pTextureresult = NULL;//	テクスチャインターフェース

//	Pushボタン
LPDIRECT3DTEXTURE9 g_pTextureResult_Push = NULL;//	テクスチャインターフェース

//	クリア
LPDIRECT3DTEXTURE9 g_pTextureResult_Clear = NULL;//	テクスチャインターフェース

//	ゲームオーバー
LPDIRECT3DTEXTURE9 g_pTextureResult_Gameover = NULL;//	テクスチャインターフェース

//	プレイヤー
LPDIRECT3DTEXTURE9 g_pTextureResult_Player = NULL;//	テクスチャインターフェース

FADE g_ResultFade = FADE_IN;	//	フェード状態
D3DXCOLOR g_ResultFadeColor;	//	フェード色

float Result_Uset = 0.0f , Result_Vset = 0.0f;
float Result_Bg_Uset = 0.0f , Result_Bg_Vset = 0.0f;
/*-----------------------------------------------------------------------------
 関数名:	void InitResult( void )
 引数:		なし
 戻り値:	なし
 説明:		リザルトの初期化
-----------------------------------------------------------------------------*/
void InitResult( void )
{
	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_TEXTURENAME , &g_pTextureresult  ) ) )
	{
		MessageBox( NULL , "リザルト背景の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_PLAYER_TEXTURENAME , &g_pTextureResult_Player  ) ) )
	{
		MessageBox( NULL , "リザルトのプレイヤーの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	ゲームパッドが無かったら
	if( GamePad[ 0 ] == NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_PUSH_TEXTURENAME , &g_pTextureResult_Push  ) ) )
		{
			MessageBox( NULL , "Pushボタン０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	ゲームパッドがあったら
	else if( GamePad[ 0 ] != NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_PAD_TEXTURENAME , &g_pTextureResult_Push  ) ) )
		{
			MessageBox( NULL , "Pushボタン０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	ゴールしたか
	if( Base -> GoolFalg == true )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_CLEAR_TEXTURENAME , &g_pTextureResult_Clear  ) ) )
		{
			MessageBox( NULL , "クリアの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}	//	end of if

	else if( Base -> GoolFalg == false )
	{
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , RESULT_GAMEOVER_TEXTURENAME , &g_pTextureResult_Gameover  ) ) )
		{
			MessageBox( NULL , "ゲームオーバーの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}	//	end of else if

	MakeVertexresult( pDevice );	//	頂点の作成

	//	初期化
	Result_Uset = 0.0f;
	Result_Vset = 0.0f;

	Result_Bg_Uset = 0.0f;
	Result_Bg_Vset = 0.0f;

	g_ResultFadeColor = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	音楽再生
	PlaySound( SOUND_LABEL_BGM_RESULT );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitResult( void )
 引数:		なし
 戻り値:	なし
 説明:		リザルトの終了
-----------------------------------------------------------------------------*/
void UninitResult( void )
{
	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	Base -> GoolFalg = false;

	//	背景

	if( g_pTextureresult != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureresult -> Release();
		g_pTextureresult = NULL;
	}	//	end of if
	if( g_pVtxBufferresult != NULL )
	{
		g_pVtxBufferresult -> Release();
		g_pVtxBufferresult = NULL;
	}	//	end of if

	//	Pushボタン
	if( g_pTextureResult_Push != NULL )
	{
		g_pTextureResult_Push -> Release();
		g_pTextureResult_Push = NULL;
	}	//	end of if

	//	プレイヤー
	if( g_pTextureResult_Player != NULL )
	{
		g_pTextureResult_Player -> Release();
		g_pTextureResult_Player = NULL;
	}	//	end of if

	//	クリア
	if( g_pTextureResult_Clear != NULL )
	{
		g_pTextureResult_Clear -> Release();
		g_pTextureResult_Clear = NULL;
	}	//	end of if

	//	ゲームオーバー
	if( g_pTextureResult_Gameover != NULL )
	{
		g_pTextureResult_Gameover -> Release();
		g_pTextureResult_Gameover = NULL;
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdeteResult( void )
 引数:		なし
 戻り値:	なし
 説明:		リザルトの更新
-----------------------------------------------------------------------------*/
void UpdateResult( void )
{
	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	//	今のモード
	MODE *Mode = GetMode();

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	フェード中かどうか
	bool *FadeUse = GetFadeUse();


	//	ゴールしたか
	if( Base -> GoolFalg == true )
	{
		Result_Uset = 0.5f;
		Result_Vset = 0.5f;
	}	//	end of if
	else if( Base -> GoolFalg == false )
	{
		Result_Uset = 0.0f;
		Result_Vset = 0.5f;
	}	//	end of else if

	Result_Bg_Uset += 0.004f;
	Result_Bg_Vset -= 0.004f;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferresult -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	MakeVertexTex( pVtx );	//	テクスチャ変更
	
	PushBottomFade(pVtx );	//	プッシュボタンのフェード

	g_pVtxBufferresult -> Unlock();

	if( *FadeUse == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
		{
			//	画面遷移
			if( GamePad[ CntPad ] != NULL )
			{
				if( GetGamePadTrigger( BUTTOM_04 ) )
				{
					//	モードがゲームの時
					if( *Mode == MODE_GAME )
					{
						SetFade(FADE_OUT , MODE_RANKING);

						//	SE再生
						PlaySound( SOUND_LABEL_SE_ENTER );

						//	音楽停止
						StopSound( SOUND_LABEL_BGM_RESULT );
					}	//	end of if

					//	モードがチュートリアルの時
					else if( *Mode == MODE_TUTORIAL )
					{
						SetFade(FADE_OUT , MODE_TITLE);
					}	//	end of else if

					//	SE再生
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	音楽停止
					StopSound( SOUND_LABEL_BGM_RESULT );

				}		//	end of if
			}	//	end of if
			else if( GamePad[ CntPad ] == NULL )
			{

				if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
				{

					//	モードがゲームの時
					if( *Mode == MODE_GAME )
					{
						SetFade(FADE_OUT , MODE_RANKING);

						//	SE再生
						PlaySound( SOUND_LABEL_SE_ENTER );

						//	音楽停止
						StopSound( SOUND_LABEL_BGM_RESULT );

					}	//	end of if

					//	モードがチュートリアルの時
					else if( *Mode == MODE_TUTORIAL )
					{
						SetFade(FADE_OUT , MODE_TITLE);
				
					}	//	end of else if

					//	SE再生
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	音楽停止
					StopSound( SOUND_LABEL_BGM_RESULT );

				}	//	end of if
			}	//	end of else if
		}	//	end of for
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawResult( void )
 引数:		なし
 戻り値:	なし
 説明:		リザルトの描画
-----------------------------------------------------------------------------*/
void DrawResult( void )
{
	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferresult , 0 , sizeof( VERTEX_2D ) );

	//	リザルト背景
	pDevice -> SetTexture( 0 , g_pTextureresult );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

	//	プレイヤー
	pDevice -> SetTexture( 0 , g_pTextureResult_Player );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16, NUM_POLYGON);


	//	ゴールしたか
	if( Base -> GoolFalg == true )
	{
		//	クリア
		pDevice -> SetTexture( 0 , g_pTextureResult_Clear );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);
	}
	else if( Base -> GoolFalg == false )
	{
		//	ゲームオーバー
		pDevice -> SetTexture( 0 , g_pTextureResult_Gameover );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12, NUM_POLYGON);
	}

	//	Pushボタン
	pDevice -> SetTexture( 0 , g_pTextureResult_Push );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexresult( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexresult( LPDIRECT3DDEVICE9 pDevice )
{

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_RESULT_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferresult , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferresult -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	背景

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_POS_X                , RESULT_POS_Y                 , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_POS_X + SCREEN_WIDTH , RESULT_POS_Y                 , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_POS_X                , RESULT_POS_Y + SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_POS_X + SCREEN_WIDTH , RESULT_POS_Y + SCREEN_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 200 );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

	//	Pushボタン

	//	頂点座標の設定
	pVtx[ 4 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y                          , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , RESULT_PUSH_POS_Y + RESULT_PUSH_POS_HEIGHT , 0.0f );

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

	//	クリア

	//	頂点座標の設定
	pVtx[ 8  ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X                      , RESULT_CLEAR_POS_Y                       , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X + RESULT_CLEAR_WIDTH , RESULT_CLEAR_POS_Y                       , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X                      , RESULT_CLEAR_POS_Y + RESULT_CLEAR_HEIGHT , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( RESULT_CLEAR_POS_X + RESULT_CLEAR_WIDTH , RESULT_CLEAR_POS_Y + RESULT_CLEAR_HEIGHT , 0.0f );

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

	//	ゲームオーバー

	//	頂点座標の設定
	pVtx[ 12 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X                         , RESULT_GAMEOVER_POS_Y                          , 0.0f );
	pVtx[ 13 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X + RESULT_GAMEOVER_WIDTH , RESULT_GAMEOVER_POS_Y                          , 0.0f );
	pVtx[ 14 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X                         , RESULT_GAMEOVER_POS_Y + RESULT_GAMEOVER_HEIGHT , 0.0f );
	pVtx[ 15 ].pos = D3DXVECTOR3( RESULT_GAMEOVER_POS_X + RESULT_GAMEOVER_WIDTH , RESULT_GAMEOVER_POS_Y + RESULT_GAMEOVER_HEIGHT , 0.0f );

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

	//	プレイヤー

	//	頂点座標の設定
	pVtx[ 16 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X                       , RESULT_PLAYER_POS_Y                          , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X + RESULT_PLAYER_WIDTH , RESULT_PLAYER_POS_Y                          , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X                       , RESULT_PLAYER_POS_Y + RESULT_PLAYER_HEIGHT , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( RESULT_PLAYER_POS_X + RESULT_PLAYER_WIDTH , RESULT_PLAYER_POS_Y + RESULT_PLAYER_HEIGHT , 0.0f );

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

	g_pVtxBufferresult -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexTex( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		テクスチャ変更
-----------------------------------------------------------------------------*/
void MakeVertexTex( VERTEX_2D* pVtx )
{
	//	リザルト背景

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 + Result_Bg_Uset , 0 + Result_Bg_Vset );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 + Result_Bg_Uset , 0 + Result_Bg_Vset );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 + Result_Bg_Uset , 1 + Result_Bg_Vset );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 + Result_Bg_Uset , 1 + Result_Bg_Vset );

	//	リザルト

	//	テクスチャ座標の設定
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 + Result_Uset         , 0 + Result_Vset );
	pVtx[ 17 ].tex = D3DXVECTOR2( TEX_WIDTH + Result_Uset , 0 + Result_Vset );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 + Result_Uset         , TEX_HEIGHT + Result_Vset );
	pVtx[ 19 ].tex = D3DXVECTOR2( TEX_WIDTH + Result_Uset , TEX_HEIGHT + Result_Vset );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void PushBottomFade( VERTEX_2D* pVtx; )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		プッシュボタンのフェード
-----------------------------------------------------------------------------*/
void PushBottomFade( VERTEX_2D* pVtx )
{
	if( g_ResultFade == FADE_IN )	//	フェードイン時の処理
	{
		g_ResultFadeColor.a -= FADE_RATE;	//	a値を減算して後ろの画面を浮かび上がらせる
		//	フェードインの終了
		if( g_ResultFadeColor.a < 0.0f )
		{
			g_ResultFadeColor.a = 0.0f;
			g_ResultFade = FADE_OUT;
		}	//	end od if
	}	//	end of if

	else if( g_ResultFade == FADE_OUT )	//	フェードアウト時の処理
	{
		g_ResultFadeColor.a += FADE_RATE;	//	a値を加算して後ろの画面を消していく
		//	フェードアウトの終了
		if( g_ResultFadeColor.a > 1.0f )
		{
			g_ResultFadeColor.a = 1.0f;
			g_ResultFade = FADE_IN;	//	フェードインに処理の切り替え
		}	//	end of if
	}	//	end of else if

	//	頂点色の設定
	pVtx[ 4 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
	pVtx[ 5 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
	pVtx[ 6 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
	pVtx[ 7 ].color = D3DXCOLOR( g_ResultFadeColor.r , g_ResultFadeColor.g , g_ResultFadeColor.b , g_ResultFadeColor.a );
}	//	end of func