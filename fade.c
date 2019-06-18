/*=============================================================================

		フェード処理[ fade.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/07/04
-------------------------------------------------------------------------------
	■　Update
		2016/04/18
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define FADE_TEXTURENAME "data/TEXTURE/func/fade.png"

#define FADE_POS_X ( 0.0f )	//	フェードの表示位置Ｘ
#define FADE_POS_Y ( 0.0f )	//	フェードの表示位置Ｙ

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexFade( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void SetColorFade(D3DXCOLOR g_colorFade);	//	頂点カラーの設定
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;//	テクスチャインターフェース

D3DXCOLOR g_colorFade;	//	フェード色
FADE g_fade;	//	フェード状態
MODE g_modenext = MODE_TITLE;	//	次のモード

bool g_FadeUse = false;
/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitFade( void )
 引数:		なし
 戻り値:	なし
 説明:		フェードの初期化
-----------------------------------------------------------------------------*/
void InitFade( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , FADE_TEXTURENAME , &g_pTextureFade  ) ) )
	{
		MessageBox( NULL , "テクスチャの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}

	MakeVertexFade( pDevice );	//	頂点の作成

	g_colorFade = D3DXCOLOR(0.0f , 0.0f , 0.0f , 1.0f);
	g_fade = FADE_IN;

	g_FadeUse = false;
}

/*-----------------------------------------------------------------------------
 関数名:	void UninitFade( void )
 引数:		なし
 戻り値:	なし
 説明:		フェードの終了
-----------------------------------------------------------------------------*/
void UninitFade( void )
{
	if( g_pTextureFade != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureFade -> Release();
		g_pTextureFade = NULL;
	}
	if(g_pVtxBufferFade != NULL)	//	頂点バッファの開放
	{
		g_pVtxBufferFade -> Release();
		g_pVtxBufferFade = NULL;
	}
}
/*-----------------------------------------------------------------------------
 関数名:	void UpdataFade( void )
 引数:		なし
 戻り値:	なし
 説明:		フェードの更新
-----------------------------------------------------------------------------*/
void UpdateFade( void )
{

	if(g_fade == FADE_NONE)	//	フェードがない時の処理
	{
		return;
	}
	if(g_fade == FADE_IN)	//	フェードイン時の処理
	{
		g_colorFade.a -= FADE_RATE;	//	a値を減算して後ろの画面を浮かび上がらせる
		//	フェードインの終了
		if(g_colorFade.a < 0.0f)
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;

			g_FadeUse = false;
		}
	}
	else if(g_fade == FADE_OUT)	//	フェードアウト時の処理
	{
		g_colorFade.a += FADE_RATE;	//	a値を加算して後ろの画面を消していく

		//	フェードアウトの終了
		if(g_colorFade.a > 1.0f)
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;	//	フェードインに処理の切り替え
			SetMode(g_modenext);

		}
	}
	//	頂点カラーの設定
	SetColorFade(g_colorFade);

}
/*-----------------------------------------------------------------------------
 関数名:	void DrawFade( void )
 引数:		なし
 戻り値:	なし
 説明:		フェードの描画
-----------------------------------------------------------------------------*/
void DrawFade( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferFade , 0 , sizeof( VERTEX_2D ) );

	//	テクスチャ設定
	pDevice -> SetTexture( 0 , g_pTextureFade );

	//pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );
	//pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	//pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	//	フェードの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
}

/*-----------------------------------------------------------------------------
 関数名:	void MakeVertexFade( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexFade( LPDIRECT3DDEVICE9 pDevice )
{
	
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferFade , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferFade -> Lock ( 0 ,
		                          0 ,
								  ( void** )&pVtx ,      // 仮想(疑似)アドレス取得
								  0 );

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( FADE_POS_X , FADE_POS_Y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( FADE_POS_X+SCREEN_WIDTH , FADE_POS_Y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( FADE_POS_X , FADE_POS_Y+SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( FADE_POS_X+SCREEN_WIDTH , FADE_POS_Y+SCREEN_HEIGHT , 0.0f );


	//	座標変換済み頂点フラグの設定
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferFade -> Unlock();

	return S_OK;

}
/*-----------------------------------------------------------------------------
 関数名:	void SetColorFade(D3DXCOLOR g_colorFade)
 引数:		D3DXCOLOR g_colorFade
 戻り値:	なし
 説明:		頂点カラーの設定
-----------------------------------------------------------------------------*/
void SetColorFade(D3DXCOLOR colorFade)
{
	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferFade -> Lock ( 0 ,
		                          0 ,
								  ( void** )&pVtx ,      // 仮想(疑似)アドレス取得
								  0 );
	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );
	pVtx[ 1 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );
	pVtx[ 2 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );
	pVtx[ 3 ].color = D3DXCOLOR( colorFade.r , colorFade.g , colorFade.b , colorFade.a );

	g_pVtxBufferFade -> Unlock();

}
/*-----------------------------------------------------------------------------
 関数名:	void SetFade( FADE fade , MODE modenext )
 引数:		FADE fade
			MODE modenext
 戻り値:	なし
 説明:		フェードの設定
-----------------------------------------------------------------------------*/
void SetFade( FADE fade , MODE modenext )
{
	g_fade = fade;
	g_modenext = modenext;

	g_FadeUse = true;

}
/*-----------------------------------------------------------------------------
 関数名:	MODE *GetNextMode( void )
 引数:		なし
 戻り値:	return &g_modenext;
 説明:		次のモードの追加
-----------------------------------------------------------------------------*/
MODE *GetNextMode( void )
{
	return &g_modenext;
}

/*-----------------------------------------------------------------------------
 関数名:	bool *GetFadeUse( void )
 引数:		なし
 戻り値:	return &g_FadeUse;
 説明:		フェード中かどうか
-----------------------------------------------------------------------------*/
bool *GetFadeUse( void )
{
	return &g_FadeUse;
}