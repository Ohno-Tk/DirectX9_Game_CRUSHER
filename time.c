/*=============================================================================

		タイム[ time.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/07/04
-------------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "time.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define TIME_TEXTURENAME "data/TEXTURE/game/number01.png"

#define TIME_DIGIT (4)	//	何桁の数字の表示
#define TIME_MIN (0)	//	タイムの最小値
#define SECOND ( 401 )	//	秒数
#define FRAME (60)	//	フレーム数


#define TIME_X ( SCREEN_WIDTH - 75.0f )	//	タイムのX座標
#define TIME_Y (50.0f)	//	タイムのY座標
#define TIME_WIDTH (50.0f)	//	タイムのWidth
#define TIME_HEIGHT (50.0f)	//	タイムのHeight

#define TIME_ASCII_X ( 750.0f )	//	タイム文字のX座標
#define TIME_ASCII_Y ( 0.0f )	//	タイム文字のY座標
#define TIME_ASCII_WIDTH ( 200.0f )	//	タイム文字のWidth
#define TIME_ASCII_HEIGHT ( 80.0f )	//	タイム文字のHeight
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void MakeTime( VERTEX_2D* pVtx );	//	タイムのテクスチャ更新
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;	//頂点バッファのインターフェースポインタ

int g_Time;
/*-----------------------------------------------------------------------------
 関数名:	void InitTime( void )
 引数:		なし
 戻り値:	なし
 説明:		タイムの初期化
-----------------------------------------------------------------------------*/
void InitTime( void )
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	MakeVertexTime( pDevice );	//	頂点の作成

	//	時間の初期化処理
	g_Time = SECOND*FRAME;

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , TIME_TEXTURENAME , &g_pTextureTime  ) ) )
	{
		MessageBox( NULL , "タイムの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitTime( void )
 引数:		なし
 戻り値:	なし
 説明:		タイムの終了
-----------------------------------------------------------------------------*/
void UninitTime( void )
{
	if( g_pTextureTime != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureTime -> Release();
		g_pTextureTime = NULL;
	}
	if(g_pVtxBufferTime != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferTime -> Release();
		g_pVtxBufferTime  = NULL;
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdeteTime( void )
 引数:		なし
 戻り値:	なし
 説明:		タイムの更新
-----------------------------------------------------------------------------*/
void UpdateTime( void )
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTime -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	タイムが0以下になったら
	if( g_Time <= 0 )
	{
		g_Time = TIME_MIN;
	}
	else
	{
		g_Time--;	//	タイムを減らす
	}

	MakeTime(pVtx);
	g_pVtxBufferTime -> Unlock();
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawTime( void )
 引数:		なし
 戻り値:	なし
 説明:		タイムの描画
-----------------------------------------------------------------------------*/
void DrawTime( void )
{
	int loop;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTime , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureTime );

	for(loop = 0;loop < TIME_DIGIT;loop++)
	{
		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , loop*4, NUM_POLYGON);
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
{
	int i;
	int num;	//	数字を表示する変数
	int value = g_Time/60;	//	タイムの値
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * TIME_DIGIT , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTime , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTime -> Lock ( 0 ,
		                          0 ,
								  ( void** )&pVtx ,      // 仮想(疑似)アドレス取得
								  0 );


	for(i = 0;i < TIME_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	頂点座標の設定
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );

		//	座標変換済み頂点フラグの設定
		pVtx[ i*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ i*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ i*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ i*NUM_VERTEX+3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ i*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ i*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ i*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ i*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.1f*num , 0 );
		pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0 );
		pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.1f*num , 0.5f );
		pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0.5f );

		//	タイム文字
		if(i == TIME_DIGIT-1)
		{
			//	頂点座標の設定
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );

			//	座標変換済み頂点フラグの設定
			pVtx[ i*NUM_VERTEX+0 ].rhw = 1.0f;
			pVtx[ i*NUM_VERTEX+1 ].rhw = 1.0f;
			pVtx[ i*NUM_VERTEX+2 ].rhw = 1.0f;
			pVtx[ i*NUM_VERTEX+3 ].rhw = 1.0f;

			//	頂点色の設定
			pVtx[ i*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
			pVtx[ i*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
			pVtx[ i*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
			pVtx[ i*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

			//	テクスチャ座標の設定
			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.3f , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.3f , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
		}

	}

	g_pVtxBufferTime -> Unlock();

	return S_OK;

}
/*-----------------------------------------------------------------------------
 関数名:	void MakeTime( void )
 引数:		
 戻り値:	なし
 説明:		タイムのテクスチャ更新
-----------------------------------------------------------------------------*/
void MakeTime( VERTEX_2D* pVtx )
{
	int i;
	int num;	//	数字を表示する変数
	int value = g_Time/60;	//	タイムの値

	for(i = 0;i < TIME_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	頂点座標の設定
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_X-i*TIME_WIDTH+TIME_WIDTH , TIME_Y+TIME_HEIGHT , 0.0f );

		//	テクスチャ座標の設定
		pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.1f*num , 0 );
		pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0 );
		pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.1f*num , 0.5f );
		pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0.5f );

		//	タイム文字
		if(i == TIME_DIGIT-1)
		{
			//	頂点座標の設定
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( TIME_ASCII_X , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( TIME_ASCII_X+TIME_ASCII_WIDTH , TIME_ASCII_Y+TIME_ASCII_HEIGHT , 0.0f );

			//	テクスチャ座標の設定
			//pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.3f , 0.5f );
			//pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			//pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.3f , 1.0f );
			//pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );

			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1.0f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1.0f , 1.0f );
		}
	}

}
/*-----------------------------------------------------------------------------
 関数名:	int GetTime( void )
 引数:		
 戻り値:	return g_Time;
 説明:		タイムの情報の取得
-----------------------------------------------------------------------------*/
int *GetTime( void )
{
	return &g_Time;
}