/*=============================================================================

		足場描画[ base.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/08/22
-------------------------------------------------------------------------------
	■　Update
		2016/08/22
			・ヘッダファイル追加
				#include <stdio.h>
				#include "main.h"
				#include "base.h"
				#include "bg.h"
			・定数定義追加
				#define BASE_TEXTURENAME
			・プロトタイプ宣言追加
				HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
				void BasePos( VERTEX_2D* pVtx , int IndexBase );			//	足場座標更新
				void CreateBase( float PosX , float PosY , float Width , float Height );	//	足場の生成
			・グローバル変数追加
				LPDIRECT3DTEXTURE9 g_pTextureBase = NULL;//	テクスチャインターフェース
				LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBase = NULL;	//頂点バッファのインターフェースポインタ
				BASE g_Base[ MAX_BASE ];
		2016/08/27
			・ヘッダファイル追加
				#include "player.h"
			・プロトタイプ宣言追加
			void Create( void );	//	画面スクロール時の足場の生成
			void BaseDecision( VERTEX_2D* pVtx );	//	足場の判定
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "base.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define BASE_TEXTURENAME "data/TEXTURE/game/base.jpg"
#define GOOL_TEXTURENAME "data/TEXTURE/game/gool.png"
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice );						//	頂点の作成
void GoolPos( VERTEX_2D* pVtx );											//	ゴール座標更新
void BasePos( VERTEX_2D* pVtx , int IndexBase );							//	足場座標更新
void CreateBase( float PosX , float PosY , float Width , float Height );	//	足場の生成
void BaseTutorial( void );	//	チュートリアルの足場の生成
void BaseStage1( void );													//	ステージ１の足場の生成
void BaseDecision( VERTEX_2D* pVtx );										//	足場の判定
void PlayerBaseDecision( VERTEX_2D* pVtx );									//	プレイヤーの足場判定
void EnemyBaseDecision( VERTEX_2D* pVtx );									//	敵の足場判定
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureBase = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBase = NULL;	//頂点バッファのインターフェースポインタ
//	ゴール
LPDIRECT3DTEXTURE9 g_pTextureGool = NULL;//	テクスチャインターフェース

BASE g_Base[ MAX_BASE ];

/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitBase( void )
 引数:		なし
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		足場の初期化
-----------------------------------------------------------------------------*/
HRESULT InitBase( void )
{
	int CntBase;

	//	デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , BASE_TEXTURENAME , &g_pTextureBase  ) ) )
	{
		MessageBox( NULL , "足場の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		return E_FAIL;
	}
	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , GOOL_TEXTURENAME , &g_pTextureGool  ) ) )
	{
		MessageBox( NULL , "ゴールの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		return E_FAIL;
	}
	
	//	足場初期化
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ )
	{
		g_Base[ CntBase ].size.x = 0.0f;	//	Width	
		g_Base[ CntBase ].size.y = 0.0f;	//	Height
		g_Base[ CntBase ].move_X = MOVE_REPOSIT;	//	移動量
		g_Base[ CntBase ].use = false;		//	使用フラグ
	}	//	end of for

	if( *Mode == MODE_TITLE )
	{
		CreateBase( -20.0f , SCREEN_HEIGHT - 50.0f , SCREEN_WIDTH + 20.0f , 50.0f );
	}	

	else if( *Mode == MODE_TUTORIAL )
	{
		BaseTutorial();	//	チュートリアルの足場の生成

		//	ゴール初期化
		g_Base[ MAX_GOOL ].pos.x = SCREEN_WIDTH + 2800.0f;
		g_Base[ MAX_GOOL ].pos.y = 200.0f;
		g_Base[ MAX_GOOL ].size.x = 100.0f;
		g_Base[ MAX_GOOL ].size.y = 323.0f;
		g_Base[ MAX_GOOL ].move_X = MOVE_REPOSIT;
		g_Base[ MAX_GOOL ].GoolFalg = false;
		g_Base[ MAX_GOOL ].use = true;
	}	//	end of if

	else if( *Mode == MODE_GAME )
	{
		BaseStage1();	//	ステージ１の足場の生成

		//	ゴール初期化
		g_Base[ MAX_GOOL ].pos.x = SCREEN_WIDTH + 8700.0f;
		g_Base[ MAX_GOOL ].pos.y = 100.0f;
		g_Base[ MAX_GOOL ].size.x = 100.0f;
		g_Base[ MAX_GOOL ].size.y = 430.0f;
		g_Base[ MAX_GOOL ].move_X = 3.0f;
		g_Base[ MAX_GOOL ].GoolFalg = false;
		g_Base[ MAX_GOOL ].use = true;

	}	//	end of else if

	MakeVertexBase( pDevice );	//	頂点の作成

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitBase( void )
 引数:		なし
 戻り値:	なし
 説明:		足場の終了
-----------------------------------------------------------------------------*/
void UninitBase( void )
{
	if( g_pTextureBase != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureBase -> Release();
		g_pTextureBase = NULL;
	}
	if(g_pVtxBufferBase != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferBase -> Release();
		g_pVtxBufferBase  = NULL;
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateBase( void )
 引数:		なし
 戻り値:	なし
 説明:		足場の更新
-----------------------------------------------------------------------------*/
void UpdateBase( void )
{
	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferBase -> Lock ( 0 ,0 ,( void** )&pVtx ,0 );

	GoolPos( pVtx );

	if( g_Base[ MAX_GOOL ].GoolFalg == false )
	{
		BaseDecision( pVtx );
	}	//	end of if

	g_pVtxBufferBase -> Unlock();
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawBase( void )
 引数:		なし
 戻り値:	なし
 説明:		足場の描画
-----------------------------------------------------------------------------*/
void DrawBase( void )
{
	int CntBase;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferBase , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureBase );

	//	足場
	for( CntBase = 0 ; CntBase < MAX_BASE - 1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == true )
		{
			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntBase * NUM_VERTEX, NUM_POLYGON);
		}	//	end of if
	}	//	end of for

	//	ゴール
	pDevice -> SetTexture( 0 , g_pTextureGool );

	if( g_Base[ MAX_GOOL ].use == true )
	{
		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , MAX_GOOL * NUM_VERTEX, NUM_POLYGON);
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBase( LPDIRECT3DDEVICE9 pDevice )
{
	int CntBase;

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_BASE  , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferBase , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferBase -> Lock ( 0 , 0 , ( void** )&pVtx ,  0 );
	
	//	足場
	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		//	頂点座標の設定
		BasePos( pVtx , CntBase );

		//	座標変換済み頂点フラグの設定
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	座標変換済み頂点フラグの設定
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ CntBase*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ CntBase*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntBase*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntBase*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntBase*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	//	ゴール
	for( CntBase = MAX_GOOL ; CntBase < MAX_BASE ; CntBase++ )
	{
		//	ゴール
		//	頂点座標の設定
		pVtx[ CntBase*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
		pVtx[ CntBase*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
		pVtx[ CntBase*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );
		pVtx[ CntBase*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );

		//	座標変換済み頂点フラグの設定
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	座標変換済み頂点フラグの設定
		pVtx[ CntBase*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntBase*NUM_VERTEX+3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ CntBase*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntBase*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ CntBase*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntBase*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntBase*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntBase*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}

	g_pVtxBufferBase -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void CreateBase( float PosX , float PosY , float Width , float Height )
 引数:		float PosX
			float PosY
			float Width
 戻り値:	なし
 説明:		足場の生成
-----------------------------------------------------------------------------*/
void CreateBase( float PosX , float PosY , float Width , float Height )
{
	int CntBase;

	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == false )
		{
			g_Base[ CntBase ].pos.x = PosX;		//	X座標
			g_Base[ CntBase ].pos.y = PosY;		//	Y座標
			g_Base[ CntBase ].size.x = Width;	//	Width
			g_Base[ CntBase ].size.y = Height;	//	Height
			g_Base[ CntBase ].use = true;		//	使用フラグ
			break;
		}
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void BaseTutorial( void )
 引数:		
 戻り値:	なし
 説明:		チュートリアルの足場の生成
-----------------------------------------------------------------------------*/
void BaseTutorial( void )
{
	//	足場の生成
	CreateBase( -20.0f , SCREEN_HEIGHT - 50.0f , SCREEN_WIDTH + 1950.0f , 50.0f );

	CreateBase( SCREEN_WIDTH + 2025.0f , SCREEN_HEIGHT - 50.0f , 2800.0f , 50.0f );

	//	ガイド足場
	CreateBase( 300.0f , SCREEN_HEIGHT - 200.0f , 150.0f , 50.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void BaseStage1( void )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		ステージ１の足場の生成
-----------------------------------------------------------------------------*/
void BaseStage1( void )
{
	//	足場の生成
	CreateBase( -20.0f , SCREEN_HEIGHT - 50.0f , SCREEN_WIDTH + 2600.0f , 50.0f );	
	CreateBase( SCREEN_WIDTH + 2700.0f , SCREEN_HEIGHT - 50.0f , 840.0f , 50.0f );	
	CreateBase( SCREEN_WIDTH + 3675.0f , SCREEN_HEIGHT - 50.0f , 3075.0f , 50.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 50.0f , 3100.0f , 50.0f );	

	//	足場１
	CreateBase( SCREEN_WIDTH - 150.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	足場２
	CreateBase( SCREEN_WIDTH + 225.0f , SCREEN_HEIGHT - 200.0f , 200.0f , 50.0f );

	//	足場３
	CreateBase( SCREEN_WIDTH + 305.0f , SCREEN_HEIGHT - 350.0f , 50.0f , 50.0f );	

	//	足場４
	CreateBase( SCREEN_WIDTH + 600.0f , SCREEN_HEIGHT - 150.0f , 80.0f , 100.0f );	

	//	足場５
	CreateBase( SCREEN_WIDTH + 950.0f, SCREEN_HEIGHT - 200.0f , 80.0f , 150.0f );	

	//	足場６
	CreateBase( SCREEN_WIDTH + 1400.0f , SCREEN_HEIGHT - 225.0f , 80.0f , 175.0f );	

	//	足場7
	CreateBase( SCREEN_WIDTH + 2000.0f , SCREEN_HEIGHT - 225.0f , 80.0f , 175.0f );	

	//	足場８
	CreateBase( SCREEN_WIDTH + 3000.0f , SCREEN_HEIGHT - 225.0f , 150.0f , 50.0f );

	//	足場９
	CreateBase( SCREEN_WIDTH + 3200.0f , SCREEN_HEIGHT - 380.0f , 400.0f , 50.0f );	

	//	足場１０
	CreateBase( SCREEN_WIDTH + 3700.0f , SCREEN_HEIGHT - 380.0f , 200.0f , 50.0f );	

	//	足場１１
	CreateBase( SCREEN_WIDTH + 3850.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	足場１２
	CreateBase( SCREEN_WIDTH + 4150.0f , SCREEN_HEIGHT - 225.0f , 100.0f , 50.0f );

	//	足場１３
	CreateBase( SCREEN_WIDTH + 4450.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	足場１３
	CreateBase( SCREEN_WIDTH + 4600.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	足場１４
	CreateBase( SCREEN_WIDTH + 4600.0f , SCREEN_HEIGHT - 350.0f , 50.0f , 50.0f );	

	//	足場１５
	CreateBase( SCREEN_WIDTH + 4750.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	足場１６
	CreateBase( SCREEN_WIDTH + 5050.0f , SCREEN_HEIGHT - 200.0f , 50.0f , 50.0f );	

	//	足場１７
	CreateBase( SCREEN_WIDTH + 5150.0f , SCREEN_HEIGHT - 300.0f , 150.0f , 50.0f );

	//	足場１８
	CreateBase( SCREEN_WIDTH + 5550.0f , SCREEN_HEIGHT - 300.0f , 200.0f , 50.0f );

	//	足場１９
	CreateBase( SCREEN_WIDTH + 5600.0f , SCREEN_HEIGHT - 200.0f , 100.0f , 50.0f );

	//	足場１９( 階段ブロック )
	CreateBase( SCREEN_WIDTH + 5850.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 5900.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 5950.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6000.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	足場２０( 逆階段ブロック )
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6150.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	足場２１( 階段ブロック )
	CreateBase( SCREEN_WIDTH + 6550.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6600.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6650.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6700.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	足場２２( 逆階段ブロック )
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 90.0f  , 200.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 130.0f , 150.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 170.0f , 100.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 6850.0f , SCREEN_HEIGHT - 210.0f , 50.0f  , 40.0f );	

	//	足場２３
	CreateBase( SCREEN_WIDTH + 7250.0f , SCREEN_HEIGHT - 150.0f  , 50.0f , 100.0f );

	//	足場２４
	CreateBase( SCREEN_WIDTH + 7450.0f , SCREEN_HEIGHT - 200.0f  , 200.0f , 50.0f );

	//	足場２５
	CreateBase( SCREEN_WIDTH + 8000.0f , SCREEN_HEIGHT - 150.0f  , 50.0f , 100.0f );

	//	足場２６( 階段ブロック )
	CreateBase( SCREEN_WIDTH + 8050.0f , SCREEN_HEIGHT - 90.0f  , 450.0f , 40.0f );	
	CreateBase( SCREEN_WIDTH + 8100.0f , SCREEN_HEIGHT - 130.0f  , 400.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8150.0f , SCREEN_HEIGHT - 170.0f  , 350.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8200.0f , SCREEN_HEIGHT - 210.0f  , 300.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8250.0f , SCREEN_HEIGHT - 250.0f  , 250.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8300.0f , SCREEN_HEIGHT - 290.0f  , 200.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8350.0f , SCREEN_HEIGHT - 330.0f  , 150.0f , 40.0f );
	CreateBase( SCREEN_WIDTH + 8400.0f , SCREEN_HEIGHT - 370.0f  , 100.0f , 40.0f );//	g_Base[ 51 ]

	//	ガイド足場
	CreateBase( 300.0f , SCREEN_HEIGHT - 200.0f , 150.0f , 50.0f );


}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void BaseDecision( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		足場の判定
-----------------------------------------------------------------------------*/
void BaseDecision( VERTEX_2D* pVtx )
{
	//	プレイヤー情報の取得
	PLAYER *Player = GetPlayer();

	//	プレイヤーが敵と当たったら
	if( Player -> Hit == false )
	{
		//	プレイヤーの足場判定
		PlayerBaseDecision( pVtx );
	}	//	end of if

	//	敵の足場判定
	EnemyBaseDecision( pVtx );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void PlayerBaseDecision( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		プレイヤーの足場判定
-----------------------------------------------------------------------------*/
void PlayerBaseDecision( VERTEX_2D* pVtx )
{
	int CntBase;

	//	プレイヤー情報の取得
	PLAYER *Player = GetPlayer();

	//	プレイヤーとの判定
	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == true )
		{
			//	足場との判定
			if( g_Base[ CntBase ].pos.x <= ( Player->Pos.x + Player->Size.x ) - 35.0f &&  Player->Pos.x + 10.0f < g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x - 10.0f )
			{
				//上の判定
				if( g_Base[ CntBase ].pos.y < Player->Pos.y + Player->Size.y && Player->OldPos.y <= g_Base[ CntBase ].pos.y )
				{					

					Player->Move.y = 0.0f;
					Player->Pos.y = g_Base[ CntBase ].pos.y - Player->Size.y;
					Player->Jump = false;

				}	//	end of if

				//下の判定
				if( Player->Pos.y <= g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y && Player->OldPos.y > g_Base[ CntBase ].pos.y )
				{
					Player->Move.y = 0.0f;
					Player->Pos.y = g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y/* + 12.0f*/;
				}	//	end of if
			}	//	end of if

			//	横壁の判定
			if( g_Base[ CntBase ].pos.y < Player->Pos.y + Player->Size.y && Player->Pos.y < g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y )
			{

				//	左壁との判定
				if(  g_Base[ CntBase ].pos.x < Player->Pos.x + Player->Size.x - 10.0f && Player->Pos.x + Player->Size.x - 10.0f < g_Base[ CntBase ].pos.x + 12.0f )
				{
					Player->Pos.x = g_Base[ CntBase ].pos.x - Player->Size.x + 10.0f;
				}	//	end of if

				//	右壁との判定
				if( Player->Pos.x < g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x && g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x < Player->Pos.x + 10.0f )
				{
					Player->Pos.x = g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x + 10.0f;
				}	//	end of if
			}	//	end of if

			//	足場座標更新
			BasePos( pVtx , CntBase );

		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void EnemyBaseDecision( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		敵の足場判定
-----------------------------------------------------------------------------*/
void EnemyBaseDecision( VERTEX_2D* pVtx )
{
	int CntBase , CntEnemy;

	//	敵の情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	敵との判定
	for( CntBase = 0 ; CntBase < MAX_BASE-1 ; CntBase++ )
	{
		if( g_Base[ CntBase ].use == true )
		{
			for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ )
			{
				if( Enemy[CntEnemy].Use ==true )
				{

					//	足場との判定
					if( g_Base[ CntBase ].pos.x <= Enemy[CntEnemy].Pos.x + Enemy[CntEnemy].Size.x &&  Enemy[CntEnemy].Pos.x < ( g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x ) - 15.0f )
					{

						//上の判定
						if( g_Base[ CntBase ].pos.y < Enemy[CntEnemy].Pos.y + Enemy[CntEnemy].Size.y && Enemy[CntEnemy].OldPos.y <= g_Base[ CntBase ].pos.y )
						{
							Enemy[CntEnemy].Move.y = 0.0f;
							Enemy[CntEnemy].Pos.y = g_Base[ CntBase ].pos.y - Enemy[CntEnemy].Size.y;
						}	//	end of if

						//////下の判定
						//if( Enemy[CntEnemy].Pos.y <= g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y && Enemy[CntEnemy].OldPos.y > g_Base[ CntBase ].pos.y )
						//{
						//	Enemy[CntEnemy].Move.y = 0.0f;
						//	Enemy[CntEnemy].Pos.y = g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y + 12.0f;
						//}
					}	//	end of if

					//	横壁の判定
					if( g_Base[ CntBase ].pos.y < Enemy[CntEnemy].Pos.y + Enemy[CntEnemy].Size.y && Enemy[CntEnemy].Pos.y < g_Base[ CntBase ].pos.y + g_Base[ CntBase ].size.y )
					{

						//	左壁との判定
						if(  g_Base[ CntBase ].pos.x < Enemy[CntEnemy].Pos.x + Enemy[CntEnemy].Size.x - 10.0f && Enemy[CntEnemy].Pos.x + Enemy[CntEnemy].Size.x - 10.0f < g_Base[ CntBase ].pos.x + 12.0f )
						{
							Enemy[CntEnemy].Pos.x = g_Base[ CntBase ].pos.x - Enemy[CntEnemy].Size.x + 10.0f;
							Enemy[CntEnemy].Move.x *= -1.0f;
						}	//	end of if

						//	右壁との判定
						if( Enemy[CntEnemy].Pos.x < g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x && g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x < Enemy[CntEnemy].Pos.x + 20.0f )
						{
							Enemy[CntEnemy].Pos.x = g_Base[ CntBase ].pos.x + g_Base[ CntBase ].size.x + 10.0f;
							Enemy[CntEnemy].Move.x *= -1.0f;
						}	//	end of if
					}	//	end of if
				}	//	end of if

				//	足場座標更新
				BasePos( pVtx , CntBase );

			}	//	end of for
		}	//	end of if
	}	//	end of for
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void GoolPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		ゴール座標更新
-----------------------------------------------------------------------------*/
void GoolPos( VERTEX_2D* pVtx )
{
	pVtx += MAX_GOOL * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Base[ MAX_GOOL ].pos.x+g_Base[ MAX_GOOL ].size.x , g_Base[ MAX_GOOL ].pos.y+g_Base[ MAX_GOOL ].size.y , 0.0f );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void BasePos( VERTEX_2D* pVtx , int IndexBase )
 引数:		VERTEX_2D* pVtx
			int IndexBase
 戻り値:	なし
 説明:		足場座標更新
-----------------------------------------------------------------------------*/
void BasePos( VERTEX_2D* pVtx , int IndexBase )
{
	pVtx += IndexBase * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x , g_Base[ IndexBase ].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x+g_Base[ IndexBase ].size.x , g_Base[ IndexBase ].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x , g_Base[ IndexBase ].pos.y+g_Base[ IndexBase ].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Base[ IndexBase ].pos.x+g_Base[ IndexBase ].size.x , g_Base[ IndexBase ].pos.y+g_Base[ IndexBase ].size.y , 0.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	BASE *GetBase( int IndexBase )
 引数:		なし
 戻り値:	return &g_Base[IndexBase];
 説明:		足場情報の取得
-----------------------------------------------------------------------------*/
BASE *GetBase( int IndexBase )
{
	return &g_Base[IndexBase];
}	//	end of func