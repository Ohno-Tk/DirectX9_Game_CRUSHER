/*=============================================================================

		説明[ tutorial.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/09/01
-------------------------------------------------------------------------------
	■　Update
		2016/09/01
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "fade.h"
#include "tutorial.h"
#include "base.h"
#include "player.h"
#include "cloud.h"
#include "enemy.h"
#include "bg.h"
#include "particle.h"
#include "pause.h"
#include "operation.h"
#include "guid.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
//	移動
#define TUTORIAL_MOVE01_TEXTURENAME "data/TEXTURE/tutorial/move01.png"
#define TUTORIAL_MOVE02_TEXTURENAME "data/TEXTURE/tutorial/move02.png"

//	攻撃
#define TUTORIAL_ATTACK01_TEXTURENAME "data/TEXTURE/tutorial/attack01.png"
#define TUTORIAL_ATTACK02_TEXTURENAME "data/TEXTURE/tutorial/attack02.png"

//	ダッシュ
#define TUTORIAL_DASH01_TEXTURENAME "data/TEXTURE/tutorial/dash01.png"
#define TUTORIAL_DASH02_TEXTURENAME "data/TEXTURE/tutorial/dash02.png"

//	ジャンプ
#define TUTORIAL_JUMP01_TEXTURENAME "data/TEXTURE/tutorial/jump01.png"
#define TUTORIAL_JUMP02_TEXTURENAME "data/TEXTURE/tutorial/jump02.png"

//	ポーズ
#define TUTORIAL_PAUSE01_TEXTURENAME "data/TEXTURE/tutorial/pause01.png"
#define TUTORIAL_PAUSE02_TEXTURENAME "data/TEXTURE/tutorial/pause02.png"
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorial = NULL;

//	移動
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Move = NULL;//	テクスチャインターフェース

//	攻撃
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Attack = NULL;//	テクスチャインターフェース

//	ダッシュ
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Dash = NULL;//	テクスチャインターフェース

//	ジャンプ
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Jump = NULL;//	テクスチャインターフェース

//	ポーズ
LPDIRECT3DTEXTURE9 g_pTextureTutorial_Pause = NULL;//	テクスチャインターフェース

TUTORIAL g_Tutorial[ MAX_TUTORIAL ];
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice );						//	頂点の作成
void TutorialPos( VERTEX_2D* pVtx , int IndexTutorial );	//	座標変更
/*-----------------------------------------------------------------------------
 関数名:	void InitTutorial(void)
 引数:		なし
 戻り値:	なし
 説明:		チュートリアルの初期化
-----------------------------------------------------------------------------*/
void InitTutorial(void)
{
	int CntTutorial;

	//	今のモード
	MODE *Mode = GetMode();

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	足場情報の取得
	BASE *Base = GetBase( 0 );

	if( GamePad[ 0 ] == NULL )
	{
		//	エラーチェック
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_MOVE01_TEXTURENAME , &g_pTextureTutorial_Move  ) ) )
		{
			MessageBox( NULL , "移動０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_ATTACK01_TEXTURENAME , &g_pTextureTutorial_Attack  ) ) )
		{
			MessageBox( NULL , "攻撃０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_DASH01_TEXTURENAME , &g_pTextureTutorial_Dash  ) ) )
		{
			MessageBox( NULL , "ダッシュ０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_JUMP01_TEXTURENAME , &g_pTextureTutorial_Jump  ) ) )
		{
			MessageBox( NULL , "ジャンプ０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_PAUSE01_TEXTURENAME , &g_pTextureTutorial_Pause  ) ) )
		{
			MessageBox( NULL , "ポーズ０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	if( GamePad[ 0 ] != NULL )
	{
		//	エラーチェック
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_MOVE02_TEXTURENAME , &g_pTextureTutorial_Move  ) ) )
		{
			MessageBox( NULL , "移動０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_ATTACK02_TEXTURENAME , &g_pTextureTutorial_Attack  ) ) )
		{
			MessageBox( NULL , "攻撃０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_DASH02_TEXTURENAME , &g_pTextureTutorial_Dash  ) ) )
		{
			MessageBox( NULL , "ダッシュ０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_JUMP02_TEXTURENAME , &g_pTextureTutorial_Jump  ) ) )
		{
			MessageBox( NULL , "ジャンプ０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
		if( FAILED( D3DXCreateTextureFromFile(  pDevice , TUTORIAL_PAUSE02_TEXTURENAME , &g_pTextureTutorial_Pause  ) ) )
		{
			MessageBox( NULL , "ポーズ０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	頂点の作成
	MakeVertexTutorial( pDevice );

	*Mode = MODE_TUTORIAL;

	InitBg();	//	背景の初期化

	InitBase();	//	足場の初期化

	InitGuid();	//	案内の初期化

	InitCloud();	//	雲の初期化

	InitEnemy();	//	敵の初期化

	InitPlayer();	//	プレイヤーの初期化

	Initparticle();	//	パーティクルの初期化

	InitPause();	//	ポーズの初期化

	InitOperation();	//	操作方法の初期化

	//	初期化
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ )
	{
		g_Tutorial[ CntTutorial ].pos.y = 100.0f;
		g_Tutorial[ CntTutorial ].size.x = 300.0f;
		g_Tutorial[ CntTutorial ].size.y = 300.0f;
		g_Tutorial[ CntTutorial ].Move_X = Base -> move_X;
	}	//	end of for

	g_Tutorial[ 0 ].pos.x = 450.0f;
	g_Tutorial[ 1 ].pos.x = SCREEN_WIDTH + 300.0f;
	g_Tutorial[ 2 ].pos.x = SCREEN_WIDTH + 1000.0f;
	g_Tutorial[ 3 ].pos.x = SCREEN_WIDTH + 1500.0f;
	g_Tutorial[ 4 ].pos.x = SCREEN_WIDTH + 2250.0f;

	

	//	音楽再生
	PlaySound( SOUND_LABEL_BGM_GAME );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitTutorial(void)
 引数:		なし
 戻り値:	なし
 説明:		チュートリアルの終了
-----------------------------------------------------------------------------*/
void UninitTutorial(void)
{
	if( g_pVtxBufferTutorial != NULL )
	{
		g_pVtxBufferTutorial -> Release();
		g_pVtxBufferTutorial = NULL;
	}	//	end of if

	//	移動
	if( g_pTextureTutorial_Move != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTutorial_Move -> Release();
		g_pTextureTutorial_Move = NULL;
	}	//	end of if

	//	攻撃
	if( g_pTextureTutorial_Attack != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTutorial_Attack -> Release();
		g_pTextureTutorial_Attack = NULL;
	}	//	end of if

	//	ダッシュ
	if( g_pTextureTutorial_Dash != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTutorial_Dash -> Release();
		g_pTextureTutorial_Dash = NULL;
	}	//	end of if

	//	ジャンプ
	if( g_pTextureTutorial_Jump != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTutorial_Jump -> Release();
		g_pTextureTutorial_Jump = NULL;
	}	//	end of if

	//	ポーズ
	if( g_pTextureTutorial_Pause != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTutorial_Pause -> Release();
		g_pTextureTutorial_Pause = NULL;
	}	//	end of if

	UninitBg();	//	背景の終了

	UninitBase();	//	足場の終了

	UninitGuid();	//	案内の終了

	UninitCloud();	//	雲の終了

	UninitEnemy();	//	敵の終了

	UninitPlayer();	//	プレイヤーの終了

	Uninitparticle();	//	パーティクルの終了

	UninitPause();	//	ポーズの終了

	UninitOperation();	//	操作方法の終了

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateTutorial(void)
 引数:		なし
 戻り値:	なし
 説明:		チュートリアルの更新
-----------------------------------------------------------------------------*/
void UpdateTutorial(void)
{
	int CntTutorial;

	//	ポーズ情報の取得
	bool *Pause = GetPause();
	bool *PauseUse = GetOpeFlag();

	//	プレイヤー情報の取得
	PLAYER *Player = GetPlayer();

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTutorial -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ )
	{
		//	座標変更
		TutorialPos( pVtx , CntTutorial );
	}	//	end of for

	g_pVtxBufferTutorial -> Unlock();

	if( *Pause == false )
	{

		UpdateBg();	//	背景の更新

		UpdateBase();	//	足場の更新

		UpdateGuid();	//	案内の更新

		UpdateCloud();	//	雲の更新

		UpdateEnemy();	//	敵の更新

		UpdatePlayer();	//	プレイヤーの更新

		Updateparticle();	//	パーティクルの更新
	}

	if( *PauseUse == false )
	{
		UpdatePause();	//	ポーズの更新
	}
	else if( *PauseUse == true )
	{
		UpdateOperation();	//	操作方法の更新
	}

	//	画面遷移
	if( Player->gool == true || Player -> bUse == false )
	{
		SetFade(FADE_OUT , MODE_RESULT);

		//	音楽停止
		StopSound( SOUND_LABEL_BGM_GAME );
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawTutorial(void)
 引数:		なし
 戻り値:	なし
 説明:		チュートリアルの描画
-----------------------------------------------------------------------------*/
void DrawTutorial(void)
{
	//	ポーズ情報の取得
	bool *PauseUse = GetOpeFlag();

	DrawBg();	//	背景の描画

	DrawBase();		//	足場の描画

	DrawGuid();	//	案内の描画

	DrawCloud();	//	雲の描画



	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTutorial , 0 , sizeof( VERTEX_2D ) );

	//	移動
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Move );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

	//	攻撃
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Attack );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4, NUM_POLYGON);

	//	ダッシュ
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Dash );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8, NUM_POLYGON);

	//	ジャンプ
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Jump );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12, NUM_POLYGON);

	//	ポーズ
	pDevice -> SetTexture( 0 , g_pTextureTutorial_Pause );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16, NUM_POLYGON);


	DrawEnemy();	//	敵の描画

	DrawPlayer();	//	プレイヤーの描画

	Drawparticle();	//	パーティクルの描画

	if( *PauseUse == false )
	{
		DrawPause();	//	ポーズの描画
	}
	else if( *PauseUse == true )
	{
		DrawOperation();	//	操作方法の描画
	}


}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice )
{
	int CntTutorial;

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TUTORIAL , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTutorial , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTutorial -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ )
	{
		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x                                    , g_Tutorial[ CntTutorial ].pos.y                                    , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x + g_Tutorial[ CntTutorial ].size.x , g_Tutorial[ CntTutorial ].pos.y                                    , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x                                    , g_Tutorial[ CntTutorial ].pos.y + g_Tutorial[ CntTutorial ].size.y , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( g_Tutorial[ CntTutorial ].pos.x + g_Tutorial[ CntTutorial ].size.x , g_Tutorial[ CntTutorial ].pos.y + g_Tutorial[ CntTutorial ].size.y , 0.0f );

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

		//	ポインタをずらす
		pVtx += 4;
	}	//	end of for

	g_pVtxBufferTutorial -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void TutorialPos( VERTEX_2D* pVtx , int IndexTutorial )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		座標変更
-----------------------------------------------------------------------------*/
void TutorialPos( VERTEX_2D* pVtx , int IndexTutorial )
{
	pVtx += IndexTutorial * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x                                      , g_Tutorial[ IndexTutorial ].pos.y                                      , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x + g_Tutorial[ IndexTutorial ].size.x , g_Tutorial[ IndexTutorial ].pos.y                                      , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x                                      , g_Tutorial[ IndexTutorial ].pos.y + g_Tutorial[ IndexTutorial ].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Tutorial[ IndexTutorial ].pos.x + g_Tutorial[ IndexTutorial ].size.x , g_Tutorial[ IndexTutorial ].pos.y + g_Tutorial[ IndexTutorial ].size.y , 0.0f );


}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	TUTORIAL *GetTutorial( void )
 引数:		
 戻り値:	なし
 説明:		チュートリアル情報の取得
-----------------------------------------------------------------------------*/
TUTORIAL *GetTutorial( void )
{
	return &g_Tutorial[ 0 ];
}