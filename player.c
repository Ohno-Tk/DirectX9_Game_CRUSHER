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
			・ヘッダファイル追加
				#include <stdio.h>
				#include "main.h"
				#include "player.h"
				#include "bg.h"
			・定数定義追加
				#define PLAYER_TEXTURENAME
			・グローバル変数追加
				LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//	テクスチャインターフェース
				LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;//頂点バッファのインターフェースポインタ
				PLAYER g_Player;
			・プロトタイプ宣言追加
				HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
				void MakePlayerPos( VERTEX_2D* pVtx );	//	座標変更
		2016/08/22
			・ヘッダファイル追加
				#include "base.h"
		2016/08/28
			・ヘッダファイル追加
				#include "score.h"
		2016/09/07
			・プロトタイプ宣言追加
				void PlayerOperation( void );	//	プレイヤーの基本操作
				void GoolPlayerOperation( void );	//	ゴールした時のプレイヤーの処理
				void GoolDecision( void );	//	ゴール判定
				void ScrollObject( void );	//	プレイヤー移動時の足場と敵のスクロール
		2016/09/14
			・プロトタイプ宣言追加
					//	スクロールするオブジェクトの速さ変更
				void AttackDecision( void );	//	攻撃判定
-------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "usability.h"
#include "sound.h"
#include "base.h"
#include "score.h"
#include "time.h"
#include "effect.h"
#include "particle.h"
#include "tutorial.h"
#include "guid.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define PLAYER_TEXTURENAME "data/TEXTURE/game/player2.png"

#define	JUMP_VALUE ( 20.0f )		// ジャンプ量

#define POS_HAMMER ( 20.0f )	//	ハンマーの座標

#define UP_PLAYER_CNT ( 50 )

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//	テクスチャインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;//頂点バッファのインターフェースポインタ

PLAYER g_Player;

//	テクスチャ切り替え
float fUesult = 0 , fVesult = 0;

int g_CntUp = 0;
int g_CntPlayerFrame = 0;

bool g_GoolMove = false;
bool g_HitUp = false;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice );	//	頂点の作成
void MakePlayerPos( VERTEX_2D* pVtx );					//	座標変更
void PlayerOperation( void );							//	プレイヤーの基本操作
void KeyboardOperation( void );							//	キーボードの基本操作
void GamePadOperation( void );							//	ゲームパッドの基本操作
void GoolPlayerOperation( void );						//	ゴールした時のプレイヤーの処理
void GoolDecision( void );								//	ゴール判定
void GoolHeightScore( void );							//	ゴールの高さによってスコアの変化
void GoolModeGame( void );								//	ゲームモードの時のプレイヤー処理
void GoolModeTutorial( void );							//	チュートリアルモードの時のプレイヤー処理
void ScrollObject( void );								//	プレイヤー移動時の足場と敵のスクロール
void ScrollMoveAlterObject( void );						//	スクロールするオブジェクトの速さ変更
void ScrollMoveRepositObject( void );					//	スクロールするオブジェクトの速さを元に戻す
void AttackDecision( void );							//	攻撃判定
void EnemyDecision( void );								//	敵との当たり判定
void HitOperation( void );								//	敵とヒットした時の行動

/*-----------------------------------------------------------------------------
 関数名:	void InitPlayer(void)
 引数:		なし
 戻り値:	なし
 説明:		プレイヤーの初期化
-----------------------------------------------------------------------------*/
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	足場情報の取得
	BASE *Base = GetBase( 0 );

	//	エラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , PLAYER_TEXTURENAME , &g_pTexturePlayer  ) ) )
	{
		MessageBox( NULL , "プレイヤーの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );
	}

	//	プレイヤーの初期化
	g_Player.Pos.x = 100.0f;
	g_Player.Pos.y = Base -> pos . y;
	g_Player.OldPos.x = 0.0f;
	g_Player.OldPos.y = 0.0f;
	g_Player.Size.x = 100.0f;
	g_Player.Size.y = 100.0f;
	g_Player.Move.x = 2.0f;
	g_Player.Move.y = 10.0f;
	g_Player.Direct = 1.0f;
	g_Player.HitMove_Y = 5.0f;
	g_Player.gool = false;
	g_Player.Hit = false;
	g_Player.Jump = false;
	g_Player.fall = false;
	g_Player.bUse = true;

	g_GoolMove = false;
	g_HitUp = false;

	fUesult = 0.0f;
	fVesult = 0.0f;
	
	g_CntUp = 0;
	g_CntPlayerFrame = 0;

	MakeVertexPolygon( pDevice );	//	頂点の作成

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitPlayer(void)
 引数:		なし
 戻り値:	なし
 説明:		プレイヤーの終了
-----------------------------------------------------------------------------*/
void UninitPlayer(void)
{
	fUesult = 0.0f;
	fVesult = 0.0f;

	if( g_pTexturePlayer != NULL )	//	テクスチャポリゴン開放
	{
		g_pTexturePlayer -> Release();
		g_pTexturePlayer = NULL;
	}	//	end of if
	if(g_pVtxBufferPlayer != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferPlayer -> Release();
		g_pVtxBufferPlayer = NULL;
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdatePlayer(void)
 引数:		なし
 戻り値:	なし
 説明:		プレイヤーの更新
-----------------------------------------------------------------------------*/
void UpdatePlayer(void)
{
	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	VERTEX_2D* pVtx;

	//	前の座標の取得
	g_Player.OldPos.x = g_Player.Pos.x;
	g_Player.OldPos.y = g_Player.Pos.y;

	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferPlayer -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	プレイヤーがゴールしていなかったら
	if( Base -> GoolFalg == false )
	{
		PlayerOperation();	//	プレイヤーの基本操作
	}
	//	ゴールしていたら
	else if( Base -> GoolFalg == true )
	{
		GoolPlayerOperation();	//	ゴールした時のプレイヤーの処理
	}

	MakePlayerPos( pVtx );	//	プレイヤーの座標変更

	g_pVtxBufferPlayer -> Unlock(); //これ以降触れてはいけない

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawPlayer(void)
 引数:		なし
 戻り値:	なし
 説明:		プレイヤーの描画
-----------------------------------------------------------------------------*/
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferPlayer , 0 , sizeof( VERTEX_2D ) );

	pDevice -> SetTexture( 0 , g_pTexturePlayer );

	if( g_Player.bUse == true )
	{
		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0, NUM_POLYGON);
	}	//	end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice )
 引数:		なし
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPolygon( LPDIRECT3DDEVICE9 pDevice )
{
	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferPlayer , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferPlayer -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );

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


	g_pVtxBufferPlayer -> Unlock();

	return S_OK;
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void MakePlayerPos( VERTEX_2D* pVtx )
 引数:		VERTEX_2D* pVtx
 戻り値:	なし
 説明:		プレイヤーの座標変更
-----------------------------------------------------------------------------*/
void MakePlayerPos( VERTEX_2D* pVtx )
{
	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Player.Pos.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Player.Pos.x+g_Player.Size.x , g_Player.Pos.y+g_Player.Size.y , 0.0f );

	//	プレイヤーの向きによってテクスチャの向きを変える
	if( g_Player.Direct == 1.0f )
	{
		//	テクスチャ座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( fUesult             , 0 + fVesult );
		pVtx[ 1 ].tex = D3DXVECTOR2( TEX_WIDTH + fUesult , 0 + fVesult );
		pVtx[ 2 ].tex = D3DXVECTOR2( fUesult             , TEX_HEIGHT + fVesult );
		pVtx[ 3 ].tex = D3DXVECTOR2( TEX_WIDTH + fUesult , TEX_HEIGHT + fVesult );
	}
	else if( g_Player.Direct == -1.0f )
	{
		//	テクスチャ座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( TEX_WIDTH+fUesult , 0 + fVesult );
		pVtx[ 1 ].tex = D3DXVECTOR2( fUesult           , 0 + fVesult );
		pVtx[ 2 ].tex = D3DXVECTOR2( TEX_WIDTH+fUesult , TEX_HEIGHT + fVesult );
		pVtx[ 3 ].tex = D3DXVECTOR2( fUesult           , TEX_HEIGHT + fVesult );
	}
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void PlayerOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		プレイヤーの基本操作
-----------------------------------------------------------------------------*/
void PlayerOperation( void )
{
	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	if( g_Player.Hit == true )
	{
		//	敵とヒットした時の行動
		HitOperation();
	}
	else if( g_Player.Hit == false )
	{
		for( int CntPad = 0 ; CntPad < MAX_PAD ; CntPad++ , GamePad++ )

		//	ゲームパッドが無かったら
		if( GamePad[ CntPad ] == NULL )
		{
			//	キーボードの基本操作
			KeyboardOperation();
		}
		//	ゲームパッドがあったら
		else if( GamePad[ CntPad ] != NULL )
		{
			//	ゲームパッドデバイス取得
			GamePadOperation();
		}

		//	敵との当たり判定
		EnemyDecision();
	
		g_Player.Move.y += GRAVITY;	//	重力を加える
		g_Player.Pos.y += g_Player.Move.y;

		//	ゴール判定
		GoolDecision();

		//	ジャンプさせない処理
		if( (g_Player.Pos.y + g_Player.Size.y) > SCREEN_HEIGHT - 25.0f )
		{
			g_Player.Jump = true;
			g_Player.fall = true;
		}

		//	画面がい処理
		if( g_Player.Pos.y > SCREEN_HEIGHT + 200.0f )
		{
			g_Player.bUse = false;
		}	//	end of if
	}	//	end of else if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void KeyboardOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		キーボードの基本操作
-----------------------------------------------------------------------------*/
void KeyboardOperation( void )
{

	//	攻撃
	if( GetKeyboardTrigger( DIK_SPACE ) )
	{
		fUesult = 0.5f;

		//	攻撃判定
		AttackDecision();

		//	SE再生
		//	攻撃音

	}	//	end of if
		
	//	攻撃ボタンを離したら
	else if( GetKeyboardRelease( DIK_SPACE ) )
	{
		fUesult = 0;
	}	//	end of if
		
	//	ダッシュ
	if( GetKeyboardPress( DIK_LSHIFT ) )
	{
		g_Player.Move.x = 4.0f;

		//	スクロールするオブジェクトの速さ変更
		ScrollMoveAlterObject();
	}	//	end of if
		
	//	攻撃ボタンが離したら
	else if( GetKeyboardRelease( DIK_LSHIFT ) )
	{
		g_Player.Move.x = 2.0f;

		//	スクロールするオブジェクトの速さを元に戻す
		ScrollMoveRepositObject();
	} // end of else if
		
	//	ジャンプ
	if ( GetKeyboardPress( DIK_W ) || GetKeyboardPress(DIK_UPARROW) )
	{
		if( g_Player.Jump == false )
		{
			g_Player.Move.y = -JUMP_VALUE;
			g_Player.Jump = true;
		}	//	end of if
	}	//	end of if
		
	//	移動
	if ( GetKeyboardPress( DIK_A ) || GetKeyboardPress(DIK_LEFTARROW) )	//	左移動
	{
		g_Player.Direct = -1.0f;	//	向きの変更

		//	画面がい処理
		if( g_Player.Pos.x < 0 )
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of if
		else
		{
			g_Player.Pos.x -= g_Player.Move.x;
		}	//	end of else
	}	//	end of if
		
	if ( GetKeyboardPress( DIK_D ) || GetKeyboardPress(DIK_RIGHTARROW) )	//	右移動
	{
		//	背景、敵、足場をスクロールさせる
		if( g_Player.Pos.x+g_Player.Size.x > SCREEN_WIDTH*0.5f )
		{
			ScrollObject();
		}	//	end of if
		else
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of else

		g_Player.Direct = 1.0f;	//	向きの変更

	}	//	end of if
}

/*-----------------------------------------------------------------------------
 関数名:	void GamePadOperation( void )
 引数:		なし
 戻り値:	なし
 説明:		ゲームパッドの基本操作
-----------------------------------------------------------------------------*/
void GamePadOperation( void )
{
	//	スティックの取得
	LONG Stick_X = GetGamePadStick_X();

	//	攻撃
	//if( GetGamePadTrigger( BUTTOM_01 ) )
	if( GetGamePadTrigger( BUTTOM_06 ) )
	{
		fUesult = 0.5f;

		//	攻撃判定
		AttackDecision();

		//	SE再生
		//	攻撃音

	}	//	end of if
		
	//	攻撃ボタンを離したら
	//else if( GetGamePadRelease( BUTTOM_01 ) )
	else if( GetGamePadRelease( BUTTOM_06 ) )
	{
		fUesult = 0;
	}	//	end of if
		
	//	ダッシュ
	if( GetGamePadPress( BUTTOM_03 ) )
	{
		g_Player.Move.x = 4.0f;

		//	スクロールするオブジェクトの速さ変更
		ScrollMoveAlterObject();
	}	//	end of if
		
	//	攻撃ボタンが離したら
	else if( GetGamePadRelease( BUTTOM_03 ) )
	{
		g_Player.Move.x = 2.0f;

		//	スクロールするオブジェクトの速さを元に戻す
		ScrollMoveRepositObject();
	} // end of else if
		
	//	ジャンプ
	if ( GetGamePadPress( BUTTOM_04 ) )
	{
		if( g_Player.Jump == false )
		{
			g_Player.Move.y = -JUMP_VALUE;
			g_Player.Jump = true;
		}	//	end of if
	}	//	end of if
		
	//	移動
	if ( Stick_X < 0 )	//	左移動
	{
		g_Player.Direct = -1.0f;	//	向きの変更

		//	画面がい処理
		if( g_Player.Pos.x < 0 )
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of if
		else
		{
			g_Player.Pos.x -= g_Player.Move.x;
		}	//	end of else
	}	//	end of if
		
	if ( Stick_X > 0 )	//	右移動
	{
		//	背景、敵、足場をスクロールさせる
		if( g_Player.Pos.x+g_Player.Size.x > SCREEN_WIDTH*0.5f )
		{
			ScrollObject();
		}	//	end of if
		else
		{
			g_Player.Pos.x += g_Player.Move.x;
		}	//	end of else

		g_Player.Direct = 1.0f;	//	向きの変更

	}	//	end of if
}

/*-----------------------------------------------------------------------------
 関数名:	void GoolPlayerOperation( void )
 引数:		なし
 戻り値:	
 説明:		ゴールした時のプレイヤーの処理
-----------------------------------------------------------------------------*/
void GoolPlayerOperation( void )
{
	//	今のモード
	MODE *Mode = GetMode();

	fUesult = 0.0f;
	fVesult = 0.0f;

	//	ゲームモードの時
	if( *Mode == MODE_GAME )
	{
		//	ゲームモードの時のプレイヤー処理
		GoolModeGame();
	}	//	end of if

	//	チュートリアルモードの時
	else if( *Mode == MODE_TUTORIAL )
	{
		//	チュートリアルモードの時のプレイヤー処理
		GoolModeTutorial();
	}	//	end of else if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void ScrollField( void )
 引数:		なし
 戻り値:	
 説明:		ゴール判定
-----------------------------------------------------------------------------*/
void GoolDecision( void )
{
	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( MAX_GOOL );

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	//	ゴール判定
	if( RectangleDecision( g_Player.Pos.x , g_Player.Pos.y , g_Player.Size.x-POS_HAMMER , g_Player.Size.y , Base->pos.x+30.0f , Base->pos.y , Base->size.x-30.0f , Base->size.y ) )
	{
		Base -> GoolFalg = true;

		if( *Mode == MODE_GAME )
		{
			//	ゴールの高さによってスコアの変化
			GoolHeightScore();
		}
		PlaySound( SOUND_LABEL_SE_GOOL );
	} // end of if
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void GoolHeightScore( void )
 引数:		なし
 戻り値:	
 説明:		ゴールの高さによってスコアの変化
-----------------------------------------------------------------------------*/
void GoolHeightScore( void )
{
	//	スコアの情報の取得
	int *Score = GetScore();

	if( 100.0f > g_Player.Pos.y )
	{
		*Score += 5000;
	}

	else if( 200.0f < g_Player.Pos.y && g_Player.Pos.y < 400.0f )
	{
		*Score += 1500;
	}

	else
	{
		*Score += 500;
	}
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void GoolModeGame( void )
 引数:		なし
 戻り値:	
 説明:		ゲームモードの時のプレイヤー処理
-----------------------------------------------------------------------------*/
void GoolModeGame( void )
{
	//	スコア情報の取得
	int *Score = GetScore();
	bool *ScoreFlag = GetScoreFlag();

	//	タイム情報の取得
	int *Time = GetTime();

	if( g_GoolMove == false )
	{
		g_Player.Pos.y += 2;
		//	ゴールの下まで行ったら
		if( g_Player.Pos.y + g_Player.Size.y > SCREEN_HEIGHT - 40.0f )
		{
			g_GoolMove = true;

		}	//	end of if
	}	//	end of if

	if( g_GoolMove == true )
	{
		g_Player.Pos.x += g_Player.Move.x;
		//	画面範囲外に行ったら
		if( g_Player.Pos.x > SCREEN_WIDTH && *Time <= 0 )
		{
			g_Player.gool = true;
		}	//	end of if
	}	//	end of if

	*Time -= 75;
	if( *ScoreFlag == false )
	{
		*Score += 50;
	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void GoolModeTutorial( void )
 引数:		なし
 戻り値:	
 説明:		チュートリアルモードの時のプレイヤー処理
-----------------------------------------------------------------------------*/
void GoolModeTutorial( void )
{
	if( g_GoolMove == false )
	{
		g_Player.Pos.y += 2;
		//	ゴールの下まで行ったら
		if( g_Player.Pos.y + g_Player.Size.y > SCREEN_HEIGHT - 40.0f )
		{
			g_GoolMove = true;
		}	//	end of if
	}	//	end of if

	if( g_GoolMove == true )
	{
		g_Player.Pos.x += g_Player.Move.x;
		//	画面範囲外に行ったら
		if( g_Player.Pos.x > SCREEN_WIDTH )
		{
			g_Player.gool = true;
		}	//	end of if
	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void ScrollObject( void )
 引数:		なし
 戻り値:	
 説明:		プレイヤー移動時の足場と敵のスクロール
-----------------------------------------------------------------------------*/
void ScrollObject( void )
{
	int CntEnemy;	//	敵をforで数える
	int CntBase;	//	足場をforで数える
	int CntTutorial;

	//	背景情報の取得
	float *pBgTex = GetBgTex();
	float *pBgVTex = GetVTex();

	//	敵情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	足場情報の取得
	BASE *Base = GetBase( 0 );

	//	チュートリアル情報の取得
	TUTORIAL *Tutorial = GetTutorial();

	//	案内情報の取得
	GUIDSIGNBOARD *Guid = GetGuid();

	//	背景のスクロール
	*pBgTex += BG_TEX_SPEED;


	if( *pBgTex >= 0.999f && *pBgTex <= 1.0f )
	{
		*pBgVTex += 0.1f;
		*pBgTex = 0.0f;
	} // end of if

	//	足場のスクロール	
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ , Base++ )
	{
		if( Base -> use == true )
		{
			Base -> pos . x -= Base ->move_X;
				
			if( Base -> pos . x + Base -> size . x < 0 )
			{
				Base -> use = false;
			}	//	end of if
		}	//	end of if
	}	//	end of for
				
	//	敵のスクロール	
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		if( Enemy -> Use == true )
		{
			Enemy -> Pos.x -= Enemy -> Scroll_Move_X;
		}	//	end of if
	}	//	end of for

	//	チュートリアルのスクロール
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ , Tutorial++ )
	{
		Tutorial -> pos.x -= Tutorial -> Move_X;
	}

	//	案内のスクロール
	Guid -> pos.x -= Guid -> move_X;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void ScrollMoveAlterObject( void )
 引数:		なし
 戻り値:	
 説明:		スクロールするオブジェクトの速さ変更
-----------------------------------------------------------------------------*/
void ScrollMoveAlterObject( void )
{
	int CntEnemy;	//	敵をforで数える
	int CntBase;	//	足場をforで数える
	int CntTutorial;

	//	チュートリアル情報の取得
	TUTORIAL *Tutorial = GetTutorial();

	//	敵情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( 0 );

	//	案内情報の取得
	GUIDSIGNBOARD *Guid = GetGuid();

	//	足場
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ , Base++ )
	{
		Base->move_X = MOVE_AFTER;
	}	//	end of for

	//	敵
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		Enemy -> Scroll_Move_X = MOVE_AFTER;
	}	//	end of for

	//	チュートリアル
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ , Tutorial++ )
	{
		Tutorial -> Move_X = MOVE_AFTER;
	}	//	end of for

	//	案内のスクロール
	Guid -> move_X = MOVE_AFTER;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void ScrollMoveRepositObject( void )
 引数:		なし
 戻り値:	
 説明:		スクロールするオブジェクトの速さを元に戻す
-----------------------------------------------------------------------------*/
void ScrollMoveRepositObject( void )
{
	int CntEnemy;	//	敵をforで数える
	int CntBase;	//	足場をforで数える
	int CntTutorial;

	//	チュートリアル情報の取得
	TUTORIAL *Tutorial = GetTutorial();

	//	敵情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	足場情報(ゴール)の取得
	BASE *Base = GetBase( 0 );

	//	案内情報の取得
	GUIDSIGNBOARD *Guid = GetGuid();

	//	足場
	for( CntBase = 0 ; CntBase < MAX_BASE ; CntBase++ , Base++ )
	{
		Base->move_X = MOVE_REPOSIT;
	}	//	end of for

	//	敵
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		Enemy -> Scroll_Move_X = MOVE_REPOSIT;
	}	//	end of for

	//	チュートリアル
	for( CntTutorial = 0 ; CntTutorial < MAX_TUTORIAL ; CntTutorial++ , Tutorial++ )
	{
		Tutorial -> Move_X = MOVE_REPOSIT;
	}	//	end of for

	//	案内
	Guid -> move_X = MOVE_REPOSIT;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void AttackDecision( void )
 引数:		なし
 戻り値:	
 説明:		攻撃判定
-----------------------------------------------------------------------------*/
void AttackDecision( void )
{
	int CntEnemy;	//	敵をforで数える

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );

	//	敵情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	スコアの情報の取得
	int *Score = GetScore();

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		if( Enemy ->Use == true )
		{
			//	向きによって判定を変える

			//	右向き
			if( g_Player.Direct == 1.0f )
			{
				if( RectangleDecision( ( g_Player.Pos.x + g_Player.Size.y ) - POS_HAMMER , g_Player.Pos.y , POS_HAMMER + 15.0f , g_Player.Size.y , Enemy->Pos.x , Enemy->Pos.y , Enemy->Size.x , Enemy->Size.y ) )
				{
					Enemy ->Use = false;

					if( *Mode == MODE_GAME )
					{
						//	スコアの加算
						*Score += Enemy->Score;
					}

					if( GamePad[ 0 ] != NULL )
					{
						//	バイブレーションの開始
						VibrationStart( 1 , 0 );
					}

					//	エフェクトのセット
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );

					//	SE再生
					PlaySound( SOUND_LABEL_SE_HIT );
				}
			}
			//	左向き
			else if( g_Player.Direct == -1.0f )
			{
				if( RectangleDecision( g_Player.Pos.x - 15.0f, g_Player.Pos.y , POS_HAMMER , g_Player.Size.y , Enemy->Pos.x , Enemy->Pos.y , Enemy->Size.x , Enemy->Size.y ) )
				{
					Enemy ->Use = false;

					if( *Mode == MODE_GAME )
					{
						//	スコアの加算
						*Score += Enemy->Score;
					}

					if( GamePad[ 0 ] != NULL )
					{
						//	バイブレーションの開始
						VibrationStart( 1 , 0 );
					}

					//	エフェクトのセット
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );
					SetParticle( Enemy->Pos.x + Enemy->Size.x * 0.5f , Enemy->Pos.y + Enemy->Size.y * 0.5f );

					//	SE再生
					PlaySound( SOUND_LABEL_SE_HIT );
				}	//	end of if
			}	//	end of else if
		}	//	end of if
	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void EnemyDecision( void )
 引数:		なし
 戻り値:	
 説明:		敵との当たり判定
-----------------------------------------------------------------------------*/
void EnemyDecision( void )
{
	int CntEnemy;	//	敵をforで数える

	//	敵情報の取得
	ENEMY *Enemy = GetEnemy( 0 );

	//	ゲームパッドデバイス取得
	LPDIRECTINPUTDEVICE8 *GamePad = GetGamePad( 0 );


	//	敵の当たり判定
	for( CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{
		if( RectangleDecision( g_Player.Pos.x , g_Player.Pos.y , g_Player.Size.x-POS_HAMMER , g_Player.Size.y , Enemy->Pos.x + 40.0f , Enemy->Pos.y + 30.0f , Enemy->Size.x - 80.0f , Enemy->Size.y - 40.0f ) && Enemy -> Use == true )
		{
			g_Player.Hit = true;

			if( GamePad[ 0 ] != NULL )
			{
				//	バイブレーションの開始
				VibrationStart( 1 , 0 );
			}

		}	//	end of if
	}	//	end of for
}	//	end of for

/*-----------------------------------------------------------------------------
 関数名:	void HitOperation( void )
 引数:		なし
 戻り値:	
 説明:		敵とヒットした時の行動
-----------------------------------------------------------------------------*/
void HitOperation( void )
{

	//	テクスチャ変更
	fUesult = 0.0f;
	fVesult = 0.5f;

	g_Player.Pos.y -= g_Player.HitMove_Y;

	g_CntUp++;

	if( g_HitUp == false )
	{
		if( g_CntUp % UP_PLAYER_CNT == 0 )
		{
			g_Player.HitMove_Y *= -1.7f;
			g_HitUp = true;
		}
	}

	//	画面外処理
	if( g_Player.Pos.y > SCREEN_HEIGHT )
	{
		g_Player.bUse = false;
	}	//	end of if
}

/*-----------------------------------------------------------------------------
 関数名:	PLAYER *GetPlayer(void)
 引数:		なし
 戻り値:	return &g_Player;
 説明:		プレイヤー情報の取得
-----------------------------------------------------------------------------*/
PLAYER *GetPlayer(void)
{
	return &g_Player;
}	//	end of func
