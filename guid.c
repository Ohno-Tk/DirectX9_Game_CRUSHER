/*=============================================================================

		プレイヤー[ player.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/07/28
-------------------------------------------------------------------------------
	■　Update
		2016/07/28
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "guid.h"
#include "base.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define GUID_TEXTURENAME "data/TEXTURE/func/guid.png"

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGuid = NULL;//頂点バッファのインターフェースポインタ
LPDIRECT3DTEXTURE9 g_pTextureGuid = NULL;//	テクスチャインターフェース

GUIDSIGNBOARD g_Guid;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexGuid( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void GuidPos( VERTEX_2D* pVtx );	//	座標変更
/*-----------------------------------------------------------------------------
 関数名:	void InitGuid( void )
 引数:		なし
 戻り値:	なし
 説明:		案内の初期化
-----------------------------------------------------------------------------*/
void InitGuid( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	足場情報の取得
	BASE *Base = GetBase( 0 );

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , GUID_TEXTURENAME , &g_pTextureGuid  ) ) )
	{
		MessageBox( NULL , "案内の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}

	//	初期化
	g_Guid.pos.x = 300.0f;
	g_Guid.pos.y = SCREEN_HEIGHT - 400.0f;
	g_Guid.size.x = 150.0f;
	g_Guid.size.y = 200.0f;
	g_Guid.move_X = Base -> move_X;

	//	頂点の作成
	MakeVertexGuid( pDevice );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitGuid( void )
 引数:		なし
 戻り値:	なし
 説明:		案内の終了
-----------------------------------------------------------------------------*/
void UninitGuid( void )
{
	//頂点バッファのインターフェースポインタの解放
	if(g_pVtxBufferGuid != NULL)
	{
		g_pVtxBufferGuid -> Release();
		g_pVtxBufferGuid = NULL;
	}	//	end of if

	//	テクスチャポリゴン開放
	if( g_pTextureGuid != NULL )
	{
		g_pTextureGuid -> Release();
		g_pTextureGuid = NULL;
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateGuid( void )
 引数:		なし
 戻り値:	なし
 説明:		案内の更新
-----------------------------------------------------------------------------*/
void UpdateGuid( void )
{
	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferGuid -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	座標変更
	GuidPos( pVtx );

	g_pVtxBufferGuid -> Unlock(); //これ以降触れてはいけない
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawGuid( void )
 引数:		なし
 戻り値:	なし
 説明:		案内の描画
-----------------------------------------------------------------------------*/
void DrawGuid( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferGuid , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureGuid );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexGuid( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexGuid( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferGuid , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferGuid -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y + g_Guid.size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y + g_Guid.size.y , 0.0f );

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


	g_pVtxBufferGuid -> Unlock(); //これ以降触れてはいけない

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void GuidPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		座標変更
-----------------------------------------------------------------------------*/
void GuidPos( VERTEX_2D* pVtx )
{
	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Guid.pos.x                 , g_Guid.pos.y + g_Guid.size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Guid.pos.x + g_Guid.size.x , g_Guid.pos.y + g_Guid.size.y , 0.0f );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	GUIDSIGNBOARD *GetGuid( void )
 引数:		
 戻り値:	return &g_Guid;
 説明:		案内情報の取得
-----------------------------------------------------------------------------*/
GUIDSIGNBOARD *GetGuid( void )
{
	return &g_Guid;
}