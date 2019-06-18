/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "effect.h"
#include "input.h"
#include "enemy.h"
#include "usability.h"
#include "score.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define EFFECT_TEXTURENAME "data/TEXTURE/game/effect000.jpg"

#define MAX_LIFE ( 30 )

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void EffectPos( VERTEX_2D* pVtx , int IndexParticle );	//	座標更新
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEffect = NULL;

EFFECT g_Effect[MAX_EFFECT];

/*-----------------------------------------------------------------------------
 関数名:	void InitEffect( void )
 引数:		なし
 戻り値:	なし
 説明:		エフェクトの初期化
-----------------------------------------------------------------------------*/
void InitEffect( void )
{
	int loop;

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , EFFECT_TEXTURENAME , &g_pTextureEffect  ) ) )
	{
		MessageBox( NULL , "エフェクトの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	パーティクルの初期化処理
	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		g_Effect[loop].pos.x = 0.0f;
		g_Effect[loop].pos.y = 0.0f;
		g_Effect[loop].size.x = 50.0f;
		g_Effect[loop].size.y = 50.0f;
		g_Effect[loop].move.x = 0.0f;
		g_Effect[loop].move.y = 5.0f;
		g_Effect[loop].color.r = 128;
		g_Effect[loop].color.g = 255;
		g_Effect[loop].color.b = 128;
		g_Effect[loop].color.a = 255;
		g_Effect[loop].length = 5.0f;
		g_Effect[loop].fRadius = 10.0f;
		g_Effect[loop].fColorAlphaValue = 5.0f;
		g_Effect[loop].nLife = MAX_LIFE;
		g_Effect[loop].bUse = false;
	}	//	end of for

	MakeVertexEffect( pDevice );	//	頂点の作成

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitEffect( void )
 引数:		なし
 戻り値:	なし
 説明:		エフェクトの終了
-----------------------------------------------------------------------------*/
void UninitEffect( void )
{
	if( g_pTextureEffect != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureEffect -> Release();
		g_pTextureEffect = NULL;
	}	//	end of if
	if(g_pVtxBufferEffect != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferEffect -> Release();
		g_pVtxBufferEffect = NULL;
	}	//	end of if
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateEffect( void )
 引数:		なし
 戻り値:	なし
 説明:		パーティクルの更新
-----------------------------------------------------------------------------*/
void UpdateEffect( void )
{
	int loop;

	//	敵情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	スコア情報の取得
	int *Score = GetScore();

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferEffect -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		if( g_Effect[loop].bUse  == true )
		{

			//	エフェクトのタイプによって更新を変える

			//	攻撃タイプだったら
			if( g_Effect[loop].type == EFFECT_TYPE_ATTACK )
			{
				g_Effect[loop].pos.x += g_Effect[loop].move.x;
			}	//	end of if

			//	寿命を減らす
			g_Effect[loop].nLife--;

			//	寿命がなくなったら
			if( g_Effect[loop].nLife <= 0 )
			{
				g_Effect[loop].bUse  = false;
				
			}	//	end of if

			//	画面外処理
			if( g_Effect[loop].pos.x > SCREEN_HEIGHT || g_Effect[loop].pos.x < 0 )
			{
				g_Effect[loop].bUse  = false;
			}

			//	座標更新
			EffectPos( pVtx , loop );

		}	//	end of if
	}	//	end of for

	g_pVtxBufferEffect -> Unlock();

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawEffect( void )
 引数:		なし
 戻り値:	なし
 説明:		エフェクトの描画
-----------------------------------------------------------------------------*/
void DrawEffect( void )
{
	int loop;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferEffect , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureEffect );

	//	aブレンド（加算合成）
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );	//	Initに書いてあるので省略可
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );	//	Initに書いてあるので省略可
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		if(g_Effect[loop].bUse  == true)
		{
			//	ポリゴンの描画
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , loop * NUM_VERTEX, NUM_POLYGON);
		}	//	end of if
	}	//	end of for

	//	元に戻す
	pDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexEffect( LPDIRECT3DDEVICE9 pDevice )
{
	int loop = 0;

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_EFFECT , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferEffect , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferEffect -> Lock ( 0 , 0 ,( void** )&pVtx , 0 );


	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		//	頂点座標の設定
		pVtx[ loop*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x , g_Effect[loop].pos.y , 0.0f );
		pVtx[ loop*NUM_VERTEX+1 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x+g_Effect[loop].size.x , g_Effect[loop].pos.y , 0.0f );
		pVtx[ loop*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x , g_Effect[loop].pos.y+g_Effect[loop].size.y , 0.0f );
		pVtx[ loop*NUM_VERTEX+3 ].pos = D3DXVECTOR3( g_Effect[loop].pos.x+g_Effect[loop].size.x , g_Effect[loop].pos.y+g_Effect[loop].size.y , 0.0f );

		//	座標変換済み頂点フラグの設定
		pVtx[ loop*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+3 ].rhw = 1.0f;

		//	座標変換済み頂点フラグの設定
		pVtx[ loop*NUM_VERTEX+0 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+1 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+2 ].rhw = 1.0f;
		pVtx[ loop*NUM_VERTEX+3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ loop*NUM_VERTEX+0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ loop*NUM_VERTEX+1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ loop*NUM_VERTEX+2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
		pVtx[ loop*NUM_VERTEX+3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

		//	テクスチャ座標の設定
		pVtx[ loop*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ loop*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ loop*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ loop*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 1 , 1 );
	}	//	end of for

	g_pVtxBufferEffect -> Unlock();

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void EffectPos( VERTEX_2D* pVtx , int IndexParticle )
 引数:		VERTEX_2D* pVtx
			int IndexParticle
 戻り値:	なし
 説明:		エフェクトの移動
-----------------------------------------------------------------------------*/
void EffectPos( VERTEX_2D* pVtx , int IndexParticle )
{
	pVtx += IndexParticle * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x , g_Effect[IndexParticle].pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x+g_Effect[IndexParticle].size.x , g_Effect[IndexParticle].pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x , g_Effect[IndexParticle].pos.y+g_Effect[IndexParticle].size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Effect[IndexParticle].pos.x+g_Effect[IndexParticle].size.x , g_Effect[IndexParticle].pos.y+g_Effect[IndexParticle].size.y , 0.0f );

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Effect[IndexParticle].color.r , g_Effect[IndexParticle].color.g , g_Effect[IndexParticle].color.b , g_Effect[IndexParticle].color.a );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void SetParticle( float PosX , float PosY )
 引数:		float PosX
			float PosY
 戻り値:	なし
 説明:		エフェクトの設置
-----------------------------------------------------------------------------*/
void SetEffect( float PosX , float PosY , float Speed , EFFECT_TYPE type )
{
	int loop;

	for(loop = 0;loop < MAX_EFFECT;loop++)
	{
		if( g_Effect[loop].bUse == false )
		{
			g_Effect[loop].pos.x = PosX;
			g_Effect[loop].pos.y = PosY;
			g_Effect[loop].move.x = Speed;
			g_Effect[loop].nLife = MAX_LIFE;
			g_Effect[loop].type = type;
			g_Effect[loop].bUse = true;

			//	エフェクトタイプによってWidthとHeightの変更
			if( g_Effect[loop].type == EFFECT_TYPE_ATTACK )
			{
				g_Effect[loop].size.x = 50.0f;
				g_Effect[loop].size.y = 10.0f;
			}	//	end of if

			break;
		}	//	end of if
	}	//	end of for
}	//	end of func
EFFECT *GetEffect( void )
{
	return &g_Effect[0];
}