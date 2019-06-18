/*=============================================================================

		背景描画[ bg.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/07/28
-------------------------------------------------------------------------------
	■　Update
		2016/07/28
			・ヘッダファイル追加
				#include <stdio.h>
				#include "main.h"
				#include "bg.h"
			・定数定義追加
				#define BG_TEXTURENAME
				#define BG_POS_X ( 0.0f )	//	背景の表示位置Ｘ
				#define BG_POS_Y ( 0.0f )	//	背景の表示位置Ｙ
			・グローバル変数追加
				LPDIRECT3DTEXTURE9 g_pTexturebg = NULL;//	テクスチャインターフェース
				LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBg = NULL;	//頂点バッファのインターフェースポインタ
			・プロトタイプ宣言追加
				HRESULT MakeVertexBg( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "bg.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define BG_TEXTURENAME "data/TEXTURE/func/bg.jpg"

#define BG_POS_X ( 0.0f )	//	背景の表示位置Ｘ
#define BG_POS_Y ( 0.0f )	//	背景の表示位置Ｙ
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTexturebg = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBg = NULL;	//頂点バッファのインターフェースポインタ

float g_BgTex = 0.0f;
float g_Vset = 0.0f;
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBg( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void MakeBgTex( VERTEX_2D* pVtx );	//	背景のテクスチャ座標変更
/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitBg( void )
 引数:		なし
 戻り値:	なし
 説明:		ポリゴンの初期化
-----------------------------------------------------------------------------*/
void InitBg( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック	１つ目
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , BG_TEXTURENAME , &g_pTexturebg  ) ) )
	{
		MessageBox( NULL , "背景の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}
	MakeVertexBg( pDevice );	//	頂点の作成
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitBg( void )
 引数:		なし
 戻り値:	なし
 説明:		ポリゴンの終了
-----------------------------------------------------------------------------*/
void UninitBg( void )
{
	//	初期化
	g_BgTex = 0.0f;
	g_Vset = 0.0f;


	if( g_pTexturebg != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturebg -> Release();
		g_pTexturebg = NULL;
	}	//	end of if
	if(g_pVtxBufferBg != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferBg -> Release();
		g_pVtxBufferBg  = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdataBg( void )
 引数:		なし
 戻り値:	なし
 説明:		ポリゴンの更新
-----------------------------------------------------------------------------*/
void UpdateBg( void )
{
	VERTEX_2D* pVtx;

	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferBg -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	MakeBgTex( pVtx );

	g_pVtxBufferBg -> Unlock(); //これ以降触れてはいけない
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawBg( void )
 引数:		なし
 戻り値:	なし
 説明:		ポリゴンの描画
-----------------------------------------------------------------------------*/
void DrawBg( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferBg , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTexturebg );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void MakeVertexBg( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBg( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferBg , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferBg -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( BG_POS_X , BG_POS_Y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( BG_POS_X+SCREEN_WIDTH , BG_POS_Y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( BG_POS_X , BG_POS_Y+SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( BG_POS_X+SCREEN_WIDTH , BG_POS_Y+SCREEN_HEIGHT , 0.0f );

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

	g_pVtxBufferBg -> Unlock();

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void MakeBgTex( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		背景のテクスチャ座標変更
-----------------------------------------------------------------------------*/
void MakeBgTex( VERTEX_2D* pVtx )
{
	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 + g_BgTex , 0 + g_Vset );
	pVtx[ 1 ].tex = D3DXVECTOR2( 0.01f + g_BgTex , 0 + g_Vset );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 + g_BgTex , 0.1f + g_Vset );
	pVtx[ 3 ].tex = D3DXVECTOR2( 0.01f + g_BgTex , 0.1f + g_Vset );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	float *GetBgTex(void)
 引数:		return &g_BgTex;
 戻り値:	なし
 説明:		背景のテクスチャU座標取得
-----------------------------------------------------------------------------*/
float *GetBgTex(void)
{
	return &g_BgTex;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	float *GetVTex( void )
 引数:		return &g_Vset;
 戻り値:	なし
 説明:		背景のテクスチャV座標取得
-----------------------------------------------------------------------------*/
float *GetVTex( void )
{
	return &g_Vset;
}