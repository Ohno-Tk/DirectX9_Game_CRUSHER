/*=============================================================================

		タイトル[ Title.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/08/02
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "title.h"
#include "fade.h"
#include "usability.h"
#include "sound.h"
#include "cloud.h"
#include "base.h"
#include "bg.h"
#include "enemy.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define TITLE_TEXTURENAME "data/TEXTURE/title/title.png"			//	タイトルテクスチャ
#define START_TEXTURENAME "data/TEXTURE/title/start.png"					//	スタートロゴテクスチャ
#define TUTORIAL_TEXTURENAME "data/TEXTURE/title/tutorial.png"			//	チュートリアルロゴテクスチャ
#define SERECT_TEXTURENAME "data/TEXTURE/title/player.png"					//	矢印テクスチャ
#define RANKING_TEXTURENAME "data/TEXTURE/title/ranking.png"			//	ランキングロゴテクスチャ

#define MAX_TITLE_TEXTURE ( 6 )	//	最大テクスチャ

#define TITLE_BG_POS_X ( 0.0f )	//	タイトル背景の表示位置Ｘ
#define TITLE_BG_POS_Y ( 0.0f )	//	タイトル背景の表示位置Ｙ

#define TITLE_POS_Y ( 100.0f )	//	タイトルの表示位置Ｙ

#define START_POS_X ( 375.0f )	//	スタートの表示位置Ｘ
#define START_POS_Y ( 250.0f )	//	スタートの表示位置Ｙ
#define START_WIDTH ( 200.0f )	//	スタートの幅
#define START_HEIGHT ( 100.0f )	//	スタートの高さ

#define TUTORIAL_POS_X ( START_POS_X )	//	チュートリアルの表示位置Ｘ
#define TUTORIAL_POS_Y ( START_POS_Y + 100.0f )		//	チュートリアルの表示位置Ｙ
#define TUTORIAL_WIDTH ( 200.0f )		//	チュートリアルの幅
#define TUTORIAL_HEIGHT ( 100.0f )		//	チュートリアルの高さ

#define RANKING_POS_X ( START_POS_X )	//	ランキングの表示位置Ｘ
#define RANKING_POS_Y ( START_POS_Y + 200.0f )		//	ランキングの表示位置Ｙ
#define RANKING_WIDTH ( 200.0f )		//	ランキングの幅
#define RANKING_HEIGHT ( 100.0f )		//	ランキングの高さ

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertextitle( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void MakeVerTexSerectPos( VERTEX_2D* pVtx );			//	矢印座標更新
void ScreenTransition( void );	//	画面遷移
void CursorMove( void );	//	カーソル移動
void GamePadCursorMove( void );	//	ゲームパッドのカーソル移動
void GamePadTitleScreenTransition( void );	//	ゲームパッドの画面遷移
void TitleRogoPos( VERTEX_2D* pVtx );	//	タイトルの座標変更
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitle = NULL;	//頂点バッファのインターフェースポインタ
//	タイトルロゴ
LPDIRECT3DTEXTURE9 g_pTexturetitle = NULL;//	テクスチャインターフェース
//	スタートロゴ
LPDIRECT3DTEXTURE9 g_pTexturestart = NULL;//	テクスチャインターフェース
//	チュートリアルロゴ
LPDIRECT3DTEXTURE9 g_pTextureTutorialRogo = NULL;//	テクスチャインターフェース
//	矢印
LPDIRECT3DTEXTURE9 g_pTextureSerect = NULL;//	テクスチャインターフェース
//	ランキングロゴ
LPDIRECT3DTEXTURE9 g_pTextureRankingRogo = NULL;//	テクスチャインターフェース

TITLE g_serect;

TITLE g_TitleRogo;

float g_Offset = 0.0f;

int CntFrame = 0;

bool KeyFlag = false;

bool g_RankingFlag = false;
bool g_TutorialFlag = false;

/*-----------------------------------------------------------------------------
 関数名:	void InitTitle( void )
 引数:		なし
 戻り値:	なし
 説明:		タイトルの初期化
-----------------------------------------------------------------------------*/
void InitTitle( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック

	//	タイトルロゴ
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , TITLE_TEXTURENAME , &g_pTexturetitle  ) ) )
	{
		MessageBox( NULL , "タイトルロゴの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	スタートロゴ
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , START_TEXTURENAME , &g_pTexturestart  ) ) )
	{
		MessageBox( NULL , "スタートロゴの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	チュートリアルロゴ
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_TEXTURENAME , &g_pTextureTutorialRogo  ) ) )
	{
		MessageBox( NULL , "チュートリアルロゴの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	矢印
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , SERECT_TEXTURENAME , &g_pTextureSerect  ) ) )
	{
		MessageBox( NULL , "矢印の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	ランキングロゴ
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , RANKING_TEXTURENAME , &g_pTextureRankingRogo  ) ) )
	{
		MessageBox( NULL , "ランキングロゴの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	初期化
	//	カーソル
	g_serect.Pos.x = START_POS_X - START_WIDTH * 0.4f;
	g_serect.Pos.y = START_POS_Y + START_HEIGHT * 0.1f;
	g_serect.Size.x = START_WIDTH * 0.5f;
	g_serect.Size.y = START_HEIGHT*0.6f;

	//	タイトルロゴ
	g_TitleRogo.Pos.x = 200.0f;
	g_TitleRogo.Pos.y = -20.0f;
	g_TitleRogo.Size.x = 600.0f;
	g_TitleRogo.Size.y = 300.0f;
	g_TitleRogo.move.y = 1.0f;

	g_Offset = 0.0f;

	CntFrame = 0;

	KeyFlag = false;

	g_RankingFlag = false;
	g_TutorialFlag = false;

	InitBg();	//	背景の初期化

	InitCloud();	//	雲の初期化

	InitBase();	//	足場の初期化

	InitEnemy();	//	敵の初期化

	MakeVertextitle( pDevice );	//	頂点の作成

	//	音楽再生
	PlaySound( SOUND_LABEL_BGM_TITLE );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitTitle( void )
 引数:		なし
 戻り値:	なし
 説明:		タイトルの終了
-----------------------------------------------------------------------------*/
void UninitTitle( void )
{

	if( g_pVtxBufferTitle != NULL )
	{
		g_pVtxBufferTitle -> Release();
		g_pVtxBufferTitle = NULL;
	}

	//	タイトルロゴ
	if( g_pTexturetitle != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturetitle -> Release();
		g_pTexturetitle = NULL;
	}	//	end of if

	//	スタートロゴ
	if( g_pTexturestart != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturestart -> Release();
		g_pTexturestart = NULL;
	}	//	end of if

	//	チュートリアルロゴ
	if( g_pTextureTutorialRogo != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTutorialRogo -> Release();
		g_pTextureTutorialRogo = NULL;
	}	//	end of if

	//	矢印
	if( g_pTextureSerect != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureSerect -> Release();
		g_pTextureSerect = NULL;
	}	//	end of if

	//	ランキングロゴ
	if( g_pTextureRankingRogo != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureRankingRogo -> Release();
		g_pTextureRankingRogo = NULL;
	}	//	end of if




	UninitBg();	//	背景の終了

	UninitCloud();	//	雲の終了

	UninitBase();	//	足場の終了

	UninitEnemy();	//	敵の終了


}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdeteTitle( void )
 引数:		なし
 戻り値:	なし
 説明:		タイトルの更新
-----------------------------------------------------------------------------*/
void UpdateTitle( void )
{
	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	フェード中かどうか
	bool *FadeUse = GetFadeUse();

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	UpdateBg();	//	背景の更新

	UpdateCloud();	//	雲の更新

	UpdateBase();	//	足場の更新

	UpdateEnemy();	//	敵の更新

	CntFrame++;	

	if( *FadeUse == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
		{
			if( GamePad[ CntPad ] == NULL )
			{
				//	カーソル移動
				CursorMove();
			}
			if( GamePad[ CntPad ] != NULL )
			{
				
				//	ゲームパッドのカーソル移動
				GamePadCursorMove();
			}

		}
	}


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTitle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	矢印座標更新
	MakeVerTexSerectPos( pVtx );

	//	タイトルの座標変更
	TitleRogoPos( pVtx );

	g_pVtxBufferTitle -> Unlock();

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawTitle( void )
 引数:		なし
 戻り値:	なし
 説明:		タイトルの描画
-----------------------------------------------------------------------------*/
void DrawTitle( void )
{
	DrawBg();	//	背景の描画

	DrawCloud();	//	雲の描画

	DrawBase();		//	足場の描画

	DrawEnemy();	//	敵の描画

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	タイトル背景ロゴ
	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTitle , 0 , sizeof( VERTEX_2D ) );

	//	スタートロゴ
	pDevice -> SetTexture( 0 , g_pTexturestart );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

	//	チュートリアルロゴ
	pDevice -> SetTexture( 0 , g_pTextureTutorialRogo );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);

	//	ランキングロゴ
	pDevice -> SetTexture( 0 , g_pTextureRankingRogo );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12, NUM_POLYGON);

	//	矢印
	pDevice -> SetTexture( 0 , g_pTextureSerect );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16, NUM_POLYGON);

	//	タイトルロゴ
	pDevice -> SetTexture( 0 , g_pTexturetitle );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertextitle( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertextitle( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TITLE_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTitle , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;
	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTitle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	タイトルロゴ
	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );

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

	//	スタートロゴ
	//	頂点座標の設定
	pVtx[ 4 ].pos = D3DXVECTOR3( START_POS_X                , START_POS_Y , 0.0f );
	pVtx[ 5 ].pos = D3DXVECTOR3( START_POS_X + START_WIDTH  , START_POS_Y , 0.0f );
	pVtx[ 6 ].pos = D3DXVECTOR3( START_POS_X               , START_POS_Y + START_HEIGHT , 0.0f );
	pVtx[ 7 ].pos = D3DXVECTOR3( START_POS_X + START_WIDTH , START_POS_Y + START_HEIGHT , 0.0f );

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

	//	チュートリアルロゴ
	//	頂点座標の設定
	pVtx[ 8  ].pos = D3DXVECTOR3( TUTORIAL_POS_X                   , TUTORIAL_POS_Y , 0.0f );
	pVtx[ 9  ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH  , TUTORIAL_POS_Y , 0.0f );
	pVtx[ 10 ].pos = D3DXVECTOR3( TUTORIAL_POS_X                   , TUTORIAL_POS_Y + TUTORIAL_HEIGHT , 0.0f );
	pVtx[ 11 ].pos = D3DXVECTOR3( TUTORIAL_POS_X + TUTORIAL_WIDTH  , TUTORIAL_POS_Y + TUTORIAL_HEIGHT , 0.0f );

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

	//	ランキング
	//	頂点座標の設定
	pVtx[ 12 ].pos = D3DXVECTOR3( RANKING_POS_X                  , RANKING_POS_Y , 0.0f );
	pVtx[ 13 ].pos = D3DXVECTOR3( RANKING_POS_X + RANKING_WIDTH  , RANKING_POS_Y , 0.0f );
	pVtx[ 14 ].pos = D3DXVECTOR3( RANKING_POS_X                  , RANKING_POS_Y + RANKING_HEIGHT , 0.0f );
	pVtx[ 15 ].pos = D3DXVECTOR3( RANKING_POS_X + RANKING_WIDTH  , RANKING_POS_Y + RANKING_HEIGHT , 0.0f );

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

	//	矢印
	//	頂点座標の設定
	pVtx[ 16 ].pos = D3DXVECTOR3( g_serect.Pos.x                   , g_serect.Pos.y , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( g_serect.Pos.x + g_serect.Size.x , g_serect.Pos.y , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( g_serect.Pos.x                   , g_serect.Pos.y + g_serect.Size.y , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( g_serect.Pos.x + g_serect.Size.x , g_serect.Pos.y + g_serect.Size.y , 0.0f );

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
	pVtx[ 17 ].tex = D3DXVECTOR2( 0.5f , 0 );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 19 ].tex = D3DXVECTOR2( 0.5f , 1 );

	g_pVtxBufferTitle -> Unlock();

	return S_OK;
}
/*-----------------------------------------------------------------------------
 関数名:	void ScreenTransition( void )
 引数:		
 戻り値:	
 説明:		画面遷移
-----------------------------------------------------------------------------*/
void ScreenTransition( void )
{
	//	画面遷移

	//	ゲーム
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , START_POS_X , START_POS_Y , START_WIDTH , START_HEIGHT ) )
	{
		if(GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_GAME);

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );


			//	音楽停止
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	チュートリアル
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
	{
		if(GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_TUTORIAL);

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	ランキング
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , RANKING_POS_X , RANKING_POS_Y , RANKING_WIDTH , RANKING_HEIGHT ) )
	{
		if(GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
		{
			g_Offset = 0.5f;

			SetFade( FADE_OUT , MODE_RANKING );

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_TITLE );
		
		}	//	end of if
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void CursorMove( void )
 引数:		
 戻り値:	
 説明:		カーソル移動
-----------------------------------------------------------------------------*/
void CursorMove( void )
{
	//	カーソル移動

	//	上移動	
	if( GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UPARROW) )
	{
		//	現在のカーソルの位置によって位置が変わる

		//	カーソルがチュートリアルにある時
		if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , RANKING_POS_X , RANKING_POS_Y , RANKING_WIDTH , RANKING_HEIGHT ) )
		{
			//	チュートリアルロゴに移動する
			g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.2f;
		}	//	end of if

		//	カーソルがランキングにある時
		else if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
		{
			//	スタートロゴに移動する
			g_serect.Pos.y = START_POS_Y + START_WIDTH * 0.2f;
		}	//	end of else if

		//	SE再生
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if


	//	下移動
	if(GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWNARROW) )
	{
		//	現在のカーソルの位置によって位置が変わる

		//	カーソルがスタートにある時
		if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , START_POS_X , START_POS_Y , START_WIDTH , START_HEIGHT ) )
		{
			//	チュートリアルロゴに移動する
			g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.2f;
		}	//	end of if

		//	カーソルがチュートリアルにある時
		else if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
		{
			//	ランキングロゴに移動する
			g_serect.Pos.y = RANKING_POS_Y + RANKING_WIDTH * 0.2f;
		}	//	end of else if

		//	SE再生
		PlaySound( SOUND_LABEL_SE_SERECT );
	}	//	end of if

	//	画面遷移
	ScreenTransition();

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void GamePadCursorMove( void )
 引数:		
 戻り値:	
 説明:		ゲームパッドのカーソル移動
-----------------------------------------------------------------------------*/
void GamePadCursorMove( void )
{
	//	スティックの取得
	LONG Stick_Y = GetGamePadStick_Y();

	if( CntFrame % DELAY_PAD == 0 )
	{
		CntFrame = 0;

		//	カーソル移動
		
		//	上移動	
		if( Stick_Y < 0 )
		{
			KeyFlag = true;

			if( KeyFlag == true )
			{
				//	現在のカーソルの位置によって位置が変わる

				//	ランキング
				if( g_RankingFlag == true )
				{
					g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.1f;

					g_RankingFlag = false;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	チュートリアル
				else if( g_TutorialFlag == true )
				{
					g_serect.Pos.y = START_POS_Y + START_WIDTH * 0.1f;

					g_TutorialFlag = false;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}
			}	//	end of if
		}


		//	下移動
		if( 999 < Stick_Y )
		{
			KeyFlag = false;

			if( KeyFlag == false )
			{
			
				//	現在のカーソルの位置によって位置が変わる

				//	チュートリアル
				if( g_TutorialFlag == false )
				{
					g_serect.Pos.y = TUTORIAL_POS_Y + TUTORIAL_WIDTH * 0.2f;

					g_TutorialFlag = true;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}

				//	ランキング
				else if( g_RankingFlag == false )
				{
					g_serect.Pos.y = RANKING_POS_Y + RANKING_WIDTH * 0.2f;

					g_RankingFlag = true;

					//	SE再生
					PlaySound( SOUND_LABEL_SE_SERECT );
				}
			}
		}
	}

	//	画面遷移
	GamePadTitleScreenTransition();
}
/*-----------------------------------------------------------------------------
 関数名:	void GamePadTitleScreenTransition( void )
 引数:		
 戻り値:	
 説明:		ゲームパッドの画面遷移
-----------------------------------------------------------------------------*/
void GamePadTitleScreenTransition( void )
{
	//	画面遷移

	//	ゲーム
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , START_POS_X , START_POS_Y , START_WIDTH , START_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_GAME);

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	チュートリアル
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , TUTORIAL_POS_X , TUTORIAL_POS_Y , TUTORIAL_WIDTH , TUTORIAL_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_Offset = 0.5f;

			SetFade(FADE_OUT , MODE_TUTORIAL);

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_TITLE );

		}	//	end of if
	}	//	end of if

	//	ランキング
	if( RectangleDecision( g_serect.Pos.x , g_serect.Pos.y , g_serect.Size.x , g_serect.Size.y  , RANKING_POS_X , RANKING_POS_Y , RANKING_WIDTH , RANKING_HEIGHT ) )
	{
		if( GetGamePadTrigger( BUTTOM_04 ) )
		{
			g_Offset = 0.5f;

			SetFade( FADE_OUT , MODE_RANKING );

			//	SE再生
			PlaySound( SOUND_LABEL_SE_ENTER );

			//	音楽停止
			StopSound( SOUND_LABEL_BGM_TITLE );
		
		}	//	end of if
	}	//	end of if
}
/*-----------------------------------------------------------------------------
 関数名:	void MakeVerTexSerectPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		矢印座標更新
-----------------------------------------------------------------------------*/
void MakeVerTexSerectPos( VERTEX_2D* pVtx )
{
	//	頂点座標の設定
	pVtx[ 16 ].pos = D3DXVECTOR3( g_serect.Pos.x , g_serect.Pos.y , 0.0f );
	pVtx[ 17 ].pos = D3DXVECTOR3( g_serect.Pos.x+g_serect.Size.x , g_serect.Pos.y , 0.0f );
	pVtx[ 18 ].pos = D3DXVECTOR3( g_serect.Pos.x, g_serect.Pos.y+g_serect.Size.y , 0.0f );
	pVtx[ 19 ].pos = D3DXVECTOR3( g_serect.Pos.x+g_serect.Size.x , g_serect.Pos.y+g_serect.Size.y , 0.0f );

	//	テクスチャ座標の設定
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 + g_Offset , 0 );
	pVtx[ 17 ].tex = D3DXVECTOR2( 0.5f + g_Offset , 0 );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 + g_Offset , 1 );
	pVtx[ 19 ].tex = D3DXVECTOR2( 0.5f + g_Offset , 1 );
}

/*-----------------------------------------------------------------------------
 関数名:	void TitleRogoPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		タイトルの座標変更
-----------------------------------------------------------------------------*/
void TitleRogoPos( VERTEX_2D* pVtx )
{
	if( g_TitleRogo.Pos.y != TITLE_POS_Y )
	{
		g_TitleRogo.Pos.y += g_TitleRogo.move.y;
	}

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x                      , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_TitleRogo.Pos.x + g_TitleRogo.Size.x , g_TitleRogo.Pos.y + g_TitleRogo.Size.y , 0.0f );
}