/*=============================================================================

		タイム[ score.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/07/04
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "score.h"
#include "time.h"
#include "base.h"
#include "enemy.h"
#include "fade.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define SCORE_TEXTURENAME "data/TEXTURE/game/number01.png"

#define SCORE_MAX ( 99999 )	//	スコアの最大値

#define SCORE_X (200.0f)		//	スコアのX座標
#define SCORE_Y (50.0f)			//	スコアのY座標
#define SCORE_WIDTH (50.0f)		//	スコアのWidth
#define SCORE_HEIGHT (50.0f)	//	スコアのHeight

#define SCORE_ASCII_X ( 20.0f )		//	スコア文字のX座標
#define SCORE_ASCII_Y ( 0.0f )		//	スコア文字のY座標
#define SCORE_ASCII_WIDTH ( 200.0f )	//	スコア文字のWidth
#define SCORE_ASCII_HEIGHT ( 80.0f )	//	スコア文字のHeight

#define SCORE_ENEMY_DIGIT ( 3 )	//	敵のスコアの何桁の数字の表示

#define UP_SCORE_POS ( 35 )	//	どの位スコアの座標の上昇させるか
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void UpdateScoreAdd( VERTEX_2D* pVtx );	//	スコアの加算処理
void MakeScore( VERTEX_2D* pVtx );	//	スコアの作成
void UpdateEnemyScore( VERTEX_2D* pVtx , int CntEnemy );	//	敵のスコア表示更新
void EnemyScorePos( VERTEX_2D* pVtx , int IndexEnemy );	//	敵のスコアの座標変更
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScore = NULL;	//頂点バッファのインターフェースポインタ

int g_Score;	//	現在のスコア
int g_AddScore;

bool ScoreFlag = false;

D3DXCOLOR g_ScoreColor;	//	色変更

/*-----------------------------------------------------------------------------
 関数名:	void Initscore( void )
 引数:		なし
 戻り値:	なし
 説明:		タイムの初期化
-----------------------------------------------------------------------------*/
void InitScore( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , SCORE_TEXTURENAME , &g_pTextureScore  ) ) )
	{
		MessageBox( NULL , "スコアの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	初期化
	//	スコア
	g_Score = 0;
	g_AddScore = 0;

	ScoreFlag = false;

	g_ScoreColor = D3DXCOLOR( 255 , 255 , 255 , 255 );

	MakeVertexScore( pDevice );	//	頂点の作成

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void Uninitscore( void )
 引数:		なし
 戻り値:	なし
 説明:		スコアの終了
-----------------------------------------------------------------------------*/
void UninitScore( void )
{
	g_Score = 0;

	ScoreFlag = false;


	if( g_pTextureScore != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureScore -> Release();
		g_pTextureScore = NULL;
	}
	if(g_pVtxBufferScore != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferScore -> Release();
		g_pVtxBufferScore  = NULL;
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdeteScore( void )
 引数:		なし
 戻り値:	なし
 説明:		スコアの更新
-----------------------------------------------------------------------------*/
void UpdateScore( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	敵の情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferScore -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	if( *Mode == MODE_GAME )
	{
		UpdateScoreAdd( pVtx );	//	スコアの加算処理
	}

	g_pVtxBufferScore -> Unlock();
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void Drawscore( void )
 引数:		なし
 戻り値:	なし
 説明:		スコアの描画
-----------------------------------------------------------------------------*/
void DrawScore( void )
{
	int loop;

	//	敵の情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferScore , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTextureScore );

	for(loop = 0;loop < SCORE_DIGIT;loop++)
	{
		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , loop*4, NUM_POLYGON);
	}

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexScore( LPDIRECT3DDEVICE9 pDevice )
{
	int i;
	int num;	//	数字を表示する変数
	int value = g_AddScore , enemyscorevalue = 100;	//	スコアの値

	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * SCORE_DIGIT , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferScore , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferScore -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	for(i = 0;i < SCORE_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	頂点座標の設定
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );

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

		//	スコア文字の表示
		if(i == SCORE_DIGIT-1)
		{
			//	頂点座標の設定
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );

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
			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
		}
	}
	
	g_pVtxBufferScore -> Unlock();

	return S_OK;

}
/*-----------------------------------------------------------------------------
 関数名:	void UpdateScoreAdd( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		スコアの加算処理
-----------------------------------------------------------------------------*/
void UpdateScoreAdd( VERTEX_2D* pVtx )
{
	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	//	タイム情報の取得
	int *Time = GetTime();

	//	タイムが0以下になったら
	if( *Time <= 0 )
	{
		ScoreFlag = true;
	}

	//	カンスト処理
	if( g_Score > SCORE_MAX )
	{
		g_Score = SCORE_MAX;
	}

	if( g_Score > g_AddScore )
	{
		if( Base -> GoolFalg == false )
		{
			g_AddScore += 10;

			g_ScoreColor.r = 0;
			g_ScoreColor.g = 0;
			g_ScoreColor.b = 255;
		}
		else if( Base -> GoolFalg == true )
		{
			g_AddScore += 200;

			g_ScoreColor.r = 0;
			g_ScoreColor.g = 0;
			g_ScoreColor.b = 255;
		}
		//	上限処理
		if( g_Score <= g_AddScore )
		{
			g_AddScore = g_Score;
		}	//	end of if
	}	//	end of if

	MakeScore(pVtx);
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void MakeScore( void )
 引数:		
 戻り値:	なし
 説明:		スコアの作成
-----------------------------------------------------------------------------*/
void MakeScore( VERTEX_2D* pVtx )
{
	int i;
	int num;	//	数字を表示する変数
	int value = g_AddScore;

	if(value >= SCORE_MAX)	//	カンスト処理
	{
		value = SCORE_MAX;
	}
	for(i = 0;i < SCORE_DIGIT;i++)
	{
		num = value % 10;
		value /= 10;

		//	頂点座標の設定
		pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y , 0.0f );
		pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );
		pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_X-i*SCORE_WIDTH+SCORE_WIDTH , SCORE_Y+SCORE_HEIGHT , 0.0f );

		//	頂点色の設定
		pVtx[ i*NUM_VERTEX+0 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );
		pVtx[ i*NUM_VERTEX+1 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );
		pVtx[ i*NUM_VERTEX+2 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );
		pVtx[ i*NUM_VERTEX+3 ].color = D3DXCOLOR( g_ScoreColor.r , g_ScoreColor.g , g_ScoreColor.b , g_ScoreColor.a );

		//	テクスチャ座標の設定
		pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0.1f*num , 0 );
		pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0 );
		pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0.1f*num , 0.5f );
		pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.1f*num+0.1f , 0.5f );

		//	スコア文字の表示
		if(i == SCORE_DIGIT-1)
		{
			//	頂点座標の設定
			pVtx[ i*NUM_VERTEX+0 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+1 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y , 0.0f );
			pVtx[ i*NUM_VERTEX+2 ].pos = D3DXVECTOR3( SCORE_ASCII_X , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );
			pVtx[ i*NUM_VERTEX+3 ].pos = D3DXVECTOR3( SCORE_ASCII_X+SCORE_ASCII_WIDTH , SCORE_ASCII_Y+SCORE_ASCII_HEIGHT , 0.0f );

			//	テクスチャ座標の設定
			//pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0.5f );
			//pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.3f , 0.5f );
			//pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1.0f );
			//pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.3f , 1.0f );

			pVtx[ i*NUM_VERTEX+0 ].tex = D3DXVECTOR2( 0 , 0.5f );
			pVtx[ i*NUM_VERTEX+1 ].tex = D3DXVECTOR2( 0.5f , 0.5f );
			pVtx[ i*NUM_VERTEX+2 ].tex = D3DXVECTOR2( 0 , 1.0f );
			pVtx[ i*NUM_VERTEX+3 ].tex = D3DXVECTOR2( 0.5f , 1.0f );
		}
	}
}
/*-----------------------------------------------------------------------------
 関数名:	int *GetScore(void)
 引数:		
 戻り値:	なし
 説明:		スコアの情報
-----------------------------------------------------------------------------*/
int *GetScore(void)
{
	return &g_Score;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	bool *GetScoreFlag( void )
 引数:		
 戻り値:	return &ScoreFlag;
 説明:		スコアフラグの情報
-----------------------------------------------------------------------------*/
bool *GetScoreFlag( void )
{
	return &ScoreFlag;
}	//	end of func
