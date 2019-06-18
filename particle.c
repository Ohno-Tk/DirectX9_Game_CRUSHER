/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "main.h"
#include "score.h"
#include "particle.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define PARTICLE_TEXTURENAME "data/TEXTURE/game/effect001.png"

#define MAX_PARTICLE ( 70 )

#define PARTICLE_SPEED ( 5.0f )	//	パーティクルスピード
/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR2 pos;	//	座標
	D3DXVECTOR2 size;	//	大きさ
	D3DXVECTOR2 move;	//	移動量
	D3DXVECTOR2 rot;	//	向き
	D3DXCOLOR color;	//	カラー
	float length;	//	長さ
	float fRadius;	//	半径
	float fRadiusValue;	//半径の変化量
	float fColorAlphaValue;	//	色の変化量
	int nLife;	//	寿命
	bool bUse;	//	使用フラグ
}PARTICLE;
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void ParticlePos( VERTEX_2D* pVtx , int IndexParticle );	//	座標更新
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferParticle = NULL;

PARTICLE g_Particle[MAX_PARTICLE];

/*-----------------------------------------------------------------------------
 関数名:	void Initparticle( void )
 引数:		なし
 戻り値:	なし
 説明:		パーティクルの初期化
-----------------------------------------------------------------------------*/
void Initparticle( void )
{
	int loop;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PARTICLE_TEXTURENAME , &g_pTextureParticle  ) ) )
	{
		MessageBox( NULL , "パーティクルの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	パーティクルの初期化処理
	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		g_Particle[loop].pos.x = 0.0f;
		g_Particle[loop].pos.y = 0.0f;
		g_Particle[loop].size.x = 0.0f;
		g_Particle[loop].size.y = 0.0f;
		g_Particle[loop].move.x = 0.0f;
		g_Particle[loop].move.y = 0.0f;
		g_Particle[loop].color.r = 255;
		g_Particle[loop].color.g = 255;
		g_Particle[loop].color.b = 0;
		g_Particle[loop].color.a = 256;
		g_Particle[loop].length = 5.0f;
		g_Particle[loop].fRadius = 30.0f;
		g_Particle[loop].fColorAlphaValue = 5.0f;
		g_Particle[loop].nLife = 0;
		g_Particle[loop].bUse = false;
	}	//	end of for

	MakeVertexParticle( pDevice );	//	頂点の作成

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void Uninitparticle( void )
 引数:		なし
 戻り値:	なし
 説明:		パーティクルの終了
-----------------------------------------------------------------------------*/
void Uninitparticle( void )
{
	if( g_pTextureParticle != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureParticle -> Release();
		g_pTextureParticle = NULL;
	}	//	end of if
	if(g_pVtxBufferParticle != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferParticle -> Release();
		g_pVtxBufferParticle = NULL;
	}	//	end of if
}
/*-----------------------------------------------------------------------------
 関数名:	void Updateparticle( void )
 引数:		なし
 戻り値:	なし
 説明:		パーティクルの更新
-----------------------------------------------------------------------------*/
void Updateparticle( void )
{
	int loop;

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferParticle -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		if( g_Particle[loop].bUse == true )
		{
			//	ハエっぽい
			//g_Particle[loop].move.x = sinf( ( ( rand()%628 ) - 314 ) / 100 ) * g_Particle[loop].length;
			//g_Particle[loop].move.y = cosf( ( ( rand()%628 ) - 314 ) / 100 ) * g_Particle[loop].length;

			g_Particle[loop].pos.x += g_Particle[loop].move.x;
			g_Particle[loop].pos.y += g_Particle[loop].move.y;

			g_Particle[loop].nLife--;	//	ライフを減らす

			g_Particle[loop].fRadius -= g_Particle[loop].fRadiusValue;

			//	ライフがなくなったら
			if( g_Particle[loop].nLife <= 0 )
			{
				g_Particle[loop].bUse  = false;
			}	//	end of if

			//	画面外処理
			if( g_Particle[loop].pos.x + g_Particle[loop].size.x < 0 || g_Particle[loop].pos.x > SCREEN_WIDTH || g_Particle[loop].pos.y + g_Particle[loop].size.x < 0 || g_Particle[loop].pos.y > SCREEN_HEIGHT )
			{
				g_Particle[loop].bUse  = false;
			}	//	end of if

			ParticlePos( pVtx , loop );	//	座標更新
		}	//	end of if
	}	//	end of for

	g_pVtxBufferParticle -> Unlock();

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void Drawparticle( void )
 引数:		なし
 戻り値:	なし
 説明:		パーティクルの描画
-----------------------------------------------------------------------------*/
void Drawparticle( void )
{
	int loop;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferParticle , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureParticle );

	//	aブレンド（加算合成）
	pDevice -> SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );	//	Initに書いてあるので省略可
	pDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );	//	Initに書いてあるので省略可
	pDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );

	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		if(g_Particle[loop].bUse  == true)
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
 関数名:	HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		パーティクルの描画
-----------------------------------------------------------------------------*/
HRESULT MakeVertexParticle( LPDIRECT3DDEVICE9 pDevice )
{
	int loop = 0;

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_PARTICLE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferParticle , NULL ) ) )
	{
		return E_FAIL;
	}	 //	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferParticle -> Lock ( 0 , 0 ,( void** )&pVtx , 0 );


	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		//	頂点座標の設定
		pVtx[ loop*NUM_VERTEX+0 ].pos = D3DXVECTOR3( g_Particle[loop].pos.x - g_Particle[loop].fRadius                                        , g_Particle[loop].pos.y - g_Particle[loop].fRadius                                        , 0.0f );
		pVtx[ loop*NUM_VERTEX+1 ].pos = D3DXVECTOR3( ( g_Particle[loop].pos.x + g_Particle[loop].size.x ) - g_Particle[loop].fRadius , g_Particle[loop].pos.y - g_Particle[loop].fRadius                                        , 0.0f );
		pVtx[ loop*NUM_VERTEX+2 ].pos = D3DXVECTOR3( g_Particle[loop].pos.x - g_Particle[loop].fRadius                                        , ( g_Particle[loop].pos.y + g_Particle[loop].size.y ) - g_Particle[loop].fRadius , 0.0f );
		pVtx[ loop*NUM_VERTEX+3 ].pos = D3DXVECTOR3( ( g_Particle[loop].pos.x + g_Particle[loop].size.x ) - g_Particle[loop].fRadius , ( g_Particle[loop].pos.y + g_Particle[loop].size.y ) - g_Particle[loop].fRadius , 0.0f );

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

	g_pVtxBufferParticle -> Unlock();

	return S_OK;
}
/*-----------------------------------------------------------------------------
 関数名:	void ParticlePos( VERTEX_2D* pVtx , int IndexParticle )
 引数:		VERTEX_2D* pVtx
			int IndexParticle
 戻り値:	なし
 説明:		パーティクルの座標更新
-----------------------------------------------------------------------------*/
void ParticlePos( VERTEX_2D* pVtx , int IndexParticle )
{
	pVtx += IndexParticle * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Particle[IndexParticle].pos.x - g_Particle[IndexParticle].fRadius                                        , g_Particle[IndexParticle].pos.y - g_Particle[IndexParticle].fRadius                                        , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( ( g_Particle[IndexParticle].pos.x + g_Particle[IndexParticle].size.x ) - g_Particle[IndexParticle].fRadius , g_Particle[IndexParticle].pos.y - g_Particle[IndexParticle].fRadius                                        , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Particle[IndexParticle].pos.x - g_Particle[IndexParticle].fRadius                                        , ( g_Particle[IndexParticle].pos.y + g_Particle[IndexParticle].size.y ) - g_Particle[IndexParticle].fRadius , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( ( g_Particle[IndexParticle].pos.x + g_Particle[IndexParticle].size.x ) - g_Particle[IndexParticle].fRadius , ( g_Particle[IndexParticle].pos.y + g_Particle[IndexParticle].size.y ) - g_Particle[IndexParticle].fRadius , 0.0f );

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Particle[IndexParticle].color.r , g_Particle[IndexParticle].color.g , g_Particle[IndexParticle].color.b , g_Particle[IndexParticle].color.a );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void SetParticle( float PosX , float PosY )
 引数:		float PosX
			float PosY
 戻り値:	なし
 説明:		パーティクルのセット
-----------------------------------------------------------------------------*/
void SetParticle( float PosX , float PosY )
{
	int loop;

	for(loop = 0;loop < MAX_PARTICLE;loop++)
	{
		if( g_Particle[loop].bUse == false )
		{

			g_Particle[loop].pos.x = PosX + rand()%20;
			g_Particle[loop].pos.y = PosY + rand()%20;

			g_Particle[loop].move.x = sinf( ( ( rand()%628 ) - 314 ) / 100 ) * ( g_Particle[loop].length + rand()%4 );
			g_Particle[loop].move.y = cosf( ( ( rand()%628 ) - 314 ) / 100 ) * ( g_Particle[loop].length + rand()%4 );

			g_Particle[loop].size.x = 50.0f;
			g_Particle[loop].size.y = 50.0f;

			g_Particle[loop].nLife = 25 + rand()%5;

			g_Particle[loop].fRadiusValue = g_Particle[loop].fRadius / g_Particle[loop].nLife;
			g_Particle[loop].fColorAlphaValue = 1.0f / g_Particle[loop].nLife;
			g_Particle[loop].bUse = true;

			break;
		}	//	end of if
	}	//	end of for
}	//	end of func