/*=============================================================================

		雲[ cloud.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/09/14
-------------------------------------------------------------------------------
	■　Update
	2016/09/14
		・ヘッダファイル追加
			#include <stdio.h>
			#include "main.h"
			#include "cloud.h"
		・定数定義追加
			#define CLOUD_TEXTURENAME
		・グローバル変数追加
			LPDIRECT3DTEXTURE9 g_pTextureCloud = NULL;//	テクスチャインターフェース
			LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCloud = NULL;//頂点バッファのインターフェースポインタ

			CLOUD g_Cloud[ MAX_CLOUD ];
		・プロトタイプ宣言追加
			HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
			void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud );	//	更新時の雲の行動
			void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud );	//	座標の更新
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "cloud.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define CLOUD_TEXTURENAME "data/TEXTURE/game/雲.png"

#define MAX_CLOUD ( 7 )	//	雲の最大数
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureCloud = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCloud = NULL;//頂点バッファのインターフェースポインタ

CLOUD g_Cloud[ MAX_CLOUD ];
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud );	//	更新時の雲の行動
void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud );	//	座標の更新
/*-----------------------------------------------------------------------------
 関数名:	void InitCloud(void)
 引数:		なし
 戻り値:	なし
 説明:		雲の初期化
-----------------------------------------------------------------------------*/
void InitCloud(void)
{
	int CntCloud;

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , CLOUD_TEXTURENAME , &g_pTextureCloud  ) ) )
	{
		MessageBox( NULL , "雲の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	if( *Mode == MODE_TITLE )
	{
		//	初期化
		for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
		{
			g_Cloud[ CntCloud ].pos.x = 0.0f;
			g_Cloud[ CntCloud ].pos.y = 0.0f;
			g_Cloud[ CntCloud ].size.x = 200.0f;
			g_Cloud[ CntCloud ].size.y = 150.0f;
			g_Cloud[ CntCloud ].move_X = 2.0f + rand()%3;
			g_Cloud[ CntCloud ].use = true;
		}	//	end of for

		g_Cloud[ 0 ].pos.x = 100.0f;
		g_Cloud[ 0 ].pos.y = 0.0f;

		g_Cloud[ 1 ].pos.x = 200.0f;
		g_Cloud[ 1 ].pos.y = 100.0f;

		g_Cloud[ 2 ].pos.x = 450.0f;
		g_Cloud[ 2 ].pos.y = 50.0f;

		g_Cloud[ 3 ].pos.x = 550.0f;
		g_Cloud[ 3 ].pos.y = 80.0f;

		g_Cloud[ 4 ].pos.x = 600.0f;
		g_Cloud[ 4 ].pos.y = 30.0f;

		g_Cloud[ 5 ].pos.x = 600.0f;
		g_Cloud[ 5 ].pos.y = 90.0f;

		g_Cloud[ 6 ].pos.x = 800.0f;
		g_Cloud[ 6 ].pos.y = 100.0f;

	}

	else
	{
		//	初期化
		for( CntCloud = 0 ; CntCloud < MAX_CLOUD - 3 ; CntCloud++ )
		{
			g_Cloud[ CntCloud ].pos.x = rand()%800;
			g_Cloud[ CntCloud ].pos.y = rand()%100;
			g_Cloud[ CntCloud ].size.x = 200.0f;
			g_Cloud[ CntCloud ].size.y = 150.0f;
			g_Cloud[ CntCloud ].move_X = 2.0f + rand()%3;
			g_Cloud[ CntCloud ].use = true;
		}	//	end of for

		for(  ; CntCloud < MAX_CLOUD ; CntCloud++ )
		{
			g_Cloud[ CntCloud ].pos.x = rand()%800;
			g_Cloud[ CntCloud ].pos.y = rand()%10;
			g_Cloud[ CntCloud ].size.x = 200.0f;
			g_Cloud[ CntCloud ].size.y = 150.0f;
			g_Cloud[ CntCloud ].move_X = 2.0f + rand()%3;
			g_Cloud[ CntCloud ].use = true;
		}	//	end of for
	}

	MakeVertexCloud( pDevice );	//	頂点の作成

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitCloud(void)
 引数:		なし
 戻り値:	なし
 説明:		雲の終了
-----------------------------------------------------------------------------*/
void UninitCloud(void)
{
	if( g_pTextureCloud != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureCloud -> Release();
		g_pTextureCloud = NULL;
	}	//	end of if
	if(g_pVtxBufferCloud != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferCloud -> Release();
		g_pVtxBufferCloud = NULL;
	}	//	end of if

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateCloud(void)
 引数:		なし
 戻り値:	なし
 説明:		雲の更新
-----------------------------------------------------------------------------*/
void UpdateCloud(void)
{
	int CntCloud;

	VERTEX_2D* pVtx;

	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferCloud -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
	{
		if( g_Cloud[ CntCloud ].use = true )
		{
			//	更新時の雲の動き
			UpdateCloudAct( pVtx , CntCloud );

		}	//	end of if
	}	//	end of for

	g_pVtxBufferCloud -> Unlock(); //これ以降触れてはいけない

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawCloud(void)
 引数:		なし
 戻り値:	なし
 説明:		雲の描画
-----------------------------------------------------------------------------*/
void DrawCloud(void)
{
	int CntCloud;

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferCloud , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureCloud );

	for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
	{
		if( g_Cloud[CntCloud].use == true )
		{
			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntCloud * NUM_VERTEX , NUM_POLYGON);
		}	//	end of if
	}	//	end of for

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		雲の描画
-----------------------------------------------------------------------------*/
HRESULT MakeVertexCloud( LPDIRECT3DDEVICE9 pDevice )
{
	int CntCloud;

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_CLOUD  , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferCloud , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferCloud -> Lock ( 0 , 0 , ( void** )&pVtx ,  0 );

	for( CntCloud = 0 ; CntCloud < MAX_CLOUD ; CntCloud++ )
	{
		//	頂点座標の設定
		pVtx[ CntCloud*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y , 0.0f );
		pVtx[ CntCloud*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y , 0.0f );
		pVtx[ CntCloud*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );
		pVtx[ CntCloud*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );

		//	座標変換済み頂点フラグの設定
		pVtx[ CntCloud*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+3 ].rhw = 1.0f;

		//	座標変換済み頂点フラグの設定
		pVtx[ CntCloud*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ CntCloud*NUM_VERTEX+3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ CntCloud*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntCloud*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntCloud*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ CntCloud*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ CntCloud*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ CntCloud*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ CntCloud*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ CntCloud*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	g_pVtxBufferCloud -> Unlock();

	return S_OK;

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud )
 引数:		VERTEX_2D* pVtx
			int CntCloud
 戻り値:	なし
 説明:		更新時の雲の動き
-----------------------------------------------------------------------------*/
void UpdateCloudAct( VERTEX_2D* pVtx , int CntCloud )
{

	g_Cloud[ CntCloud ].pos.x -= g_Cloud[ CntCloud ].move_X;

	//	画面外処理
	if( g_Cloud[ CntCloud ].pos.x + g_Cloud[ CntCloud ].size.x < 0 )
	{
		g_Cloud[ CntCloud ].pos.x = SCREEN_WIDTH;
	}	//	end of if

	MakeVertexCloudPos( pVtx , CntCloud );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud )
 引数:		VERTEX_2D* pVtx
			int CntCloud
 戻り値:	なし
 説明:		座標の更新
-----------------------------------------------------------------------------*/
void MakeVertexCloudPos( VERTEX_2D* pVtx , int CntCloud )
{
	pVtx += CntCloud * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x                            , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Cloud[CntCloud].pos.x + g_Cloud[CntCloud].size.x , g_Cloud[CntCloud].pos.y+g_Cloud[CntCloud].size.y , 0.0f );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	CLOUD *GetCloud( void )
 引数:		
 戻り値:	なし
 説明:		雲の情報の取得
-----------------------------------------------------------------------------*/
CLOUD *GetCloud( void )
{
	return &g_Cloud[ 0 ];
}	//	end of func