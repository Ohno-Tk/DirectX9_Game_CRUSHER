/*=============================================================================

		ランキング[ ranking.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/09/05
-------------------------------------------------------------------------------
	■　作成日
		2016/09/05
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
		scanf のwarning防止 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "result.h"
#include "ranking.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define ENABLE_PEOC_SAVE	//	セーブ処理有効
#undef ENABLE_PEOC_SAVE	//	( マクロを無効にする )

#ifdef ENABLE_PEOC_SAVE
	#define FILENAME "data/RANKING_DATA/save.txt"	//	セーブする名前
#else
	#define FILENAME "data/RANKING_DATA/save.bin"	//	セーブする名前
#endif

//	数字
#define RANKING_SCORE_TEXTURENAME "data/TEXTURE/ranking/number000.png"
//	順位
#define RANKING_RANK_TEXTURENAME "data/TEXTURE/ranking/ranking_rank.png"

//	ランキング背景
#define RANKING_BG_TEXTURENAME "data/TEXTURE/func/titleBg.jpg"
//	ランキング文字
#define RANKING_ASCII_TEXTURENAME "data/TEXTURE/ranking/RankingAscii.png"
//	ボタン
#define RANKING_PUSH_TEXTURENAME "data/TEXTURE/func/ButtomRogo .png"

#define RANKING_PAD_TEXTURENAME "data/TEXTURE/func/ButtomPad.png"

#define MAX_RANKING ( 5 )	//	ランキングの最大数

#define TEXTURE ( 3 )	//	数字と順位を除いたテクスチャの数

#define RANKING_X ( 750.0f )	//	ランキング数字のX
#define RANKING_Y ( 500.0f )	//	ランキング数字のY
#define RANKING_WIDTH ( 100.0f )	//	ランキング数字のWIDTH
#define RANKING_HEIGHT ( 50.0f )	//	ランキング数字のHEIGHT

#define RANKING_ASCII_X ( 300.0f )	//	ランキング文字のX
#define RANKING_ASCII_Y ( 0.0f )	//	ランキング文字のY
#define RANKING_ASCII_WIDTH ( 400.0f )	//	ランキング文字のWIDTH
#define RANKING_ASCII_HEIGHT ( 100.0f )	//	ランキング文字のHEIGHT

#define RANKING_RANK_X ( 100.0f )	//	順位のX
#define RANKING_RANK_Y ( RANKING_Y )	//	順位のY
#define RANKING_RANK_WIDTH ( 80.0f )	//	順位のWIDTH
#define RANKING_RANK_HEIGHT ( 100.0f )	//	順位のX
/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexRanking( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void RankingButtomFade( VERTEX_2D* pVtx );	//	ボタンのフェード
void RankTex( VERTEX_2D* pVtx );	//	順位のテクスチャと色を変更
void Save( void );	//	セーブ
void Load( void );	//	ロード
void BubbleSort( void );	//	バブルソート
/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRanking = NULL;	//	頂点バッファ

//	ランキング数字
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;//	テクスチャインターフェース

//	ランキング背景
LPDIRECT3DTEXTURE9 g_pTextureRankingBg = NULL;//	テクスチャインターフェース

//	ランキング文字
LPDIRECT3DTEXTURE9 g_pTextureRankingAscii = NULL;//	テクスチャインターフェース

//	順位
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;//	テクスチャインターフェース

//	ボタン
LPDIRECT3DTEXTURE9 g_pTextureRankingButtom = NULL;//	テクスチャインターフェース

RANKING g_RankingList[ MAX_RANKING ] = { 0 };

FADE g_RankingFade = FADE_IN;	//	フェード状態
D3DXCOLOR g_RankingFadeColor;	//	フェード色
/*-----------------------------------------------------------------------------
 関数名:	HRESULT InitRanking( void )
 引数:		なし
 戻り値:	なし
 説明:		ランキングの初期化
-----------------------------------------------------------------------------*/
void InitRanking( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	スコアの取得
	int *Hiscore = GetScore();


	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_SCORE_TEXTURENAME , &g_pTextureRanking  ) ) )
	{
		MessageBox( NULL , "ナンバーの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_BG_TEXTURENAME , &g_pTextureRankingBg  ) ) )
	{
		MessageBox( NULL , "ランキング背景の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_ASCII_TEXTURENAME , &g_pTextureRankingAscii  ) ) )
	{
		MessageBox( NULL , "ランキング文字の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_RANK_TEXTURENAME , &g_pTextureRankingRank  ) ) )
	{
		MessageBox( NULL , "順位の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

	//	ゲームパッドが無かったら
	if( GamePad[ 0 ] == NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_PUSH_TEXTURENAME , &g_pTextureRankingButtom  ) ) )
		{
			MessageBox( NULL , "ボタン０１の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}

	//	ゲームパッドがあったら
	else if( GamePad[ 0 ] != NULL )
	{
		if( FAILED( D3DXCreateTextureFromFile( pDevice , RANKING_PAD_TEXTURENAME , &g_pTextureRankingButtom  ) ) )
		{
			MessageBox( NULL , "ボタン０２の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
		}	//	end of if
	}


#ifdef _DEBUG
	for( int Cnt2 = 0 ; Cnt2 < MAX_RANKING ; Cnt2++ )
	{
		g_RankingList[ Cnt2 ].score = 0;
	}

	//	セーブ
	Save();
#endif
	
	//	ロード
	Load();

	//	バブルソート
	BubbleSort();

	//	ランキングの一番下とゲームスコアの比較
	if( g_RankingList[ 0 ].score <= *Hiscore )
	{
	//	//	名前の入力
	//	g_RankingList[ MAX_RANKING - 1 ].name[ 0 ] = 0;
	//	g_RankingList[ MAX_RANKING - 1 ].name[ 0 ] = 10;
	//	g_RankingList[ MAX_RANKING - 1 ].name[ 0 ] = 17;

		//	ランキングの一番下にハイスコアを入れる
		g_RankingList[ 0 ].score = *Hiscore;

		//	バブルソート
		BubbleSort();
	}	//	end of if

	//	セーブ
	Save();
		

	g_RankingFadeColor = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//	頂点の作成
	MakeVertexRanking( pDevice );

	//	音楽再生
	PlaySound( SOUND_LABEL_BGM_RESULT );

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UninitRanking( void )
 引数:		なし
 戻り値:	なし
 説明:		ランキングの終了
-----------------------------------------------------------------------------*/
void UninitRanking( void )
{

	if(g_pVtxBufferRanking != NULL)	//	頂点バッファの開放
	{
		g_pVtxBufferRanking -> Release();
		g_pVtxBufferRanking = NULL;
	}	//	end of if

	//	ランキング数字
	if( g_pTextureRanking != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureRanking -> Release();
		g_pTextureRanking = NULL;
	}	//	end of if

	//	ランキング背景
	if( g_pTextureRankingBg != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureRankingBg -> Release();
		g_pTextureRankingBg = NULL;
	}	//	end of if

	//	ランキング文字
	if( g_pTextureRankingAscii != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureRankingAscii -> Release();
		g_pTextureRankingAscii = NULL;
	}	//	end of if

	//	順位
	if( g_pTextureRankingRank != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureRankingRank -> Release();
		g_pTextureRankingRank = NULL;
	}	//	end of if

	//	ボタン
	if( g_pTextureRankingButtom != NULL )	//	テクスチャポリゴン開放
	{
		g_pTextureRankingButtom -> Release();
		g_pTextureRankingButtom = NULL;
	}	//	end of if

	UninitScore();	//	スコアの終了
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void UpdateRanking( void )
 引数:		なし
 戻り値:	なし
 説明:		ランキングの更新
-----------------------------------------------------------------------------*/
void UpdateRanking( void )
{
	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	フェード中かどうか
	bool *FadeUse = GetFadeUse();

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferRanking -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	//	順位のテクスチャと色を変更
	RankTex( pVtx );

	//	ボタンのフェード
	RankingButtomFade( pVtx );

	g_pVtxBufferRanking -> Unlock();

	if( *FadeUse == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )
		{
			if( GamePad[ CntPad ] == NULL )
			{
				//	画面遷移
				if( GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) )
				{
					SetFade(FADE_OUT , MODE_TITLE);

					//	SE再生
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	音楽停止
					StopSound( SOUND_LABEL_BGM_RESULT );
				}	//	end of if
			}
			if( GamePad[ CntPad ] != NULL )
			{
				if( GetGamePadTrigger( BUTTOM_04 ) )
				{
					SetFade(FADE_OUT , MODE_TITLE);

					//	SE再生
					PlaySound( SOUND_LABEL_SE_ENTER );

					//	音楽停止
					StopSound( SOUND_LABEL_BGM_RESULT );
				}
			}
		}
	}
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void DrawRanking( void )
 引数:		なし
 戻り値:	なし
 説明:		ランキングの描画
-----------------------------------------------------------------------------*/
void DrawRanking( void )
{
	int Cnt;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferRanking , 0 , sizeof( VERTEX_2D ) );

	//	ランキング背景

	//	テクスチャ設定
	pDevice -> SetTexture( 0 , g_pTextureRankingBg );

	//	フェードの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);

	//	ランキング文字

	//	テクスチャ設定
	pDevice -> SetTexture( 0 , g_pTextureRankingAscii );

	//	フェードの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);

	//	順位
	//	テクスチャ設定
	pDevice -> SetTexture( 0 , g_pTextureRankingRank );

	for( Cnt = 0 ; Cnt < MAX_RANKING ; Cnt++ )
	{
		//	フェードの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , ( Cnt + TEXTURE ) * NUM_VERTEX , NUM_POLYGON);
	}	//	end of for

	//	ランキング数字

	//	テクスチャ設定
	pDevice -> SetTexture( 0 , g_pTextureRanking );

	for( Cnt = 0 ; Cnt < MAX_RANKING * ( SCORE_DIGIT - 1 ) ; Cnt++ )
	{
		//	フェードの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , ( Cnt + TEXTURE + MAX_RANKING ) * NUM_VERTEX , NUM_POLYGON);
	}	//	end of for

	//	ボタン

	//	テクスチャ設定
	pDevice -> SetTexture( 0 , g_pTextureRankingButtom );

	//	フェードの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8 , NUM_POLYGON);

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexRanking( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice
 戻り値:	なし
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexRanking( LPDIRECT3DDEVICE9 pDevice )
{
	int Cnt1 , Cnt2;
	int num;	//	数字を表示する変数
	int value;
	
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * ( ( SCORE_DIGIT * MAX_RANKING ) + TEXTURE + MAX_RANKING ) , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferRanking , NULL ) ) )
	{
		return E_FAIL;
	}

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;

	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferRanking -> Lock ( 0 , 0 ,( void** )&pVtx ,0 );

	//	ランキング背景

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f                , 0.0f                 , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f                 , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f                , 0.0f + SCREEN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f + SCREEN_WIDTH , 0.0f + SCREEN_HEIGHT , 0.0f );

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

	//	ポインタをずらす
	pVtx += 4;

	//	ランキング文字

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_ASCII_X                       , RANKING_ASCII_Y                 , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_ASCII_X + RANKING_ASCII_WIDTH , RANKING_ASCII_Y                 , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_ASCII_X                       , RANKING_ASCII_Y + RANKING_ASCII_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_ASCII_X + RANKING_ASCII_WIDTH , RANKING_ASCII_Y + RANKING_ASCII_HEIGHT , 0.0f );

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

	//	ポインタをずらす
	pVtx += 4;

	//	ボタン

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , ( RESULT_PUSH_POS_Y + 28.0f ) , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , ( RESULT_PUSH_POS_Y + 28.0f ) , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X                         , ( RESULT_PUSH_POS_Y + 28.0f ) + RESULT_PUSH_POS_HEIGHT - 20.0f , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( RESULT_PUSH_POS_X + RESULT_PUSH_POS_WIDTH , ( RESULT_PUSH_POS_Y + 28.0f ) + RESULT_PUSH_POS_HEIGHT - 20.0f , 0.0f );

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

	//	ポインタをずらす
	pVtx += 4;

	//	順位
	for( Cnt1 = 0 ; Cnt1 < MAX_RANKING ; Cnt1++ )
	{
			//	頂点座標の設定
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_RANK_X                      , ( RANKING_RANK_Y - 20.0f )                       - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_RANK_X + RANKING_RANK_WIDTH , ( RANKING_RANK_Y - 20.0f )                       - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_RANK_X                      , ( RANKING_RANK_Y - 20.0f ) + RANKING_RANK_HEIGHT - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_RANK_X + RANKING_RANK_WIDTH , ( RANKING_RANK_Y - 20.0f ) + RANKING_RANK_HEIGHT - ( Cnt1 * RANKING_RANK_HEIGHT ) , 0.0f );

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

			//	ポインタをずらす
			pVtx += 4;

	}	//	end of for

	//	ランキング数字
	for( Cnt1 = 0 ; Cnt1 < MAX_RANKING ; Cnt1++ )
	{
		value = g_RankingList[ Cnt1 ].score;

		for( Cnt2 = 0 ; Cnt2 < SCORE_DIGIT - 1 ; Cnt2++ )
		{
			num = value % 10;
			value /= 10;

			//	頂点座標の設定
			pVtx[ 0 ].pos = D3DXVECTOR3( RANKING_X                 - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y                  - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );
			pVtx[ 1 ].pos = D3DXVECTOR3( RANKING_X + RANKING_WIDTH - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y                  - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );
			pVtx[ 2 ].pos = D3DXVECTOR3( RANKING_X                 - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y + RANKING_HEIGHT - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );
			pVtx[ 3 ].pos = D3DXVECTOR3( RANKING_X + RANKING_WIDTH - ( Cnt2 * RANKING_WIDTH ) , RANKING_Y + RANKING_HEIGHT - ( Cnt1 * ( RANKING_HEIGHT * 2 ) ) , 0.0f );

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
			pVtx[ 0 ].tex = D3DXVECTOR2( 0.1f * num        , 0 );
			pVtx[ 1 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 0 );
			pVtx[ 2 ].tex = D3DXVECTOR2( 0.1f * num        , 1 );
			pVtx[ 3 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 1 );

			//	ポインタをずらす
			pVtx += 4;
		}	//	end of for

	}	//	end of for

	g_pVtxBufferRanking -> Unlock();

	return S_OK;
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void RankingButtomFade( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		ボタンのフェード
-----------------------------------------------------------------------------*/
void RankingButtomFade( VERTEX_2D* pVtx )
{
	if( g_RankingFade == FADE_IN )	//	フェードイン時の処理
	{
		g_RankingFadeColor.a -= FADE_RATE;	//	a値を減算して後ろの画面を浮かび上がらせる
		//	フェードインの終了
		if( g_RankingFadeColor.a < 0.0f )
		{
			g_RankingFadeColor.a = 0.0f;
			g_RankingFade = FADE_OUT;
		}	//	end od if
	}	//	end of if

	else if( g_RankingFade == FADE_OUT )	//	フェードアウト時の処理
	{
		g_RankingFadeColor.a += FADE_RATE;	//	a値を加算して後ろの画面を消していく
		//	フェードアウトの終了
		if( g_RankingFadeColor.a > 1.0f )
		{
			g_RankingFadeColor.a = 1.0f;
			g_RankingFade = FADE_IN;	//	フェードインに処理の切り替え
		}	//	end of if
	}	//	end of else if

	//	頂点色の設定
	pVtx[ 8  ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
	pVtx[ 9  ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
	pVtx[ 10 ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
	pVtx[ 11 ].color = D3DXCOLOR( g_RankingFadeColor.r , g_RankingFadeColor.g , g_RankingFadeColor.b , g_RankingFadeColor.a );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void RankTex( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		順位のテクスチャと色を変更
-----------------------------------------------------------------------------*/
void RankTex( VERTEX_2D* pVtx )
{
	//	5位

	//	頂点色の設定
	pVtx[ 12 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 13 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 14 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 15 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 12 ].tex = D3DXVECTOR2( 0 , 0.8f );
	pVtx[ 13 ].tex = D3DXVECTOR2( 1 , 0.8f );
	pVtx[ 14 ].tex = D3DXVECTOR2( 0 , 1.0f );
	pVtx[ 15 ].tex = D3DXVECTOR2( 1 , 1.0f );

	//	4位

	//	頂点色の設定
	pVtx[ 16 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 17 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 18 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );
	pVtx[ 19 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 16 ].tex = D3DXVECTOR2( 0 , 0.6f );
	pVtx[ 17 ].tex = D3DXVECTOR2( 1 , 0.6f );
	pVtx[ 18 ].tex = D3DXVECTOR2( 0 , 0.8f );
	pVtx[ 19 ].tex = D3DXVECTOR2( 1 , 0.8f );

	//	3位

	//	頂点色の設定
	pVtx[ 20 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );
	pVtx[ 21 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );
	pVtx[ 22 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );
	pVtx[ 23 ].color = D3DCOLOR_RGBA( 196 , 112 , 34 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 20 ].tex = D3DXVECTOR2( 0 , 0.4f );
	pVtx[ 21 ].tex = D3DXVECTOR2( 1 , 0.4f );
	pVtx[ 22 ].tex = D3DXVECTOR2( 0 , 0.6f );
	pVtx[ 23 ].tex = D3DXVECTOR2( 1 , 0.6f );

	//	2位

	//	頂点色の設定
	pVtx[ 24 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );
	pVtx[ 25 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );
	pVtx[ 26 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );
	pVtx[ 27 ].color = D3DCOLOR_RGBA( 192 , 192 , 192 , 192 );

	//	テクスチャ座標の設定
	pVtx[ 24 ].tex = D3DXVECTOR2( 0 , 0.2f );
	pVtx[ 25 ].tex = D3DXVECTOR2( 1 , 0.2f );
	pVtx[ 26 ].tex = D3DXVECTOR2( 0 , 0.4f );
	pVtx[ 27 ].tex = D3DXVECTOR2( 1 , 0.4f );

	//	1位

	//	頂点色の設定
	pVtx[ 28 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );
	pVtx[ 29 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );
	pVtx[ 30 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );
	pVtx[ 31 ].color = D3DCOLOR_RGBA( 255 , 215 , 0 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 28 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 29 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 30 ].tex = D3DXVECTOR2( 0 , 0.2f );
	pVtx[ 31 ].tex = D3DXVECTOR2( 1 , 0.2f );
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void Save( void )
 引数:		
 戻り値:
 説明:		セーブ
-----------------------------------------------------------------------------*/
void Save( void )
{
	FILE *fp;	//	ファイルポインタ

	fp = fopen( FILENAME , " wb " );	//	ファイルを開く
									
	if( fp == NULL )	//	エラーチェック
	{
		MessageBox( NULL , "ファイルが開けませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

//	テキストデータでセーブさせる
#ifdef ENABLE_PEOC_SAVE
	// データをセーブさせる
	for( int CntSave = 0 ; CntSave < MAX_RANKING ; CntSave++ )
	{
		fprintf( fp , "%d" , g_RankingList[ CntSave ].score );
	}	//	end of for
#else	//	バイナリデータでセーブさせる

	fwrite( &g_RankingList[ 0 ] , sizeof( RANKING ) , MAX_RANKING , fp );

#endif
	fclose( fp );	//	ファイルを閉じる

}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void Load( void )
 引数:		
 戻り値:	
 説明:		ロード
-----------------------------------------------------------------------------*/
void Load( void )
{
	FILE *fp;	//	ファイルポインタ

	fp = fopen( FILENAME , " rb " );	//	ファイルを開く
						
	//	エラーチェック
	if( fp == NULL )
	{
		MessageBox( NULL , "ファイルが開けませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}	//	end of if

//	テキストデータ出力させる
#ifdef ENABLE_PEOC_SAVE
	// データを人数分ロードさせる 
	for( int CntLoad = 0 ; CntLoad < MAX_RANKING ; CntLoad++ )
	{
		fscanf( fp , "%d" , &g_RankingList[ CntLoad ].score );
	}	//	end of func
#else	//	バイナリデータ出力させる

	fread( &g_RankingList[ 0 ] , sizeof( RANKING ) , MAX_RANKING , fp );

#endif
		
	fclose( fp );	//	ファイルを閉じる
}	//	end of func
/*-----------------------------------------------------------------------------
 関数名:	void BubbleSort( void )
 引数:		
 戻り値:	
 説明:		バブルソート
-----------------------------------------------------------------------------*/
void BubbleSort( void )
{
	int Cnt1 , Cnt2;
	unsigned int MaxScore = 0;

	for( Cnt1 = 0; Cnt1 < MAX_RANKING - 1 ; Cnt1++ )
	{
		for( Cnt2 = MAX_RANKING - 1 ; Cnt2 > Cnt1 ; Cnt2-- )
		{
			if( g_RankingList[ Cnt2 ].score < g_RankingList[ Cnt2 - 1 ].score )
			{
				//	入れ替え処理
				MaxScore = g_RankingList[ Cnt2 ].score;

				g_RankingList[ Cnt2 ].score = g_RankingList[ Cnt2 - 1 ].score;

				g_RankingList[ Cnt2 - 1 ].score = MaxScore;

			}	//	end of if
		}	//	end of for
	}	//	end of for
}	//	end of func