/*=============================================================================

		敵[ enemy.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/08/08
-------------------------------------------------------------------------------
	■　Update
	2016/07/28
		・ヘッダファイル追加
			#include <stdio.h>
			#include "main.h"
			#include "enemy.h"
		・定数定義追加
			#define ENEMY_TEXTURENAME
		・グローバル変数追加
			LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//	テクスチャインターフェース
			LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//頂点バッファのインターフェースポインタ

			ENEMY g_Enemy;
		・プロトタイプ宣言追加
			HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
	2016/09/07
		・プロトタイプ宣言追加
			void EnemyStage1( void );	//	ステージ１の敵の配置
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "sound.h"
#include "enemy.h"
#include "player.h"
#include "base.h"
#include "fade.h"
#include "particle.h"
#include "score.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define ENEMY_TEXTURENAME "data/TEXTURE/game/car.png"
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//頂点バッファのインターフェースポインタ

ENEMY g_Enemy[MAX_ENEMY];
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEnemy( LPDIRECT3DDEVICE9 pDevice );		//	頂点の作成
void ModeTitleEnemy( int IndexEnemy );						//	タイトルモード時の敵の行動
void MakeEnemyPos( VERTEX_2D* pVtx , int IndexEnemy );		//	敵の座標変更
void UpdateEnemyAct( int IndexEnemy );						//	更新時の敵の行動
void CreateEnemy( float PosX , float PosY , float Speed );	//	敵の生成
void EnemyTitle( void );									//	タイトルの敵の生成
void EnemyTutorial( void );									//	チュートリアルの敵の配置
void EnemyStage1( void );									//	ステージ１の敵の配置
/*-----------------------------------------------------------------------------
 関数名:	void InitEnemy(void)
 引数:		なし
 戻り値:	なし
 説明:		敵の初期化
-----------------------------------------------------------------------------*/
void InitEnemy(void)
{
	int CntEnemy;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	足場情報の取得
	BASE *Base = GetBase( 0 );

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , ENEMY_TEXTURENAME , &g_pTextureEnemy  ) ) )
	{
		MessageBox( NULL , "敵の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	敵の初期化
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		g_Enemy[CntEnemy].Size.x = 100.0f;
		g_Enemy[CntEnemy].Size.y = 100.0f;
		g_Enemy[CntEnemy].Move.x = 1.0f;
		g_Enemy[CntEnemy].Move.y = 1.0f;
		g_Enemy[CntEnemy].Scroll_Move_X = Base -> move_X;
		g_Enemy[CntEnemy].Score = 100 + rand()%100;
		g_Enemy[CntEnemy].Use = false;
	}	//	end of for

	//	敵の生成
	if( *Mode == MODE_TITLE )
	{
		EnemyTitle();	//	タイトルの敵の配置
	}	//	end of if
	if( *Mode == MODE_TUTORIAL )
	{
		EnemyTutorial();	//	チュートリアルの敵の配置
	}	//	end of if

	else if( *Mode == MODE_GAME )
	{
		EnemyStage1();	//	ステージ１の敵の配置
	}	//	end of else if
	

	MakeVertexEnemy( pDevice );	//	頂点の作成
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitEnemy(void)
 引数:		なし
 戻り値:	なし
 説明:		敵の終了
-----------------------------------------------------------------------------*/
void UninitEnemy(void)
{

	if( g_pTextureEnemy != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureEnemy -> Release();
		g_pTextureEnemy = NULL;
	}	//	end of if

	if(g_pVtxBufferEnemy != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferEnemy -> Release();
		g_pVtxBufferEnemy = NULL;
	}	//	end of if

	//	敵の初期化
	for( int CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		if( g_Enemy[CntEnemy].Use == true )
		{
			g_Enemy[CntEnemy].Use = false;
		}
	}	//	end of for

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateEnemy(void)
 引数:		なし
 戻り値:	なし
 説明:		敵の更新
-----------------------------------------------------------------------------*/
void UpdateEnemy(void)
{
	int CntEnemy;

	//	今のモード
	MODE *Mode = GetNextMode();

	//	フェード中かどうか
	bool *FadeUse = GetFadeUse();

	VERTEX_2D* pVtx;

	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferEnemy -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{

		//	敵が生きていたら かつ　フェードをしていなかったら
		if( g_Enemy[CntEnemy].Use == true && *FadeUse == false )
		{
			if( *Mode == MODE_TITLE )
			{
				//	タイトルモード時の敵の行動
				ModeTitleEnemy( CntEnemy );

			}

			else if( *Mode == MODE_GAME || *Mode == MODE_TUTORIAL )
			{
				//	更新時の敵の行動
				UpdateEnemyAct( CntEnemy );
			}

			MakeEnemyPos( pVtx , CntEnemy );	//	敵の座標変更
		}	//	end of if
	}	//	end of for

	g_pVtxBufferEnemy -> Unlock(); //これ以降触れてはいけない

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawEnemy(void)
 引数:		なし
 戻り値:	なし
 説明:		敵の描画
-----------------------------------------------------------------------------*/
void DrawEnemy(void)
{
	int CntEnemy;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferEnemy , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureEnemy );

	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		if( g_Enemy[CntEnemy].Use == true )
		{
			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntEnemy * NUM_VERTEX , NUM_POLYGON);
		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexEnemy( LPDIRECT3DDEVICE9 pDevice )
 引数:		なし
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEnemy( LPDIRECT3DDEVICE9 pDevice )
{
	int CntEnemy;

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_ENEMY  , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferEnemy , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferEnemy -> Lock ( 0 , 0 , ( void** )&pVtx ,  0 );

	for(CntEnemy = 0;CntEnemy < MAX_ENEMY;CntEnemy++)
	{
		//	頂点座標の設定
		pVtx[ CntEnemy*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x , g_Enemy[CntEnemy].Pos.y , 0.0f );
		pVtx[ CntEnemy*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x+g_Enemy[CntEnemy].Size.x , g_Enemy[CntEnemy].Pos.y , 0.0f );
		pVtx[ CntEnemy*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x , g_Enemy[CntEnemy].Pos.y+g_Enemy[CntEnemy].Size.y , 0.0f );
		pVtx[ CntEnemy*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Enemy[CntEnemy].Pos.x+g_Enemy[CntEnemy].Size.x , g_Enemy[CntEnemy].Pos.y+g_Enemy[CntEnemy].Size.y , 0.0f );

		//	座標変換済み頂点フラグの設定
		pVtx[ CntEnemy*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+3 ].rhw = 1.0f;

		//	座標変換済み頂点フラグの設定
		pVtx[ CntEnemy*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntEnemy*NUM_VERTEX+3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ CntEnemy*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntEnemy*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntEnemy*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntEnemy*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ CntEnemy*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntEnemy*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntEnemy*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntEnemy*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	g_pVtxBufferEnemy -> Unlock();

	return S_OK;
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void ModeTitleEnemy( int IndexEnemy )
 引数:		int IndexEnemy
 戻り値:	なし
 説明:		タイトルモード時の敵の行動
-----------------------------------------------------------------------------*/
void ModeTitleEnemy( int IndexEnemy )
{
	g_Enemy[IndexEnemy].Pos.x += g_Enemy[IndexEnemy].Move.x;

	//	画面外処理
	if( g_Enemy[IndexEnemy].Pos.x > SCREEN_WIDTH )
	{
		g_Enemy[IndexEnemy].Pos.x = -( g_Enemy[IndexEnemy].Size.x );
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateEnemyAct( int IndexEnemy )
 引数:		int IndexEnemy
 戻り値:	なし
 説明:		更新時の敵の行動
-----------------------------------------------------------------------------*/
void UpdateEnemyAct( int IndexEnemy )
{
	g_Enemy[IndexEnemy].OldPos.x = g_Enemy[IndexEnemy].Pos.x;
	g_Enemy[IndexEnemy].OldPos.y = g_Enemy[IndexEnemy].Pos.y;

	//	敵が画面に現れたら
	if( g_Enemy[IndexEnemy].Pos.x + g_Enemy[IndexEnemy].Size.x < SCREEN_WIDTH )
	{
		//	移動
		g_Enemy[IndexEnemy].Pos.x -= g_Enemy[IndexEnemy].Move.x;
	}	//	end of if

	g_Enemy[IndexEnemy].Move.y += GRAVITY;	//	重力を加える
	g_Enemy[IndexEnemy].Pos.y += g_Enemy[IndexEnemy]. Move.y;

	//	画面がい処理
	if( g_Enemy[IndexEnemy].Pos.x + g_Enemy[IndexEnemy].Size.x < 0 )
	{
		g_Enemy[IndexEnemy].Use = false;
	}	//	end of if
	if( g_Enemy[IndexEnemy].Pos.y > SCREEN_HEIGHT )
	{
		g_Enemy[IndexEnemy].Use = false;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void CreateEnemy( float PosX , float PosY )
 引数:		float PosX
			float PosY
 戻り値:	なし
 説明:		敵の生成
-----------------------------------------------------------------------------*/
void CreateEnemy( float PosX , float PosY , float Speed )
{
	int CntEnemy;

	for ( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
	{
		if( g_Enemy[ CntEnemy ].Use == false )
		{
			g_Enemy[ CntEnemy ].Pos.x = PosX;
			g_Enemy[ CntEnemy ].Pos.y = PosY;
			g_Enemy[CntEnemy].Move.x = Speed;
			g_Enemy[ CntEnemy ].Use = true;
			break;
		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void MakeEnemyPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		敵の座標変更
-----------------------------------------------------------------------------*/
void MakeEnemyPos( VERTEX_2D* pVtx , int IndexEnemy )
{
	pVtx += IndexEnemy*NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x , g_Enemy[IndexEnemy].Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x+g_Enemy[IndexEnemy].Size.x , g_Enemy[IndexEnemy].Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x , g_Enemy[IndexEnemy].Pos.y+g_Enemy[IndexEnemy].Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Enemy[IndexEnemy].Pos.x+g_Enemy[IndexEnemy].Size.x , g_Enemy[IndexEnemy].Pos.y+g_Enemy[IndexEnemy].Size.y , 0.0f );
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void EnemyTutorial( void )
 引数:		
 戻り値:	なし
 説明:		タイトルの敵の配置
-----------------------------------------------------------------------------*/
void EnemyTitle( void )
{
	CreateEnemy( -100.0f , SCREEN_HEIGHT , 1.0f );						//	g_Enemy[ 0 ]
	CreateEnemy( -100.0f , SCREEN_HEIGHT , 2.0f );						//	g_Enemy[ 0 ]
	CreateEnemy( -100.0f , SCREEN_HEIGHT , 3.0f );						//	g_Enemy[ 0 ]
}

/*-----------------------------------------------------------------------------
 関数名:	void EnemyTutorial( void )
 引数:		
 戻り値:	なし
 説明:		チュートリアルの敵の配置
-----------------------------------------------------------------------------*/
void EnemyTutorial( void )
{
	CreateEnemy( SCREEN_WIDTH + 600.0f , SCREEN_HEIGHT  - 100.0f , 1.0f );						//	g_Enemy[ 0 ]

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void EnemyStage1( void )
 引数:		
 戻り値:	なし
 説明:		ステージ１の敵の配置
-----------------------------------------------------------------------------*/
void EnemyStage1( void )
{
	CreateEnemy( SCREEN_WIDTH + 97.0f , SCREEN_HEIGHT  - 50.0f , 1.0f );						//	g_Enemy[ 0 ]
	CreateEnemy( SCREEN_WIDTH + 1270.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 1 ]
	CreateEnemy( SCREEN_WIDTH + 1700.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 2 ]
	CreateEnemy( SCREEN_WIDTH + 1895.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 3 ]
	CreateEnemy( SCREEN_WIDTH + 3200.0f , SCREEN_HEIGHT - 480.0f , 1.0f );						//	g_Enemy[ 4 ]
	CreateEnemy( SCREEN_WIDTH + 3300.0f , SCREEN_HEIGHT - 480.0f , 1.0f );						//	g_Enemy[ 5 ]
	CreateEnemy( SCREEN_WIDTH + 3900.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 6 ]
	//CreateEnemy( SCREEN_WIDTH + 4000.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 7 ]
	CreateEnemy( SCREEN_WIDTH + 4300.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 8 ]
	CreateEnemy( SCREEN_WIDTH + 4800.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 9 ]
	//CreateEnemy( SCREEN_WIDTH + 4750.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 10 ]
	CreateEnemy( SCREEN_WIDTH + 4930.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 11 ]
	CreateEnemy( SCREEN_WIDTH + 5440.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 12 ]
	CreateEnemy( SCREEN_WIDTH + 5800.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 13 ]
	CreateEnemy( SCREEN_WIDTH + 5890.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 14 ]
	CreateEnemy( SCREEN_WIDTH + 7700.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 15 ]
	CreateEnemy( SCREEN_WIDTH + 7800.0f , SCREEN_HEIGHT - 50.0f , 1.0f );						//	g_Enemy[ 16 ]

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	ENEMY *GetEnemy( int IndexEnemy )
 引数:		int IndexEnemy
 戻り値:	return &g_Enemy;
 説明:		敵情報の取得
-----------------------------------------------------------------------------*/
ENEMY *GetEnemy( int IndexEnemy )
{
	return &g_Enemy[ IndexEnemy ];
}	//	end of func